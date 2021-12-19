#define _CRT_SECURE_NO_WARNINGS
#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include<filesystem>
#include<vector>
#include "Model.h"
//#include "shader.h"

#include <shader/Shader.h>

#include <iostream>
using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);
unsigned int loadCubemap(vector<std::string> faces);


//void  drawCar(Model& model, Shader& shader, unsigned int diffuseMapStreetLight, unsigned int normalMapStreetLight);
void drawCar(Model& model, Shader& shader);
void drawRaceTrack(Model& model, Shader& shader);

void drawStreetLights1(Model& model, Shader& shader,unsigned int,unsigned int);
void drawLimitSign(Model& model, Shader& shader, unsigned int, unsigned int);

void drawStopSign1(Model& model, Shader& shader);
void drawStreetLights2(Model& model, Shader& shader);
void drawTrafficSign(Model& model, Shader& shader);
void drawHouse(Model& model,Model& , Model& wood,Shader& shader);
void drawCone(Model& model, Shader& shader);
void drawPlane(Model& model,Model& model2, Shader& shader);



// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

glm::vec3 cameraPos(0.0f, 10.0f, 10.0f);
Camera camera(cameraPos);


float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);



	/*********************************     skybox单独放       ************************************/

	Shader skyboxShader("../resources/sceneResources/src/4.advanced_opengl/6.2.cubemaps_environment_mapping/6.2.skybox.vs", "../docs/src/4.advanced_opengl/6.2.cubemaps_environment_mapping/6.2.skybox.fs");
	// set up vertex data (and buffer(s)) and configure vertex attributes
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	// skybox VAO
	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// load textures
	// -------------
	vector<std::string> faces
	{
		("../resources/textures/skybox/right.jpg"),
		("../resources/textures/skybox/left.jpg"),
		("../resources/textures/skybox/top.jpg"),
		("../resources/textures/skybox/bottom.jpg"),
		("../resources/textures/skybox/front.jpg"),
		("../resources/textures/skybox/back.jpg"),
	};
	unsigned int cubemapTexture = loadCubemap(faces);

	// shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);
	/*********************************     skybox完       ************************************/


	/*********************************     各个model       ************************************/
	Model carModel(("../resources/objects/Avent_sport/Avent_sport.obj"));
	Model raceTrackModel(("../resources/sceneResources/race-track/race-track.obj"));

	//这两个用 normalShader 法线贴图
	Model streetLight1(("../resources/sceneResources/StreetLamp/light/Street_Lamp_obj.obj"));
	//Model streetLight2(("../resources/sceneResources/StreetLamp/light/Street_Lamp_obj.obj"));
	Model limitSignModel("../resources/sceneResources/StreetLamp/limitsign/10566_Speed Limit Sign (70 MPH)_v2-L3.obj");

	//以下用modelShader
	Model cottage1("../resources/sceneResources/house/cottage1/medieval house.obj");
	Model cottage2("../resources/sceneResources/house/cottage2/Snow covered CottageOBJ.obj");	
	Model cone("../resources/sceneResources/roadcone/cone2_obj.obj");
	Model wood("../resources/sceneResources/wood/trunk wood.obj");
	Model plane("../resources/sceneResources/plane/Crash_plane_brothel.obj");
	Model brokenCar("../resources/sceneResources/brokencar/ballarat-car.obj");
	Model stopSignModel("../resources/sceneResources/StreetLamp/stopsign/StopSign.obj");
	Model trafficSignModel("../resources/sceneResources/StreetLamp/trafficsign/Traffic_Sign.obj");

	/*********************************     model 完      ************************************/


	/*********************************     各个shader       ************************************/

	Shader shader("../resources/sceneResources/shader/light_and_shadow.vs", "../resources/sceneResources/shader/light_and_shadow.fs");
	shader.use();
	shader.setInt("diffuseTexture", 0);
	shader.setInt("shadowMap", 15); // 这里的15是指"GL_TEXTURE15"，需要与后面的对应

	// shader configuration
	// --------------------
	// build and compile shaders
	Shader envirShader("../resources/sceneResources/src/4.advanced_opengl/6.2.cubemaps_environment_mapping/6.2.cubemaps.vs", "../resources/sceneResources/src/4.advanced_opengl/6.2.cubemaps_environment_mapping/6.2.cubemaps.fs");
	// load models
	envirShader.use();
	envirShader.setInt("skybox", 0);

	Shader normalShader("../resources/sceneResources/shader/normal_mapping.vs", "../resources/sceneResources/shader/normal_mapping.fs");
	normalShader.use();


	unsigned int diffuseMapStreetLight = loadTexture((char*)("../resources/sceneResources/StreetLamp/light/lamba_DefaultMaterial_BaseColor.png"));
	unsigned int normalMapStreetLight = loadTexture((char*)("../resources/sceneResources/StreetLamp/light/lamba_DefaultMaterial_Normal.png"));

	
	unsigned int diffuseMapLimitSign = loadTexture((char*)("../resources/sceneResources/StreetLamp/limitSign/Speed Limit Sign (70 MPH).jpg"));
	unsigned int normalMapLimitSign = loadTexture((char*)("../resources/sceneResources/StreetLamp/limitSign/NormalMap.png"));

	/*unsigned int diffuseCar = loadTexture((char*)("../resources/objects/Avent_sport/diffuse.jpg"));
	unsigned int normalCar = loadTexture((char*)("../resources/objects/Avent_sport/NormalMap.png"));*/




	Shader modelShader("../resources/sceneResources/shader/normal_mapping.vs", "../resources/sceneResources/shader/normal_mapping.fs");
	modelShader.use();


	/*********************************     shader完       ************************************/



	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);




		drawCar(carModel,modelShader);
		drawRaceTrack(raceTrackModel, shader);
		drawStreetLights1(streetLight1, normalShader, diffuseMapStreetLight, normalMapStreetLight);
		drawLimitSign(limitSignModel, normalShader, diffuseMapLimitSign, normalMapLimitSign);

		drawStopSign1(stopSignModel, modelShader);
		drawTrafficSign(trafficSignModel, modelShader);
		drawHouse(cottage1, cottage2, wood, modelShader);
		drawCone(cone, modelShader);
		drawPlane(plane, brokenCar, modelShader);



		// draw skybox as last
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		skyboxShader.use();

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera.GetViewMatrix();
		//glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4  projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		// draw scene as normal

		view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
		skyboxShader.setMat4("view", view);
		skyboxShader.setMat4("projection", projection);
		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // set depth function back to default

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVAO);

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front) 
// -Z (back)
// -------------------------------------------------------
unsigned int loadCubemap(vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrComponents;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

void drawCar(Model& model, Shader& shader)
//void  drawCar(Model& model, Shader& shader,unsigned int diffuseMapStreetLight,unsigned int normalMapStreetLight)
{

	glm::mat4 model_0 = glm::mat4(1.0f);
	model_0 = glm::translate(model_0, glm::vec3(0.0f, 1.5f, 0.0f)); // translate it down so it's at the center of the scene
	//model_0 = glm::rotate(model_0, glm::radians(-90.0f),glm::vec3(1.0f, .0f, 0.0f)); // translate it down so it's at the center of the scene
	model_0 = glm::scale(model_0, glm::vec3(1.0f));	// it's a bit too big for our scene, so scale it down
	
	glm::mat4 view = camera.GetViewMatrix();
	//glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4  projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	
	shader.use();

	shader.setMat4("model", model_0);
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);
	shader.setVec3("cameraPos", camera.Position);

    

	model.Draw(shader);

}

