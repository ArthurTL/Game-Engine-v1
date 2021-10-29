#ifndef __MRENDERER_H__
#define __MRENDERER_H__
#include "OBJLoader.h"
#include "EntityRenderer.h"
#include "TerrainRenderer.h"
#include "SkyboxRenderer.h"
#include "NormalRenderer.h"
#include "ShadowMapMasterRenderer.h"


class MasterRenderer {
private:
	
	float time;
	void getBlendFactor();
	StaticShader* shader = new StaticShader();
	EntityRenderer* renderer = new EntityRenderer(shader);

	TerrainShader* terrainShader = new TerrainShader();
	TerrainRenderer* terrainRenderer = new TerrainRenderer(terrainShader);
	
	SkyboxRenderer* skyboxRenderer= NULL;
	ShadowMapMasterRenderer* shadowMapRenderer = NULL;
	NormalShader* normalShader = new NormalShader();
	NormalRenderer* normalRenderer = new NormalRenderer(normalShader);
	std::map<TexturedModel*, std::vector<Entity*>> entities;
	std::map<TexturedModel*, std::vector<Entity*>> normalEntities;
	std::vector<Terrain*>  terrains;


public:

	static const float SKY_RED;
	static const float SKY_GREEN;
	static const float SKY_BLUE;
	MasterRenderer(Loader loader,Camera* camera);

	void prepare();
	void processEntity(Entity* entity);
	void processNormalEntity(Entity* entity);
	void renderScene(std::vector<Entity*>, std::vector<Entity*>, std::vector<Terrain*>, std::vector<Light*>, Camera* camera, glm::vec4 clipPlane);
	void renderShadowMap(std::vector<Entity*>, std::vector<Entity*>, Light* sun, Camera* camera);
	void processTerrain(Terrain* terrain);
	void render(std::vector<Light*> lights, Camera camera, glm::vec4 clipPlane);
	inline float getTime() const { return time; }
	inline GLuint getShadowMapTexture() { return shadowMapRenderer->getShadowMap();}
	void cleanUp();
	
};

#endif