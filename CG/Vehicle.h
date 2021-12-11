#ifndef VEHICLE_H
#define VEHICLE_H

#include "Object.h"
#include <bullet/BulletDynamics/Vehicle/btRaycastVehicle.h>
//#include <bullet/BulletDynamics/Vehicle/btVehicleRaycaster.h>

class VehiclePhysicsModel : public PhysicsModel
{
private:
	btRigidBody* chassis;
	btVehicleRaycaster* Raycaster;
	btRaycastVehicle* RaycastModel;
public:
	btTransform WheelTransform[4];
	btTransform ChassisTransform;

	VehiclePhysicsModel(btRigidBody* ChassisBody)
	{
		RaycastModel->setCoordinateSystem(0, 1, 2);	//坐标顺序
		chassis = ChassisBody;
		btRaycastVehicle::btVehicleTuning tuning;	//计算相关参数
		RaycastModel = new btRaycastVehicle(tuning, chassis, Raycaster);
		btVector3 connectionPointCS[4] = { btVector3(-0.6f, 0.0f, 0.7f),btVector3(0.6f, 0.0f, 0.7f), btVector3(-0.6f, 0.0f, 0.7f), btVector3(0.6f, 0.0f, 0.7f) };	//轮胎位置
		for (int i = 0; i < 4; i++)
		{
			btVector3 wheelDirectionCS0 = btVector3(0.0f, -1.0f, 0.0f);	//轮胎方向
			btVector3 wheelAxleCS = btVector3(-1.0f, 0.0f, 0.0f);	//轮胎轴
			btScalar suspensionRestLength = 0.1f;	//悬架长度
			btScalar wheelRadius = 0.4f;	//轮胎半径
			RaycastModel->addWheel(connectionPointCS[i], wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, tuning, true);
		}

		dynamicsWorld->addRigidBody(chassis);
	}
	~VehiclePhysicsModel()
	{
		delete Raycaster;
		delete RaycastModel;
	}
	void Speedup()
	{
		for (int i = 0; i < RaycastModel->getNumWheels(); i++)
			RaycastModel->applyEngineForce(10, i);
	}
	void Brake()
	{
		for (int i = 0; i < RaycastModel->getNumWheels(); i++)
			RaycastModel->applyEngineForce(-10, i);
	}
	void Left()
	{
		btScalar steering = RaycastModel->getSteeringValue(0);
		for (int i = 0; i < RaycastModel->getNumWheels(); i++)
			RaycastModel->setSteeringValue(steering + 1, i);
	}
	void Right()
	{
		btScalar steering = RaycastModel->getSteeringValue(0);
		for (int i = 0; i < RaycastModel->getNumWheels(); i++)
			RaycastModel->setSteeringValue(steering - 1, i);
	}
};
class Vehicle : public Object
{
private:

	//未加轮胎
public:
	Vehicle(Mesh mesh_from_object, btDiscreteDynamicsWorld* dynamicsWorld) : Object(mesh_from_object, dynamicsWorld)
	{

	}
	~Vehicle()
	{

	}
};


#endif