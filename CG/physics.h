#ifndef PHYSICS_H
#define PHYSICS_H

#include <bullet/btBulletDynamicsCommon.h>


class Physics
{
private:
public:
	PhysicsManager* phyManager;
	Physics()
	{
		phyManager = new PhysicsManager();
	}
	void ground()
	{
		btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(200.), btScalar(3.), btScalar(200.)));

		phyManager->collisionShapes.push_back(groundShape);

		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(0, -3, 0));

		btScalar mass(0.);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			groundShape->calculateLocalInertia(mass, localInertia);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		//add the body to the dynamics world
		phyManager->dynamicsWorld->addRigidBody(body);
	}
	void setCube(float x, float y, float w)
	{
		// btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
		btCollisionShape* colShape = new btSphereShape(btScalar(1.));
		
		phyManager->collisionShapes.push_back(colShape);

		/// 创建动态对象
		btTransform startTransform;
		startTransform.setIdentity();

		btScalar mass(w);

		// 刚体是动态的如果且仅当质量为非零时，否则是静止的
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			colShape->calculateLocalInertia(mass, localInertia);

		startTransform.setOrigin(btVector3(x, 1, y));

		// 推荐使用motionstate，它提供插值功能，只同步“活动”对象
		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		phyManager->dynamicsWorld->addRigidBody(body);
	}
};

#endif