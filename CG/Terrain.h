
/**************************
* This file is for the terrain class
* It stores functions both in physics world
* and the rendering world
***************************/

#ifndef TERRAIN_H
#define TERRAIN_H

#include<bullet/btBulletCollisionCommon.h>
#include<bullet/btBulletDynamicsCommon.h>
#include<bullet/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>

#include<shader/Shader.h>
#include<string>
#include"stb_image.h"

typedef unsigned char byte;

class Terrain {
public:
	/*variables*/
	unsigned int terrainVAO;
	unsigned int terrainVBO;
	GLuint textureID;

	std::vector<glm::vec3> normals;
	std::vector<float> vertices;

	//physics related
	float* heightFieldData;
	//byte* rawHeightFieldData;//just for test 
	btHeightfieldTerrainShape* heightFieldShape;

	//texture related
	int mapWidth, mapHeight;
	int mapChannels;

	//常数，即高度图上每一格对应世界中的比例
	int gridSize = 16 + 1; //must be 2^N + 1; don't know what this variable is for.
	btScalar widthScale;//width
	btScalar heightScale;//height
	btScalar valueScale;
	btScalar minHeight;
	btScalar maxHeight;
	PHY_ScalarType type;
	int upAxis;
	float phase;
	bool flipQuadEdges;
	const int radix = 50;

	std::string path;

	/*functions*/
	Terrain(std::string filename) {
		widthScale = 1.0;
		heightScale = 1.0;
		valueScale = 10.0f;
		upAxis = 1; //y axis is up 
		type = PHY_FLOAT;
		flipQuadEdges = false;

		path = "../resources/height_map/";
		//read 3 channels

		stbi_set_flip_vertically_on_load(true);
		byte* rawHeightFieldData = stbi_load((path + filename).c_str(), &mapWidth, &mapHeight, &mapChannels, 3);
		heightFieldData = new float[(radix + 2) * (radix + 2)];
		minHeight = 10000;
		maxHeight = 0;
		int count = 0;
		for (int i = 0; i < radix + 2; ++i)
		{
			for (int j = 0; j < radix + 2; ++j)
			{
				int index = (100 + i) * mapWidth + 100 + j;
				int R = (int)rawHeightFieldData[3 * index];
				int G = (int)rawHeightFieldData[3 * index + 1];
				int B = (int)rawHeightFieldData[3 * index + 2];

				int grey = (R * 38 + G * 75 + B * 15) >> 5;
				float h = grey / 256.0 * valueScale;
				//std::cout << h << std::endl;
				heightFieldData[count++] = h;
				if (h < minHeight)
					minHeight = h;
				if (h > maxHeight)
					maxHeight = h;
			}
		}
		stbi_image_free(rawHeightFieldData);


		heightFieldShape = new btHeightfieldTerrainShape(radix, radix, (byte*)heightFieldData, heightScale,
			minHeight, maxHeight, upAxis, type, flipQuadEdges); //what's the use of gridSize？
		//optional,may not support all feature
		heightFieldShape->buildAccelerator();
		//scale the shape
		btVector3 localScaling = btVector3(widthScale, 1.0, heightScale);
		heightFieldShape->setLocalScaling(localScaling);

		for (int i = 1; i <= radix; i++)
		{
			for (int j = 1; j <= radix; j++)
			{
				glm::vec3 normal(heightFieldData[(i - 1) * 52 + j] - heightFieldData[(i + 1) * 52 + j], heightFieldData[i * 52 + j - 1] - heightFieldData[i * 52 + j + 1], 2.0);
				normal = glm::normalize(normal);
				normals.push_back(normal);
			}
		}
		for (int i = 1; i <= radix - 1; i++)
		{
			for (int j = 1; j <= radix - 1; j++)
			{
				pushVertice(i, j);
				pushVertice(i, j + 1);
				pushVertice(i + 1, j + 1);
				pushVertice(i, j);
				pushVertice(i + 1, j);
				pushVertice(i + 1, j + 1);
			}
		}
	}
	~Terrain() {
		//delete rawHeightFieldData;
		delete heightFieldData;
	}
	void pushVertice(int x, int y)
	{
		vertices.push_back(x - radix / 2 + 10);
		vertices.push_back(heightFieldData[x * 52 + y] - (maxHeight + minHeight) / 2);
		vertices.push_back(y - radix / 2 + 60);
		auto normal = normals[50 * (x - 1) + y - 1];
		vertices.push_back(normal.x);
		vertices.push_back(normal.y);
		vertices.push_back(normal.z);
		//vertices.push_back(0.0f);
		//vertices.push_back(0.0f);
		vertices.push_back((rand() % 1024) / 1024.0f);
		vertices.push_back((rand() % 1024) / 1024.0f);
		//随便填2位纹理
	}

	void prepareTerrain() {
		glGenVertexArrays(1, &terrainVAO);
		glGenBuffers(1, &terrainVBO);
		glBindVertexArray(terrainVAO);
		glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindVertexArray(0);
	}
};
#endif // !TERRAIN_H