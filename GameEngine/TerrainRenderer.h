#ifndef __TRENDERER_H__
#define __TRENDERER_H__
#include "TerrainShader.h"
#include "Entity.h"
#include "Maths.h"
#include "Terrain.h"
#include <vector>

class TerrainRenderer {

private:

	TerrainShader* shader;
	void prepareTerrain(Terrain terrain);
	void unbindTexturedModel();
	void loadModelMatrix(Terrain terrain);

public:

	TerrainRenderer(TerrainShader* shader_) : shader(shader_) {};
	void bindTextures(Terrain terrain);
	void render(std::vector<Terrain*> terrains,mat4 toShadowSpace) ;

};







#endif