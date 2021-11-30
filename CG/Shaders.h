#ifndef SHADERS_H
#define SHADERS_H
/****************************
* This file is different from shader.h 
* In this file, settings for all kinds of shaders are stored 
* so that it'll be easier to setUp shaders 
*****************************/
#include<shader/Shader.h>
#include<glm/glm.hpp>
class PcssSoftShadowShader {
	/**************************
	* pcss for parallel light
	**************************/
public:
	Shader* shadowShader;
	Shader* sceneShader;
	std::string path;

	PcssSoftShadowShader() {
		path = "../shader/";
		shadowShader = new Shader((path + "shadow_mapping_depth.vs").c_str(),(path + "shadow_mapping_depth.fs").c_str());
		sceneShader = new Shader((path + "shadow_mapping.vs").c_str(),(path+"shadow_mapping.fs").c_str());
	}
	~PcssSoftShadowShader() {
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
#endif // !SHADERS_H
