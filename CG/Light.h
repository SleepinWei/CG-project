#ifndef LIGHT_H
#define LIGHT_H
/*********************************
* This header file contains light models
* point light 
* parallel light 
* flash light 
**********************************/
#include<shader/Shader.h>
#include<glfw/glfw3.h>
#include<glm/glm.hpp>
#include<string>


class Light {
public:

	//do setting work for shader 
	virtual void setUp(Shader& shader) = 0;
};



class PointLight:virtual public Light{
public:
	/*variables*/
	glm::vec3 position;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	
	//constants for attenuation 
	float constant;
	float linear;
	float quadratic;

	/*functions*/
	void setUp(Shader& shader,std::string& lightName) {
		/*
		* @para lightName "name of variable for light" 
		*/
		shader.setVec3(lightName + ".position", position);
		shader.setVec3(lightName + ".ambient", ambient);
		shader.setVec3(lightName + ".diffuse", diffuse);
		shader.setVec3(lightName + ".specular", specular);

		shader.setFloat(lightName + ".constant", constant);
		shader.setFloat(lightName + ".linear", linear);
		shader.setFloat(lightName + ".quadratic", quadratic);
	}
};

class FlashLight: virtual public Light {
public:
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	//constants for attenuation 
	float constant;
	float linear;
	float quadratic;
	float cutOff;
	float outerCutOff;

	void setUp(Shader& shader, std::string& lightName) {
		/*
		* @para lightName "name of variable for light"
		*/
		shader.setVec3(lightName + ".position", position);
		shader.setVec3(lightName + ".direction", direction);
		shader.setFloat(lightName + ".cutOff", cutOff);
		shader.setFloat(lightName + ".outerCutOff", outerCutOff);

		shader.setVec3(lightName + ".ambient", ambient);
		shader.setVec3(lightName + ".diffuse", diffuse);
		shader.setVec3(lightName + ".specular", specular);

		shader.setFloat(lightName + ".constant", constant);
		shader.setFloat(lightName + ".linear", linear);
		shader.setFloat(lightName + ".quadratic", quadratic);
	}
};

#endif // !LIGHT_H