void drawRaceTrack(Model& model, Shader& shader)
{


	glm::mat4 _model = glm::mat4(1.0f);
	_model = glm::translate(_model, glm::vec3(0.0f, -0.16f, 0.0f));
	_model = glm::scale(_model, glm::vec3(3.0f));

	glm::mat4 view = camera.GetViewMatrix();
	//glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4  projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	// draw scene as normal
	shader.use();
	shader.setMat4("model", _model);
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);
	shader.setVec3("cameraPos", camera.Position);


	model.Draw(shader);
}

void drawStreetLights1(Model& model, Shader& shader, unsigned int diffuseMapStreetLight, unsigned int normalMapStreetLight)
{
	glm::mat4 _model = glm::mat4(1.0f);
	//_model = glm::translate(_model, glm::vec3(-100.0f, .0f, 50.0f));

	_model = glm::translate(_model, glm::vec3(-70.0f, 2.0f, 0.0f));
	_model = glm::rotate(_model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	_model = glm::scale(_model, glm::vec3(6.0f));


	glm::mat4 view = camera.GetViewMatrix();
	//glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	// draw scene as normal




	shader.use();
	shader.setInt("diffuseMap", 14);
	shader.setInt("normalMap", 13);

	glActiveTexture(GL_TEXTURE14);
	glBindTexture(GL_TEXTURE_2D, diffuseMapStreetLight);
	glActiveTexture(GL_TEXTURE13);
	glBindTexture(GL_TEXTURE_2D, normalMapStreetLight);

	shader.setMat4("model", _model);
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);
	shader.setVec3("cameraPos", camera.Position);

	model.Draw(shader);



	_model = glm::mat4(1.0f);
	_model = glm::translate(_model, glm::vec3(+155.0f, 2.0f, -3.0f));
	_model = glm::rotate(_model, glm::radians(120.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	_model = glm::scale(_model, glm::vec3(6.0f));


	view = camera.GetViewMatrix();
	//glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
     projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	// draw scene as normal

	shader.use();

	shader.setMat4("model", _model);
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);
	shader.setVec3("cameraPos", camera.Position);

	model.Draw(shader);


	//画第一条道路的内圈
	/*for (int i = 0; i <= 30; ++i)
	{
		if (i < 2)
			_model = glm::translate(_model, glm::vec3(10.0f, 0.0f, -21.0f));
		else if (i == 2)
			_model = glm::translate(_model, glm::vec3(10.0f, 0.0f, -11.0f));
		else
			_model = glm::translate(_model, glm::vec3(10.0f, 0.0f, -10.7f));
			



		shader.use();
		shader.setMat4("model", _model);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
		shader.setVec3("cameraPos", camera.Position);

		model.Draw(shader);
	}

	glm::mat4 model2 = _model;*/
	
}

void drawStreetLights2(Model& model, Shader& shader)
{
	glm::mat4 _model = glm::mat4(1.0f);
	//_model = glm::translate(_model, glm::vec3(0.0f,0.1f,0.0f));


	_model = glm::translate(_model, glm::vec3(10.0, 0.0f, 0.0f));
	_model = glm::rotate(_model, glm::radians(-120.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 view = camera.GetViewMatrix();
	//glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4  projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	// draw scene as normal
	shader.use();
	shader.setMat4("model", _model);
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);
	shader.setVec3("cameraPos", camera.Position);

	model.Draw(shader);
}


void drawStopSign1(Model& model, Shader& shader)
{
	glm::mat4 _model = glm::mat4(1.0f);
	//_model = glm::translate(_model, glm::vec3(0.0f,0.1f,0.0f));


	_model = glm::translate(_model, glm::vec3(20.0, 7.0f, 0.0f));
	_model = glm::rotate(_model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	_model = glm::scale(_model, glm::vec3(3.0f));

	glm::mat4 view = camera.GetViewMatrix();
	//glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4  projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	// draw scene as normal
	shader.use();
	shader.setMat4("model", _model);
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);
	shader.setVec3("cameraPos", camera.Position);

	model.Draw(shader);


	//画第一条道路的内圈

}



void drawTrafficSign(Model& model, Shader& shader)
{
	glm::mat4 _model = glm::mat4(1.0f);
	//_model = glm::translate(_model, glm::vec3(0.0f,0.1f,0.0f));

	_model = glm::translate(_model, glm::vec3(80.0, 3.0f, 60.0f));
	_model = glm::rotate(_model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	_model = glm::scale(_model, glm::vec3(1.0f));

	glm::mat4 view = camera.GetViewMatrix();
	//glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4  projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	// draw scene as normal
	shader.use();
	shader.setMat4("model", _model);
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);
	shader.setVec3("cameraPos", camera.Position);

	model.Draw(shader);



	for (int i = 0; i < 4; ++i)
	{
		_model = glm::mat4(1.0f);
		//_model = glm::translate(_model, glm::vec3(0.0f,0.1f,0.0f));

		if (i == 0)
			_model = glm::translate(_model, glm::vec3(-200.0, 8.1f, 330.0f));
		if (i == 1)
			_model = glm::translate(_model, glm::vec3(+200.0, 8.1f, 300.0f));
		if (i == 2)
			_model = glm::translate(_model, glm::vec3(-270.0, 8.1f, +350.0f));
		if (i == 3)
			_model = glm::translate(_model, glm::vec3(-270.0, 8.1f, -350.0f));


		_model = glm::rotate(_model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		_model = glm::scale(_model, glm::vec3(1.0f));


		// draw scene as normal
		shader.use();
		shader.setMat4("model", _model);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
		shader.setVec3("cameraPos", camera.Position);

		model.Draw(shader);
	}


	//画第一条道路的内圈

}


void drawLimitSign(Model& model, Shader& shader,unsigned int diffuseMapLimitSign, unsigned int  normalMapLimitSign)
{
	glm::mat4 _model = glm::mat4(1.0f);
	//_model = glm::translate(_model, glm::vec3(0.0f,0.1f,0.0f));

	_model = glm::translate(_model, glm::vec3(+390.0, 2.1f, -350.0f));
	_model = glm::rotate(_model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	_model = glm::scale(_model, glm::vec3(0.15f));

	glm::mat4 view = camera.GetViewMatrix();
	//glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4  projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	// draw scene as normal
	shader.use();
	glActiveTexture(GL_TEXTURE12);
	glBindTexture(GL_TEXTURE_2D, diffuseMapLimitSign);
	glActiveTexture(GL_TEXTURE11);
	glBindTexture(GL_TEXTURE_2D, normalMapLimitSign);
	shader.setInt("diffuseMap", 12);
	shader.setInt("normalMap", 11);


	shader.setMat4("model", _model);
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);
	shader.setVec3("cameraPos", camera.Position);

	model.Draw(shader);

	for (int i = 0; i < 3; ++i)
	{
		_model = glm::mat4(1.0f);
		//_model = glm::translate(_model, glm::vec3(0.0f,0.1f,0.0f));

		if (i == 0)
			_model = glm::translate(_model, glm::vec3(-200.0, 2.1f, 0.0f));
		if (i == 1)
			_model = glm::translate(_model, glm::vec3(+200.0, 2.1f, 0.0f));
		if (i == 2)
			_model = glm::translate(_model, glm::vec3(-370.0, 2.1f, +350.0f));

	
		_model = glm::rotate(_model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//_model = glm::rotate(_model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		_model = glm::scale(_model, glm::vec3(0.15f));

		shader.use();
		shader.setMat4("model", _model);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
		shader.setVec3("cameraPos", camera.Position);

		model.Draw(shader);

	}
	
}


void drawHouse(Model& model,Model& model2,Model& wood, Shader& shader)
{
	glm::mat4 _model = glm::mat4(1.0f);
	//_model = glm::translate(_model, glm::vec3(0.0f,0.1f,0.0f));


	_model = glm::translate(_model, glm::vec3(-320.0, 2.1f, -340.0f));
	//_model = glm::rotate(_model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	_model = glm::scale(_model, glm::vec3(6.0f));

	glm::mat4 view = camera.GetViewMatrix();
	//glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4  projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	// draw scene as normal
	shader.use();
	shader.setMat4("model", _model);
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);
	shader.setVec3("cameraPos", camera.Position);

	model.Draw(shader);


	_model = glm::mat4(1.0f);

	_model = glm::translate(_model, glm::vec3(+100.0, 2.1f, -450.0f));
	//_model = glm::rotate(_model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	_model = glm::scale(_model, glm::vec3(1.5f));

	 view = camera.GetViewMatrix();
	//glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	  projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	// draw scene as normal
	shader.use();
	shader.setMat4("model", _model);
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);
	shader.setVec3("cameraPos", camera.Position);

	model2.Draw(shader);



	_model = glm::mat4(1.0f);

	_model = glm::translate(_model, glm::vec3(-200.0+30.0, 0.1f, -450.0-20.0f));
	//_model = glm::rotate(_model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	_model = glm::scale(_model, glm::vec3(10.0f));

	view = camera.GetViewMatrix();
	//glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	// draw scene as normal
	shader.use();
	shader.setMat4("model", _model);
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);
	shader.setVec3("cameraPos", camera.Position);

	wood.Draw(shader);

	for (int i = 0; i < 50; ++i)
	{
		_model = glm::translate(_model, glm::vec3(+30.0, 0.0f, -15.0f));

		if(i>=30)
		_model = glm::translate(_model, glm::vec3(+30.0, 0.0f, +15.0f));

		_model = glm::rotate(_model, glm::radians(50.0f),glm::vec3(.0, 1.0f, 0.0f));

		// draw scene as normal
		shader.use();
		shader.setMat4("model", _model);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
		shader.setVec3("cameraPos", camera.Position);

		wood.Draw(shader);
	}


}



void drawCone(Model& model,Shader& shader)
{
	glm::mat4 _model = glm::mat4(1.0f);
	//_model = glm::translate(_model, glm::vec3(0.0f,0.1f,0.0f));


	_model = glm::translate(_model, glm::vec3(-320.0-50.0, 2.1f, -340.0-20.0f));
	//_model = glm::rotate(_model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	_model = glm::scale(_model, glm::vec3(0.3f));

	glm::mat4 view = camera.GetViewMatrix();
	//glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4  projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	// draw scene as normal
	shader.use();
	shader.setMat4("model", _model);
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);
	shader.setVec3("cameraPos", camera.Position);

	model.Draw(shader);

	for (int i = 0; i < 10; ++i)
	{
		_model = glm::translate(_model, glm::vec3(- 50.0, 2.0f,+ 20.0f));
				
		// draw scene as normal
		shader.use();
		shader.setMat4("model", _model);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
		shader.setVec3("cameraPos", camera.Position);

		model.Draw(shader);
	}






}


void drawPlane(Model& model,Model& model2, Shader& shader)
{
	glm::mat4 _model = glm::mat4(1.0f);
	//_model = glm::translate(_model, glm::vec3(0.0f,0.1f,0.0f));


	_model = glm::translate(_model, glm::vec3(-320.0 - 50.0, 2.0f, -20.0f));
	_model = glm::rotate(_model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	_model = glm::scale(_model, glm::vec3(5.0f));

	glm::mat4 view = camera.GetViewMatrix();
	//glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4  projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	// draw scene as normal
	shader.use();
	shader.setMat4("model", _model);
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);
	shader.setVec3("cameraPos", camera.Position);

	model.Draw(shader);



	_model = glm::mat4(1.0f);
	_model = glm::translate(_model, glm::vec3(+320.0 - 50.0, 2.0f, -20.0f));
	_model = glm::rotate(_model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	_model = glm::scale(_model, glm::vec3(5.0f));

	
	shader.use();
	shader.setMat4("model", _model);
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);
	shader.setVec3("cameraPos", camera.Position);

	model2.Draw(shader);

}



