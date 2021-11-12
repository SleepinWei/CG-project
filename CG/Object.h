#ifndef OBJECT_H
#define OBJECT_H

#include "Model.h"

class PhysicsModel {
	//bind bullet shapes here 
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
