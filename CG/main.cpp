
#include<glad/glad.h>
#include<iostream>
#include<glfw/glfw3.h>
#include<glm/glm.hpp>
//#include<filesystem>

#include"Mesh.h"
#include"Model.h"
#include"Camera.h"
#include<shader/Shader.h>

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

void renderQuad();
void renderCube();
void renderScene(const Shader& shader);

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

//lighting 
//glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
//glm::vec3 lightPos(1.0 * 10, 1.0 * 10, 1.0 * 10);

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
	//Shader lightingShader("../shader/multiple_cube.vs", "../shader/multiple_cube.fs");
	//Shader lightCubeShader("../shader/light_cube.vs", "../shader/light_cube.fs");
    //Shader shader("../shader/shadow_mapping.vs", "../shader/shadow_mapping.fs");
    Shader simpleDepthShader("../shader/shadow_mapping_depth.vs", "../shader/shadow_mapping_depth.fs");
    Shader debugDepthQuad("../shader/debug_quad.vs", "../shader/debug_quad_depth.fs");
    //Model ourModel("../resources/objects/Mercedes_Benz/Mercedes_Benz.obj");
    Shader pcssShader("../shader/shadow_mapping.vs", "../shader/pcss.fs");
    Shader lightShader("../shader/light.vs", "../shader/light.fs");
	//settings 
    float planeVertices[] = {
        // positions            // normals         // texcoords
         25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
        -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

         25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
         25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
    };
    // plane VAO
    unsigned int planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glBindVertexArray(0);

    // load textures
    // -------------
    unsigned int woodTexture = loadTexture("../resources/textures/wood.png");

    // configure depth map FBO
    // -----------------------
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    // create depth texture
    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    
    //set border color 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    GLfloat borderColor[] = { 1.0,1.0,1.0,1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    // shader configuration
    // --------------------
    pcssShader.use();
    pcssShader.setInt("diffuseTexture", 0);
    pcssShader.setInt("shadowMap", 1);
    debugDepthQuad.use();
    debugDepthQuad.setInt("depthMap", 0);

    // lighting info
    // -------------
    float n =20.f;
    glm::vec3 lightPos(-2.0f * n + 10, 4.0f * n, -1.0f*n);
#endif

	//render loop
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

        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        float near_plane = 5.f, far_plane = 50.f;
 
        //lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        float fovy = glm::radians(45.0f);
        float aspect = (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT;
        lightProjection = glm::perspective(fovy,aspect, near_plane, far_plane);
        lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;
        
        simpleDepthShader.use();
        simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        // render scene from light's point of view

        //¿ªÆôÕýÃæÌÞ³ý
        glCullFace(GL_FRONT);
        
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, woodTexture);
        //renderScene(simpleDepthShader);
        renderScene(simpleDepthShader);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        //reset face culling 
        glCullFace(GL_BACK);

        // 2. render scene as normal using the generated depth/shadow map  
        // --------------------------------------------------------------
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        //shader.setMat4("projection", projection);
        //shader.setMat4("view", view);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
        model = glm::translate(model, lightPos);
        lightShader.use();
        lightShader.setMat4("model", model);
        lightShader.setMat4("projection", projection);
        lightShader.setMat4("view", view);
        renderCube();

        pcssShader.use();
        pcssShader.setMat4("lightView", lightView);

        pcssShader.setMat4("projection", projection);
        pcssShader.setMat4("view", view);

        // set light uniforms
        pcssShader.setVec3("viewPos", camera.Position);
        pcssShader.setVec3("lightPos", lightPos);
        pcssShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, woodTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);

        glm::vec2 lightSize = glm::vec2(5.f, 5.f);
        pcssShader.setVec2("lightSize", lightSize);
        glm::vec2 planeSize = glm::vec2(2 * near_plane * glm::tan(fovy / 2) * aspect, 2 * near_plane * glm::tan(fovy / 2));
        pcssShader.setVec2("planeSize", planeSize);
        pcssShader.setFloat("zNear", near_plane);

        //float zNear = 10.0f;
        //pcssShader.setFloat("zNear", 0.01f);
        renderScene(pcssShader);
        //pcss Test

        // models 
        //ourModel.Draw(shader);
        //

        // render Depth map to quad for visual debugging
        // ---------------------------------------------
        debugDepthQuad.use();
        debugDepthQuad.setFloat("near_plane", near_plane);
        debugDepthQuad.setFloat("far_plane", far_plane);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        //renderQuad();
#endif

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

#ifdef TEST_OBJ

#endif