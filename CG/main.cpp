
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
#include"BasicShapes.h"
#include"Shadow.h"
//#define STB_IMAGE_IMPLEMENTATION
//#include"stb_image.h"
#include"Manager.h"
#include"Terrain.h"

#include"Physics.h"
#include"Vehicle.h"
//imgui 
#include"../include/imgui/imgui_impl_glfw.h"
#include"../include/imgui/imgui.h"
#include"../include/imgui/imgui_impl_opengl3.h"

#include"Utils.h"

#include<bullet/btBulletDynamicsCommon.h>
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

Physics* physicsWorld;
Terrain* terrain;

//plane 
//unsigned int planeVAO;
#endif
void renders() {
	glfwInit();

	//create a window
	GLFWwindow* window;
	createWindow(window, SCR_WIDTH, SCR_HEIGHT);
	glfwMakeContextCurrent(window);

	//set Viewport
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//init glad 
	gladInit();
	glEnable(GL_DEPTH_TEST);

	//init GUI 
	imGuiInit(window);

#ifdef TEST_OBJ
	//Shader skyShader("../shader/sky_box.vs", "../shader/sky_box.fs");
	Shader shadowShader("../shader/parallel_light/shadow_mapping_depth.vs", "../shader/parallel_light/shadow_mapping_depth.fs");
	Shader shader("../shader/parallel_light/pcss.vs", "../shader/parallel_light/pcss.fs");
	Shader lightShader("../shader/light.vs", "../shader/light.fs");
	Shader modelShader("../shader/model.vs", "../shader/model.fs");
	//Shader modelShader("../shader/parallel_light/pcss.vs", "../shader/parallel_light/pcss.fs");

	//Model model("../resources/objects/Avent_sport/Avent_sport.obj");

	//SkyBox* skybox = new SkyBox();
	DynamicSky* sky = new DynamicSky();
	Shadow* shadow = new Shadow();
	Plane* plane = new Plane();
	glm::vec3 lightPosition = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

	//Model car("../resources/objects/Mercedes_Benz/Mercedes_Benz.obj");
	Model car("../resources/objects/Avent_sport/Avent_sport.obj");
	Model raceTrackModel(("../resources/sceneResources/race-track/FullTrack.obj"));
	GLuint woodTexture = loadTexture("../resources/textures/wood.png");
	//std::cout << car.length << std::endl;
	//std::cout << car.width << std::endl;
	//std::cout << car.height << std::endl;
	//settings 

	// configure depth map FBO
	// -----------------------

	// shader configuration
	// --------------------

	shader.use();
	shader.setInt("diffuseTexture", 0);
	shader.setInt("shadowMap", 1);

	physicsWorld->ground();
	/*
	physicsWorld->phyManager->collisionShapes.push_back(terrain->heightFieldShape);
	btTransform field_trans;
	field_trans.setIdentity();
	auto motionState = new btDefaultMotionState(field_trans);
	btScalar mass = 0.0;
	btVector3 inertia(0, 0, 0);
	btRigidBody::btRigidBodyConstructionInfo fieldRigidBodyCI(mass, motionState, terrain->heightFieldShape, inertia);
	auto field_body = new btRigidBody(fieldRigidBodyCI);
	physicsWorld->phyManager->dynamicsWorld->addRigidBody(field_body);
	*/
	physicsWorld->setCube(-20, 10, 10);
	physicsWorld->setCube(-20, -10, 0);
	Vehicle vehicle(physicsWorld->phyManager->dynamicsWorld);
	vehicle.InitVehicle();

#endif
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window, vehicle);

		//1. Camera Movement

		//2. Objects Movement

		//3. Rendering 
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		//imgui---
		{
			ImGui::Begin("Controlling pad");
			ImGui::Text("Change light position.");
			ImGui::SliderFloat("x", &(lightPosition.x), 0.0f, 25.f);
			ImGui::SliderFloat("y", &(lightPosition.y), 0.0f, 25.f);
			ImGui::SliderFloat("z", &(lightPosition.z), 0.0f, 25.f);
			ImGui::Text("Camera pos");
			ImGui::SliderFloat("x", &camera.Position.x, -1.0f, 1.0f);
			ImGui::SliderFloat("y", &camera.Position.y, -1.0f, 1.0f);
			ImGui::SliderFloat("z", &camera.Position.z, -1.0f, 1.0f);
			ImGui::SliderFloat("Yaw", &camera.Yaw, -90.0f, 90.0f);
			ImGui::SliderFloat("Pitch", &camera.Pitch, -90.0f, 90.0f);
			ImGui::SliderFloat("Zoom", &camera.Zoom, 0.0f, 90.0f);
			ImGui::SliderFloat("Vehicle_X", &vehicle.positionX, -1000.0f, 1000.0f);
			ImGui::SliderFloat("Vehicle_Y", &vehicle.positionY, -1000.0f, 1000.0f);
			ImGui::SliderFloat("Vehicle_Z", &vehicle.positionZ, -1000.0f, 1000.0f);
			ImGui::End();
		}
		ImGui::Render();

		//--------
