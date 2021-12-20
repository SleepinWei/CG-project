#ifndef BASIC_SHAPES_H
#define BASIC_SHAPES_H
#include<glfw/glfw3.h>
#include<vector>
#include<string>
#include<shader/Shader.h>
#include"stb_image.h"
#include"Utils.h"

class Plane {
public:
	GLuint VAO,VBO;
	GLuint textureID;

	std::vector<float> vertices;

	Plane() {
		this->vertices = {
			// positions            // normals         // texcoords
				100.0f, -0.5f,  100.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
				-100.0f, -0.5f, 100.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
				-100.0f, -0.5f, -100.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

				100.0f, -0.5f,  100.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
				-100.0f, -0.5f, -100.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
				100.0f, -0.5f, -100.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
		};
		setUp();
	}
	void setUp() {
		//set VAO
		unsigned int planeVBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &planeVBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float),& vertices[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindVertexArray(0);

		//set texture
		textureID = loadTexture("../resources/textures/grass.png");
	//	int width, height, nrComponents;
	//	unsigned char* data = stbi_load("../resources/textures/grass.png", &width, &height, &nrComponents, 3);
	//	if (data)
	//	{
	//		GLenum format;
	//		if (nrComponents == 1)
	//			format = GL_RED;
	//		else if (nrComponents == 3)
	//			format = GL_RGB;
	//		else if (nrComponents == 4)
	//			format = GL_RGBA;

	//		glBindTexture(GL_TEXTURE_2D, textureID);
	//		//glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	//		glGenerateMipmap(GL_TEXTURE_2D);

	//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//		stbi_image_free(data);
	//	}
	//	else
	//	{
	//		std::cout << "Texture failed to load at path: " << "grass.jpg" << std::endl;
	//		stbi_image_free(data);
	//	}
	}
};

#endif // !BASIC_SHAPES_H
