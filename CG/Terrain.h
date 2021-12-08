
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
	//physics related
	float* heightFieldData;
	btHeightfieldTerrainShape* heightFieldShape;
	bool isDynamic;

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

	std::string path;

	/*functions*/
	Terrain(std::string filename) {
		widthScale = 1.0;
		heightScale = 1.0;
		heightScale = 1.0;
		upAxis = 2; //y axis is up 
		type = PHY_FLOAT;
		flipQuadEdges = false;

		isDynamic = false;

		path = "../resources/height_map/";
		//read 3 channels
		stbi_set_flip_vertically_on_load(true);
		byte* rawHeightFieldData = stbi_load((path + filename).c_str(), &mapWidth, &mapHeight, &mapChannels, 3);
		heightFieldData = new float[mapWidth * mapHeight];
		for (int i = 0; i < mapHeight; ++i)
		{
			for (int j = 0; j < mapWidth; ++j) {
				int index = i * mapWidth + j;
				float h = rawHeightFieldData[3 * index] / 256.0 * valueScale;
				heightFieldData[index] = h;
				if (!i && !j) {
					minHeight = h;
					maxHeight = h;
				}
				else {
					if (h < minHeight) {
						minHeight = h;
					}
					if (h > maxHeight) {
						maxHeight = h;
					}
				}
			}
		}
		stbi_image_free(rawHeightFieldData);
		

		heightFieldShape = new btHeightfieldTerrainShape(gridSize, gridSize, (byte*)heightFieldData, heightScale,
			minHeight,maxHeight,upAxis,type,flipQuadEdges); //what's the use of gridSize？
		//optional,may not support all feature
		heightFieldShape->buildAccelerator();
		//scale the shape 
		btVector3 localScaling = btVector3(widthScale, 1.0, heightScale);
		heightFieldShape->setLocalScaling(localScaling);
	}
	~Terrain() {
		//delete rawHeightFieldData;
		delete heightFieldData;
	}
	int getIndex(int x, int y) {
		//y in [0,mapHeight-1]; x in [0,mapWidth-1] 
		return y * mapHeight + x;
	}

	void renderTerrain(Shader& shader) {
		
	}
};
#endif // !TERRAIN_H