#ifdef TEST_OBJ

		//time 
		float rate = 5.0f; 
		float time = (float)glfwGetTime() * rate - 0.0f;
		lightPosition = glm::vec3(0.0, 10 * glm::sin(time * 0.01), 10 * glm::cos(time * 0.01));
		//1. render shadows 

		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		float near_plane = 1.0f;
		float far_plane = 50.f;

		lightProjection = glm::ortho(-10.f, 10.f, -10.f, 10.f, near_plane, far_plane);
		float fovy = glm::radians(90.f);
		float aspect = (float)shadow->shadowWidth / (float)shadow->shadowHeight;
		//lightProjection = glm::perspective(fovy, aspect, near_plane, far_plane);
		lightView = glm::lookAt(lightPosition, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;
		shadowShader.use();
		shadowShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

		glCullFace(GL_FRONT);
		glViewport(0, 0, shadow->shadowWidth, shadow->shadowHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, shadow->depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, plane->textureID);
		//bind wood texture? why? 
		glm::mat4 model = glm::mat4(1.0f);

		glBindVertexArray(plane->VAO);
		shadowShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		/*model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
		model = glm::translate(model, glm::vec3(1.0f));
		shadowShader.setMat4("model", model);
		renderCube();*/
		//glm::mat4 model = glm::mat4(1.0f);
		shadowShader.setMat4("model", vehicle.getTransform());
		car.Draw(shadowShader);

		model = glm::scale(glm::mat4(1.0f), glm::vec3(20.0f));
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -4.4f, 0.0f)) * model;
		modelShader.setMat4("model", model);
		raceTrackModel.Draw(shadowShader);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		//2. draw plane 
		/*
		glCullFace(GL_BACK);
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();

		shader.use();
		shader.setMat4("model", glm::mat4(1.0f));
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
		shader.setVec3("viewPos", camera.Position);
		shader.setVec3("lightPos", lightPosition);
		shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
		shader.setMat4("lightView", lightView);
		shader.setFloat("zNear", 1.0f);
		shader.setVec2("planeSize", glm::vec2(20.f, 20.f));
		shader.setVec2("lightSize", glm::vec2(5.f, 5.f));
		glBindVertexArray(terrain->terrainVAO);
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_TRIANGLES, 0, terrain->vertices.size() / 8);

		*/
		glCullFace(GL_BACK);
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		shader.use();
		//shader.setMat4("model", model);
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
		shader.setVec3("viewPos", camera.Position);
		shader.setVec3("lightPos", lightPosition);
		shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
		shader.setMat4("lightView", lightView);
		shader.setFloat("zNear", 1.0f);
		shader.setVec2("planeSize", glm::vec2(20.f, 20.f));
		shader.setVec2("lightSize", glm::vec2(5.f, 5.f));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, plane->textureID);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, shadow->depthMap);

		shader.setMat4("model", glm::mat4(1.0f));
		glBindVertexArray(plane->VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		
		/*draw car & road*/
		modelShader.use();
		modelShader.setMat4("projection", projection);
		modelShader.setMat4("view", view);
		modelShader.setVec3("viewPos", camera.Position);
		modelShader.setVec3("lightPos", lightPosition);
		modelShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
		modelShader.setMat4("lightView", lightView);
		modelShader.setFloat("zNear", 1.0f);
		modelShader.setVec2("planeSize", glm::vec2(20.f, 20.f));
		modelShader.setVec2("lightSize", glm::vec2(5.f, 5.f));
		//shader.setMat4("projection", projection);
		//shader.setMat4("view", view);
		//shader.setMat4("model", vehicle.getTransform() * glm::scale(glm::vec3(0.2f)));
		model = glm::scale(glm::mat4(1.0f), glm::vec3(20.0f));
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -4.4f, 0.0f)) * model;
		modelShader.setMat4("model", model);
		raceTrackModel.Draw(modelShader);

		modelShader.setMat4("model", vehicle.getTransform());
		car.Draw(modelShader);

		model = glm::mat4(1.0f);
		btCollisionObject* obj = physicsWorld->phyManager->dynamicsWorld->getCollisionObjectArray()[1];
		btRigidBody* body = btRigidBody::upcast(obj);
		btTransform trans;
		if (body && body->getMotionState())
			body->getMotionState()->getWorldTransform(trans);
		else
			trans = obj->getWorldTransform();
		glm::vec3 mov;
		mov.x = trans.getOrigin().getX();
		mov.y = trans.getOrigin().getY() - 0.5;
		mov.z = trans.getOrigin().getZ();
		model = glm::translate(glm::mat4(1.0f), mov) * model;
		//modelShader.setMat4("model", model);
		shader.use();
		shader.setMat4("model", model);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, woodTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, shadow->depthMap);
		renderCube();
		glBindTexture(GL_TEXTURE_2D, 0);

		//modelShader.use();		
		/*draw box*/
		model = glm::mat4(1.0f);
		obj = physicsWorld->phyManager->dynamicsWorld->getCollisionObjectArray()[2];
		body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
			body->getMotionState()->getWorldTransform(trans);
		else
			trans = obj->getWorldTransform();
		mov.x = trans.getOrigin().getX();
		mov.y = trans.getOrigin().getY() - 0.5;
		mov.z = trans.getOrigin().getZ();
		model = glm::translate(glm::mat4(1.0f), mov) * model;
		//modelShader.setMat4("model", model);
		shader.use();
		shader.setMat4("model", model);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, woodTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, shadow->depthMap);
		renderCube();
		glBindTexture(GL_TEXTURE_2D, 0);

		//lightShader.use();
		//glm::mat4 lightModel = glm::mat4(1.0f);
		//lightModel = glm::scale(model, glm::vec3(0.1f));
		//lightModel = glm::translate(model, lightPosition);
		//lightShader.setMat4("projection", projection);
		//lightShader.setMat4("view", view);
		//lightShader.setMat4("model", lightModel);
		//renderCube();

		sky->renderSky(camera,time);
		glBindVertexArray(0);
#endif
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
		vehicle.getKeyboard(window, deltaTime);
		vehicle.updateTransform(deltaTime);
		physicsWorld->phyManager->dynamicsWorld->stepSimulation(deltaTime);
	}
}

int main() {
	physicsWorld = new Physics;
	terrain = new Terrain("height_map.png");
	renders();
}

#ifdef TEST_OBJ

#endif