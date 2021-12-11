#ifndef OBJECT_H
#define OBJECT_H

#include "Mesh.h"
#include<bullet/btBulletCollisionCommon.h>
#include<bullet/btBulletDynamicsCommon.h>
#include<glm/glm.hpp>
#include<glm/ext.hpp>

class PhysicsModel {
	//all kinds of mysterious things in bullet 

public:
	//some attributes here 
	btScalar mass;
	bool isDynamic;
	btVector3 localInertia;
	//info necessary for construct info: including mass,isDynamic etc. 
	btRigidBody::btRigidBodyConstructionInfo* modelInfo;

	btTransform modelTransform; 
	btDiscreteDynamicsWorld* dynamicsWorld;
	btRigidBody* body;
	btDefaultMotionState* motionState;
	btCollisionShape* collisionShape;

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
	void registerRigidBody(Mesh* mesh) {
		collisionShape = new btBvhTriangleMeshShape(mesh->meshInterface, false, true);
		motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
		btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, motionState, collisionShape, btVector3(0, 0, 0));
		body = new btRigidBody(groundRigidBodyCI);
		body->setContactProcessingThreshold(0.f);

		dynamicsWorld->addRigidBody(body);
	}
};

class Object{
private:
	Mesh* mesh;
	PhysicsModel* physicsModel;

	glm::mat4 model;//model transformation 
public:
	GLuint id; //id for every object 
	static GLuint idCounter;

	Object(Mesh mesh_from_object, btDiscreteDynamicsWorld* dynamicsWorld)
	{
		mesh = new Mesh(mesh_from_object);
		physicsModel = new PhysicsModel;
		physicsModel->dynamicsWorld = dynamicsWorld;

		id = idCounter++;
		model = glm::mat4(1.0f);

		physicsModel->registerRigidBody(mesh);
	}
	virtual ~Object() {
		delete mesh;
		delete physicsModel;
	}
	void renderObject(Shader& shader) {
		shader.use();
		shader.setMat4("model", model);
		//objectModel->Draw(shader);
	}
	void stepSimulation(){
		//physics world simulation herer
		// convert bullet tranformations into glm:: matrices
	}

	glm::mat4 getModelMatrix()
	{
		btTransform transform = getTransform();
		glm::mat4 locationMatrix = glm::translate(
			glm::mat4(1.0f),
			glm::vec3(transform.getOrigin().getX(), transform.getOrigin().getY(), transform.getOrigin().getZ())
		);

		glm::mat4 rotationMatrix = glm::toMat4(
			glm::quat(
				transform.getRotation().getW(),
				transform.getRotation().getX(),
				transform.getRotation().getY(),
				transform.getRotation().getZ()
			)
		);

		return locationMatrix * rotationMatrix;
	}

	btTransform getTransform()
	{
		return physicsModel->modelTransform;
	}

	virtual void updateTransform()
	{
		btTransform btTransform;
		physicsModel->body->getMotionState()->getWorldTransform(btTransform);
		physicsModel->modelTransform = btTransform;
	}

	virtual void updateTransform(const btTransform& btTransform)
	{
		physicsModel->modelTransform = btTransform;
	}
};
GLuint Object::idCounter = 0;

#endif // !OBJECT_H
