#ifndef VEHICLE_H
#define VEHICLE_H

#include "Object.h"
#include <bullet/BulletDynamics/Vehicle/btRaycastVehicle.h>
//#include <bullet/BulletDynamics/Vehicle/btVehicleRaycaster.h>

class Vehicle : public PhysicsModel
{
private:
	btScalar engine_force;
	btRigidBody* chassis;
	btVehicleRaycaster* Raycaster;
	btRaycastVehicle* RaycastModel;
public:
	btTransform WheelTransform[4];
	btTransform ChassisTransform;

	Vehicle(btDiscreteDynamicsWorld* world) :PhysicsModel(world)
	{
		collisionShape = new btBoxShape(btVector3(1.0f, 0.5f, 2.0f));
		btTransform vehicleTransform;
		vehicleTransform.setIdentity();
		vehicleTransform.setOrigin(btVector3(0, 1, 0));
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
		Raycaster = new btDefaultVehicleRaycaster(dynamicsWorld);
		RaycastModel = new btRaycastVehicle(tuning, chassis, Raycaster);
		RaycastModel->setCoordinateSystem(0, 1, 2);	//坐标顺序
		btVector3 connectionPointCS[4] = { btVector3(-0.6f, 0.0f, 0.7f),btVector3(0.6f, 0.0f, 0.7f), btVector3(-0.6f, 0.0f,-0.7f), btVector3(0.6f, 0.0f, -0.7f) };	//轮胎位置
		for (int i = 0; i < 4; i++)
		{
			btVector3 wheelDirectionCS0 = btVector3(0.0f, -1.0f, 0.0f);	//轮胎方向
			btVector3 wheelAxleCS = btVector3(0.0f, 0.0f, -1.0f);	//轮胎轴
			btScalar suspensionRestLength = 0.6f;	//悬架长度
			btScalar wheelRadius = 0.5f;	//轮胎半径
			RaycastModel->addWheel(connectionPointCS[i], wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, tuning, true);
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
	void Steering(double control)
	{
		btScalar steering_new = RaycastModel->getSteeringValue(0) + 1 * control;
		if (fabs(control)< 1e-6)
			steering_new = 0;
		for (int i = 0; i < RaycastModel->getNumWheels(); i++)
			RaycastModel->setSteeringValue(steering_new, i);
	}
	void updateTransform(double deltaTime)
	{
		//RaycastModel->updateVehicle(deltaTime);
		for (int i = 0; i < RaycastModel->getNumWheels(); i++)
		{
			RaycastModel->updateWheelTransform(i, true);
		}
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

	glm::mat4 getTransform()
	{
		//chassis->getMotionState()->getWorldTransform(modelTransform);
		modelTransform = RaycastModel->getChassisWorldTransform();
		//std::cout << RaycastModel->getCurrentSpeedKmHour() << std::endl;
		//std::cout << modelTransform.getOrigin().getX() << " " << modelTransform.getOrigin().getY() << " " << modelTransform.getOrigin().getZ() << std::endl;
		glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(modelTransform.getOrigin().getX(), modelTransform.getOrigin().getY(), modelTransform.getOrigin().getZ()));
		glm::mat4 rotation = glm::mat4(glm::quat(modelTransform.getRotation().getW(), modelTransform.getRotation().getX(), modelTransform.getRotation().getY(), modelTransform.getRotation().getZ()));
		glm::mat4 worldTransform = translation * rotation;
		return worldTransform;
	}
};

#endif