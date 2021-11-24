#ifndef MANAGER_H
#define MANAGER_H
#include"Object.h"
#include"Camera.h"
#include"Car.h"
#include<vector>
#include<glfw/glfw3.h>


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


	/********************************************************
	Function name: keyDown(GLFWwindow*, Camera&, Car&, float)
	Input: window  camera  car  deltatime
	Output: void
	Operation: from the input of keyboard, control camera and car
	********************************************************/
	void keyDown(GLFWwindow* window, Camera& cam, Car& car, float deltatime) {
		//esc
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		//W A S D control the camera
		float deltaTime = 1.0;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			cam.ProcessKeyboard(FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			cam.ProcessKeyboard(BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			cam.ProcessKeyboard(LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			cam.ProcessKeyboard(RIGHT, deltaTime);

		//UP DOWN LEFT RIGHT control the car
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			car.ProcessKeyboard(CAR_FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			car.ProcessKeyboard(CAR_BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && car.Speed > 0)
			car.ProcessKeyboard(CAR_LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && car.Speed > 0)
			car.ProcessKeyboard(CAR_RIGHT, deltaTime);
	}

	/********************************************************
	Function name: mouse_move(GLFWwindow*, float, float)
	Input: window  pre_x  pre_y
	Output: void
	Operation: from the mouse input, control the camera
	********************************************************/
	void mouse_move(GLFWwindow* window, float pre_x, float pre_y) {
		
	}
};

#endif // !MANAGER_H
