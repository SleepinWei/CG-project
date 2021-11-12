#ifndef MANAGER_H
#define MANAGER_H
#include"Object.h"
#include<vector>

class RenderManager {
	std::vector<Object*> objects; 

	void render() {
		for (auto p : objects) {
			p->render();
		}
	}

	void addObject(Object* object) {
		//add objects here 
		objects.push_back(object);
	}
};

#endif // !MANAGER_H
