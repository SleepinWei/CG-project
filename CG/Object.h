#ifndef OBJECT_H
#define OBJECT_H

#include "Model.h"
#include<bullet/btBulletCollisionCommon.h>
#include<bullet/btBulletDynamicsCommon.h>
class PhysicsModel {
	//all kinds of mysterious things in bullet 

	//some attributes here 
	btScalar mass;
	bool isDynamic;
	btVector3 localInertia;
	//info necessary for construct info: including mass,isDynamic etc. 
	btRigidBody::btRigidBodyConstructionInfo* modelInfo;
	btRigidBody* body; 

	btTransform modelTransfrom; 
	//setIdentity() 
	//setOrigin() 
	void setMass(btScalar mass) {
		mass = mass;
	}
	void setDynamic(bool dynamic) {
		isDynamic = dynamic;
	}
	void setLocalInertia(btVector3 inertia) {
		localInertia = inertia;
	}
	void registerRigidBody() {

	}
};


class Object:public PhysicsModel,public Model {
public:
	GLuint id; //id for every object 
	static GLuint idCounter;

	void init() {
		//set initial state
		idCounter = 0;
	}

	void render() {
		Model::render();
	}

	void createId() {
		//bind a id to the Object 
		id = idCounter++;
	}
};

#endif // !OBJECT_H
