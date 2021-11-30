#ifndef SHADERS_H
#define SHADERS_H
/****************************
* This file is different from shader.h 
* In this file, settings for all kinds of shaders are stored 
* so that it'll be easier to setUp shaders 
*****************************/
#include<shader/Shader.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<vector>

class ParaSoftShadowShader {
	/**************************
	* pcss for parallel light
	* para stands for parallel
	**************************/
public:
	Shader* shadowShader;
	Shader* sceneShader;
	std::string path;

	ParaSoftShadowShader() {
		path = "../shader/parallelLight/";
		shadowShader = new Shader((path + "shadow_mapping_depth.vs").c_str(),(path + "shadow_mapping_depth.fs").c_str());
		sceneShader = new Shader((path + "shadow_mapping.vs").c_str(),(path+"shadow_mapping.fs").c_str());
	}
	~ParaSoftShadowShader() {
		delete shadowShader;
		delete sceneShader;
	}
	void sceneShaderSetUp(
		glm::mat4 projection,
		glm::mat4 view,
		glm::mat4 model,
		glm::vec3 lightPos,
		glm::vec3 viewPos,
		glm::vec2 lightSize,
		glm::mat4 lightView,
		float zNear,
		glm::vec2 planeSize
	) {
		//setUp scene Shaders
		sceneShader->setMat4("projection", projection);
		sceneShader->setMat4("view", view);
		sceneShader->setMat4("model", model);
		sceneShader->setVec3("lightPos", lightPos);
		sceneShader->setVec3("viewPos", viewPos);
		sceneShader->setVec2("lightSize", lightSize);
		sceneShader->setMat4("lightView", lightView);
		sceneShader->setFloat("zNear", zNear);
		sceneShader->setVec2("planeSize", planeSize);
	}
	void shadowShaderSetUp(
		glm::mat4 projection,
		glm::mat4 view,
		glm::mat4 model
	) {
		shadowShader->setMat4("projection", projection);
		shadowShader->setMat4("view", view);
		shadowShader->setMat4("model", model);
	}
};

class PointShadowShader {
	Shader* shadowShader;
	Shader* sceneShader;
	std::string path;

	PointShadowShader() {
		path = "../shader/point_light/";
		shadowShader = new Shader("sm_depth.vs","sm_depth.fs","sm_depth.gs");
		sceneShader = new Shader("sm.vs", "sm.fs");
	}
	~PointShadowShader() {
		delete shadowShader;
		delete sceneShader;
	}
	void shadowShaderSetUp(
		glm::mat4 projection,
		glm::mat4 view,
		glm::mat4 model,
		glm::vec3 lightPos,
		glm::mat4 shadowProj,
		float far_plane,
		float near_plane,
		std::vector<glm::mat4> shadowMatrices
	) {
		shadowShader->setMat4("projection", projection);
		shadowShader->setMat4("view", view);
		shadowShader->setMat4("model", model);

		for (int i = 0; i < shadowMatrices.size(); i++) {
			//there may be less than 6 faces 
			shadowShader->setMat4("shadowMatrices[" + std::to_string(i) + "]", shadowMatrices[i]);
		}
		shadowShader->setVec3("lightPos", lightPos);
		shadowShader->setFloat("far_plane", far_plane);
	}
	void sceneShaderSetUp(
		glm::mat4 projection,
		glm::mat4 view,
		glm::mat4 model,
		bool reverse_normals,
		glm::vec3 lightPos,
		glm::vec3 viewPos,
		float far_plane,
		bool shadows
	) {
		sceneShader->setMat4("projection", projection);
		sceneShader->setMat4("view", view);
		sceneShader->setMat4("model", model);
		sceneShader->setBool("reverse_normals", reverse_normals);
		sceneShader->setVec3("lightPos", lightPos);
		sceneShader->setVec3("viewPos", viewPos);
		sceneShader->setFloat("far_plane", far_plane);
		sceneShader->setBool("shadows", shadows);
	}
};
#endif // !SHADERS_H
