#ifndef __TERRAIN_H__
#define __TERRAIN_H__
#include "Loader.h"
#include "TerrainTexturePack.h"
#include "stb_image.h"
#include "glm.hpp"
#include <vector>
#include "Maths.h"

class Terrain {

private:
	static const float SIZE;
	static const float MAX_HEIGHT;
	static const float MAX_PIXEL_COLOR;

	vector<vector<float>> heights;
	float getHeight(int x, int y, unsigned char* heightMap,int height, int channelCount);
	glm::vec3 calculateNormal(int x, int y, unsigned char* heightMap, int height, int channelCount);
public:
	float x;
	float z;
	RawModel* model = NULL;
	TerrainTexturePack * texturePack = NULL;
	TerrainTexture* blendMap = NULL;
	
	float getHeightOfTerrain(float worldX, float worldZ);

	Terrain(int gridX, int gridZ, Loader loader, TerrainTexturePack* texturePack, TerrainTexture* blendMap, const string& heightMap);
	RawModel* generateTerrain(Loader loader,const string& heightMap);

	static int determineTerrain(float x, float y);
};







#endif

