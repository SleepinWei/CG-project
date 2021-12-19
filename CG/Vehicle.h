#ifndef VEHICLE_H
#define VEHICLE_H

#include "Object.h"
#include<iomanip>
#include <bullet/BulletDynamics/Vehicle/btRaycastVehicle.h>
//#include <bullet/BulletDynamics/Vehicle/btVehicleRaycaster.h>

const double pi = 3.14159265359;

class Vehicle : public PhysicsModel
{
private:
	btScalar engine_force;
	btRigidBody* chassis;
	btVehicleRaycaster* Raycaster;
	btRaycastVehicle* RaycastModel;
public:
	btScalar positionX;
	btScalar positionY;
	btScalar positionZ;
	btTransform WheelTransform[4];
	btTransform ChassisTransform;

	Vehicle(btDiscreteDynamicsWorld* world) :PhysicsModel(world)
	{
		collisionShape = new btBoxShape(btVector3(1.0f, 0.5f, 2.0f));

		btQuaternion rotation = btQuaternion(btVector3(0, 1, 0), btScalar(6.28));
		btTransform vehicleTransform(rotation, btVector3(0, 1, 0));
		motionState = new btDefaultMotionState(vehicleTransform);
		btScalar mass = 10.0;
		btVector3 inertia(0, 0, 0);
		collisionShape->calculateLocalInertia(mass, inertia);
		btRigidBody::btRigidBodyConstructionInfo chassisRigidBodyCI(mass, motionState, collisionShape, inertia);
		chassis = new btRigidBody(chassisRigidBodyCI);
		chassis->setActivationState(DISABLE_DEACTIVATION);
		dynamicsWorld->addRigidBody(chassis);
	}
	~Vehicle()
	{
		delete Raycaster;
		delete RaycastModel;
	}
	void InitVehicle()
	{
		btRaycastVehicle::btVehicleTuning tuning;	//计算相关参数
		tuning.m_suspensionStiffness = 20.f;
		tuning.m_suspensionCompression = 4.4f;
		tuning.m_suspensionDamping = 2.3f;
		tuning.m_frictionSlip = 1000;
		Raycaster = new btDefaultVehicleRaycaster(dynamicsWorld);
		RaycastModel = new btRaycastVehicle(tuning, chassis, Raycaster);
		RaycastModel->setCoordinateSystem(2, 1, 0);	//坐标顺序
		btVector3 connectionPointCS[4] = { btVector3(-0.6f, 0.5f, 0.7f),btVector3(0.6f, 0.5f, 0.7f), btVector3(-0.6f, 0.5f,-0.7f), btVector3(0.6f, 0.5f, -0.7f) };	//轮胎位置
		for (int i = 0; i < 4; i++)
		{
			btVector3 wheelDirectionCS0 = btVector3(0.0f, -1.0f, 0.0f);	//轮胎方向
			btVector3 wheelAxleCS = btVector3(0.0f, 0.0f, -1.0f);	//轮胎轴
			btScalar suspensionRestLength = 0.6f;	//悬架长度
			btScalar wheelRadius = 0.4f;	//轮胎半径
			bool front = (i < 2) ? true : false;
			RaycastModel->addWheel(connectionPointCS[i], wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, tuning, front);
		}
		engine_force = 0;
		dynamicsWorld->addVehicle(RaycastModel);
	}
	void Speedup(double control)
	{
		engine_force = 10 * control;
		for (int i = 0; i < RaycastModel->getNumWheels(); i++)
			RaycastModel->applyEngineForce(engine_force, i);
	}
	void Steering(double deltatime)
	{
		btScalar steer_cur = RaycastModel->getSteeringValue(0);
		btVector3 orient = chassis->getLinearVelocity();
		//std::cout << -orient.getX() << " " << orient.getZ() << std::endl;
		if (orient.getX() > 1 && orient.getZ() > 1)
		{
			steer_cur = atan(orient.getZ() / (-orient.getX()));
			if (-orient.getX() < -1 && orient.getZ() > 1)
				steer_cur += pi;
			else if (-orient.getX() < -1e-1 && orient.getZ() < -1e-1)
				steer_cur -= pi;
			if (fabs(RaycastModel->getSteeringValue(0) - steer_cur) > 0.5)	//在倒车
				steer_cur = RaycastModel->getSteeringValue(0);
		}
		btScalar steering_new = steer_cur + 0.4 * deltatime;
		if (steering_new > pi)
			steering_new -= 2 * pi;
		else if (steering_new < -pi)
			steering_new += 2 * pi;
		for (int i = 0; i < RaycastModel->getNumWheels(); i++)
			RaycastModel->setSteeringValue(steering_new, i);
	}
	void getKeyboard(GLFWwindow* window, float deltaTime)
	{
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			Speedup(0.75);
		else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			Speedup(-0.75);
		else
			Speedup(0);
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			Steering(deltaTime);
		else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			Steering(-deltaTime);
	}


	void updateTransform(double deltaTime)
	{
		RaycastModel->updateVehicle(deltaTime);
	}
	void getPosition()
	{
		modelTransform = RaycastModel->getChassisWorldTransform();
		positionX = modelTransform.getOrigin().getX();
		positionY = modelTransform.getOrigin().getY();
		positionZ = modelTransform.getOrigin().getZ();
	}
	glm::mat4 getTransform()
	{
		chassis->getMotionState()->getWorldTransform(modelTransform);

		btQuaternion steering(btVector3(0, 1, 0), RaycastModel->getSteeringValue(0));
		btTransform trans(steering, btVector3(modelTransform.getOrigin().getX(), modelTransform.getOrigin().getY(), modelTransform.getOrigin().getZ()));
		//modelTransform = RaycastModel->getChassisWorldTransform();
		glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(modelTransform.getOrigin().getX(), modelTransform.getOrigin().getY() - 0.5, modelTransform.getOrigin().getZ()));
		glm::mat4 rotation = glm::mat4(glm::quat(trans.getRotation().getW(), trans.getRotation().getX(), trans.getRotation().getY(), trans.getRotation().getZ()));
		glm::mat4 worldTransform = translation * rotation;
		return worldTransform;
	}
	glm::vec3 getFront()
	{
		btQuaternion steering(btVector3(0, 1, 0), RaycastModel->getSteeringValue(0));
		btTransform trans(steering, btVector3(modelTransform.getOrigin().getX(), modelTransform.getOrigin().getY(), modelTransform.getOrigin().getZ()));
		return glm::vec3(trans.getRotation().getX(), trans.getRotation().getY(), trans.getRotation().getZ());
	}
};

#endif