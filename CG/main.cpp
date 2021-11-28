
#include<glad/glad.h>
#include<iostream>
#include<glfw/glfw3.h>
#include<glm/glm.hpp>
//#include<filesystem>

#include"Mesh.h"
#include"Model.h"
#include"Camera.h"
#include<shader/Shader.h>
#include"SkyBox.h"

//#define STB_IMAGE_IMPLEMENTATION
//#include"stb_image.h"

//imgui 
#include"../include/imgui/imgui_impl_glfw.h"
#include"../include/imgui/imgui.h"
#include"../include/imgui/imgui_impl_opengl3.h"

#include"Utils.h"

int initPhysics() {
	//use functions or define a class? 
	//initialize a dynamics world 
	//How to design? 
    return 0;
}

class PhysicsManager {
	void initPhysics();
	void stepSimulation();
};

#define TEST_OBJ
#ifdef TEST_OBJ
// settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 1200;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//plane 
unsigned int planeVAO;
#endif

int main() {
	glfwInit();

	//create a window
	GLFWwindow* window;
	createWindow(window, SCR_WIDTH,SCR_HEIGHT);
	glfwMakeContextCurrent(window);
	
	//set Viewport
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//init glad 
	gladInit();
    glEnable(GL_DEPTH_TEST);

    //init GUI 
    imGuiInit(window);

#ifdef TEST_OBJ
	Shader skyShader("../shader/sky_box.vs", "../shader/sky_box.fs");
    
    SkyBox* skybox = new SkyBox();
    skybox->setUp();
	//settings 

    // configure depth map FBO
    // -----------------------

    // shader configuration
    // --------------------
    
    float n =20.f;
    glm::vec3 lightPos(-2.0f * n + 10, 4.0f * n, -1.0f*n);
#endif

	while(!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		//1. Camera Movement

		//2. Objects Movement
		
		//3. Rendering 

	
#ifdef TEST_OBJ
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		glDepthFunc(GL_LEQUAL);
		skyShader.use();
		glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));//remove translation 
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		skyShader.setMat4("view", view);
		skyShader.setMat4("projection", projection);
		glBindVertexArray(skybox->VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->textureID);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);
        
#endif
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

#ifdef TEST_OBJ

#endif