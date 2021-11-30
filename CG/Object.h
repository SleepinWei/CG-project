#ifndef OBJECT_H
#define OBJECT_H

#include "Model.h"
#include<bullet/btBulletCollisionCommon.h>
#include<bullet/btBulletDynamicsCommon.h>
#include<glm/glm.hpp>

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

class Object{
private:
	Model* objectModel;
	PhysicsModel* physicsModel;

	glm::mat4 model;//model transformation 
public:
	GLuint id; //id for every object 
	static GLuint idCounter;

	Object(std::string const& path, bool gamma = false) {
		objectModel = new Model(path, gamma);
		physicsModel = new PhysicsModel;
		id = idCounter++;
		model = glm::mat4(1.0f);
	}
	virtual ~Object() {
		delete objectModel;
		delete physicsModel;
	}
	void renderObject(Shader& shader) {
		shader.use();
		shader.setMat4("model", model);
		objectModel->Draw(shader);
	}
	void stepSimulation(){
		//physics world simulation herer
		// convert bullet tranformations into glm:: matrices
	}
};
GLuint Object::idCounter = 0;

#endif // !OBJECT_H
