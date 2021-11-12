#include<glad/glad.h>
#include<iostream>
#include<glfw/glfw3.h>
#include<glm/glm.hpp>

//#include<stb/stb_image.h>
#include"Mesh.h"
#include"Model.h"
#include"Camera.h"
#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"

int createWindow(GLFWwindow*& window, 
	int width, 
	int height,
	std::string title="Car Game") {
	//create a glfw window 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//create window
	window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	return 0;
}

int gladInit() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize" << std::endl;
		return -1;
	}
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	//viewport control
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main() {
	glfwInit();

	//create a window
	GLFWwindow* window;
	createWindow(window, 800, 600);
	glfwMakeContextCurrent(window);
	
	//set Viewport
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//init glad 
	gladInit();

	//render loop
	while(!glfwWindowShouldClose(window))
	{
		processInput(window);

		//1. Camera Movement

		//2. Objects Movement
		
		//3. Rendering 


		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}