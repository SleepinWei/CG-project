#ifndef MANAGER_H
#define MANAGER_H
/******************************
* This file contains various managers 
* @class RenderManager: controls rendering process(basically the whole world is controlled by it
* @class Physics Manager: if necessary.  
* @class World Manager: if Physics manager is applied, then World Manger is applied and 
	controls both rendering and physics simulation
*******************************/

/********************************
* notifications: 
********************************/


#include"Object.h"
#include"Camera.h"
#include"Car.h"
#include<vector>
#include<glfw/glfw3.h>
#include"SkyBox.h"
#include<bullet/btBulletDynamicsCommon.h>
#include<bullet/btBulletCollisionCommon.h>

class RenderManager {
private:
	/*private Functions*/
	void initCamera() {
		lastX = SCR_WIDTH / 2.0f;
		lastY = SCR_HEIGHT / 2.0f;
	}
	void initSkyBox() {
		skyBox = new SkyBox();
	}

public:
	/*attributes*/
	//basic settings 
	static const unsigned int SCR_WIDTH = 1600;
	static const unsigned int SCR_HEIGHT = 1200;
	bool enableMouse;

	std::vector<Object*> objects; 

	SkyBox* skyBox;

	//cameras
	Camera* camera;
	float lastX;
	float lastY;
	
	//time
	float deltaTime;
	float lastFrame;
	//terrain engine 


	/*public functions*/
	void initWorld() {
		enableMouse = true;
		deltaTime = 0.0f;
		lastFrame = 0.0f;
		initCamera();
		initSkyBox();
	}
	void renderScene() {
		for (auto p : objects) {
			;
		}
	}


	void addObject(Object* object) {
		//add objects here 
		objects.push_back(object);
	}

};

class PhysicsManager {
	/***************************
	* manager of physics world 
	****************************/
public:
	/*settings*/
	btAlignedObjectArray<btCollisionShape*> collisionShapes;
	btBroadphaseInterface* broadPhase;
	btCollisionDispatcher* dispatcher;
	btConstraintSolver* solver;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btDiscreteDynamicsWorld* dynamicsWorld;
	
	/*constants*/
	btVector3 gravity;
	btScalar deltaTime;

	PhysicsManager() {
		collisionConfiguration = new btDefaultCollisionConfiguration();
		dispatcher = new btCollisionDispatcher(collisionConfiguration);
		broadPhase = new btDbvtBroadphase();
		btSequentialImpulseConstraintSolver* sol = new btSequentialImpulseConstraintSolver;
		solver = sol;

		dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadPhase, solver, collisionConfiguration);
		
		gravity = btVector3(0, -9.8, 0);
		deltaTime = 0.1f;
		dynamicsWorld->setGravity(gravity);
	}
	~PhysicsManager() {
		delete collisionConfiguration;
		delete dispatcher;
		delete solver;
		delete dynamicsWorld;
		
		for (int i = 0; i < collisionShapes.size(); i++) {
			delete collisionShapes[i];
		}
	}
	void stepSimulation(float deltaTime) {
		if (dynamicsWorld) {
			dynamicsWorld->stepSimulation(deltaTime);
		}
	}
	void stepSimulation() {
		//use default deltaTime for simulation
		stepSimulation(this->deltaTime);
	}

	void physicsDebugDraw(int debugFlags) {
		if (dynamicsWorld && dynamicsWorld->getDebugDrawer()) {
			dynamicsWorld->getDebugDrawer()->setDebugMode(debugFlags);
			dynamicsWorld->debugDrawWorld();
		}
	}
	void debugDraw(int debugDrawFlags) {
		if (dynamicsWorld) {
			if (dynamicsWorld->getDebugDrawer()) {
				dynamicsWorld->getDebugDrawer()->setDebugMode(debugDrawFlags);
			}
			dynamicsWorld->debugDrawWorld();
		}
	}
	void registerCollisionObject(btCollisionShape* collisionShape) {
		collisionShapes.push_back(collisionShape);
	}
};
#endif // !MANAGER_H
