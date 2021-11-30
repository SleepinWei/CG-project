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
* TEXTURE_0 is for skybox 
********************************/


#include"Object.h"
#include"Camera.h"
#include"Car.h"
#include<vector>
#include<glfw/glfw3.h>
#include"SkyBox.h"

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
