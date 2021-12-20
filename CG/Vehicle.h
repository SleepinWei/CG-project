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
	btTransform ChassisTransform;

	Vehicle(btDiscreteDynamicsWorld* world) :PhysicsModel(world)
	{
		collisionShape = new btBoxShape(btVector3(1.0f, 0.5f, 2.0f));

		btQuaternion rotation = btQuaternion(btVector3(0, 1, 0), btScalar(2* pi));
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
		btScalar wheelHeight = 0.4f;
		btVector3 connectionPointCS[4] = { btVector3(-1.5f, wheelHeight, 0.8f),btVector3(-1.5f, wheelHeight, -0.8f), btVector3(1.5f, wheelHeight,0.8f), btVector3(1.5f, wheelHeight, -0.8f) };	//轮胎位置
		for (int i = 0; i < 2; i++)
		{
			btVector3 wheelDirectionCS0 = btVector3(0.0f, -1.0f, 0.0f);	//轮胎方向
			btVector3 wheelAxleCS = btVector3(0.0f, 0.0f, -1.0f);	//轮胎轴
			btScalar suspensionRestLength = 0.6f;	//悬架长度
			btScalar wheelRadius = 0.4f;	//轮胎半径
			bool front = true;
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
		btScalar steering_new = steer_cur + 0.4 * deltatime;
		if (fabs(deltatime) < 1e-6)
			steering_new = 0;
		if (steering_new > 3.0)
			steering_new = 3.0;
		else if (steering_new < -3.0)
			steering_new = -3.0;
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
		else
			Steering(0);
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
		//std::cout << wheeltr.getRotation().getX() << " " << wheeltr.getRotation().getY() << " " << wheeltr.getRotation().getZ() << " " << wheeltr.getRotation().getW() << std::endl;
		glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(modelTransform.getOrigin().getX(), modelTransform.getOrigin().getY() - 0.5, modelTransform.getOrigin().getZ()));
		glm::mat4 rotation = glm::toMat4(glm::quat(modelTransform.getRotation().getW(), modelTransform.getRotation().getX(), modelTransform.getRotation().getY(), modelTransform.getRotation().getZ()));
		glm::mat4 worldTransform = translation * rotation;
		return worldTransform;
	}
	glm::vec3 getPos()
	{
		return glm::vec3(modelTransform.getOrigin().getX(), modelTransform.getOrigin().getY() - 0.5, modelTransform.getOrigin().getZ());
	}
	glm::vec3 getFront()
	{
		return glm::vec3(RaycastModel->getForwardVector().getX(), RaycastModel->getForwardVector().getY(), RaycastModel->getForwardVector().getZ());
	}
};

#endif