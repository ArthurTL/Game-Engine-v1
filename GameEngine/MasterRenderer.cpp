#include "MasterRenderer.h"

const float MasterRenderer::SKY_RED= 116. / 256.;
const float MasterRenderer::SKY_GREEN= 148. / 256.;
const float MasterRenderer::SKY_BLUE= 170. / 256.;

MasterRenderer::MasterRenderer(Loader loader, Camera* camera) { 
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	shader->start();
	shader->connectTextureUnits();
	shader->loadProjectionMatrix(*camera);
	shader->stop();
	terrainShader->start();
	terrainShader->connectTextureUnits();
	terrainShader->loadProjectionMatrix(*camera);
	terrainShader->stop();
	normalShader->start();
	normalShader->connectTextureUnits();
	normalShader->loadProjectionMatrix(*camera);
	normalShader->stop();
	skyboxRenderer = new SkyboxRenderer(loader, *camera);
	shadowMapRenderer = new ShadowMapMasterRenderer(camera);
	time = skyboxRenderer->getTime();
	
}

void MasterRenderer::renderScene(std::vector<Entity*> entities, std::vector<Entity*> normalEntities, std::vector<Terrain*>terrains, std::vector<Light*> lights, Camera* camera, glm::vec4 clipPlane) {
	for (Terrain* terrain : terrains) {
		processTerrain(terrain);
	}

	for (Entity* entity : entities) {
	    processEntity(entity);
	}
	for (Entity* entity : normalEntities) {
		processNormalEntity(entity);
	}

	render(lights, *camera, clipPlane);
}

void MasterRenderer::render(std::vector<Light*> lights, Camera camera, glm::vec4 clipPlane) {

	MasterRenderer::prepare();
	shader->start();
	shader->loadClipPlane(clipPlane);
	shader->loadSkyColor(SKY_RED, SKY_GREEN, SKY_BLUE);
	shader->loadLights(lights);
	shader->loadViewMatrix(camera);
	getBlendFactor();
	renderer->render(entities, shadowMapRenderer->getToShadowMapSpaceMatrix());
	
	shader->stop();
	
	normalShader->start();
	normalShader->loadClipPlane(clipPlane);
	normalShader->loadSkyColor(SKY_RED, SKY_GREEN, SKY_BLUE);
	normalShader->loadLights(lights,camera);
	normalShader->loadViewMatrix(camera);
	getBlendFactor();
	normalRenderer->render(normalEntities, shadowMapRenderer->getToShadowMapSpaceMatrix());
	normalShader->stop();
	
	terrainShader->start();
	terrainShader->loadClipPlane(clipPlane);
	terrainShader->loadSkyColor(SKY_RED, SKY_GREEN, SKY_BLUE);
	terrainShader->loadLights(lights);
	terrainShader->loadViewMatrix(camera);
	getBlendFactor();
	terrainRenderer->render(terrains, shadowMapRenderer->getToShadowMapSpaceMatrix());
	terrainShader->stop();
	skyboxRenderer->render(camera, SKY_RED, SKY_GREEN, SKY_BLUE);
	terrains.clear();
	entities.clear();
	normalEntities.clear();

}

void MasterRenderer::renderShadowMap(std::vector<Entity*>entityList, std::vector<Entity*> normalEntityList,Light* sun,Camera* camera){
	
	for (Entity *entity : entityList) {
		processEntity(entity);
	}
	for (Entity* entity : normalEntityList) {
		processNormalEntity(entity);
	}

	shadowMapRenderer->render(entities,normalEntities,*sun,camera);
	
	
	normalEntities.clear();
	entities.clear();
}

void MasterRenderer::processEntity(Entity* entity ) {
	TexturedModel* entityModel = entity->meshes[0];
	auto findBatch = entities.find(entityModel);

	if (!(findBatch==entities.end())) {
		std::vector<Entity*> *batch = &entities.at(entityModel);
		batch->push_back(entity);
	}
	else {
		const std::vector<Entity*> newBatch = {entity};
		entities.insert(std::make_pair(entityModel, newBatch));
	}
}

void MasterRenderer::processNormalEntity(Entity* entity) {
	TexturedModel* entityModel = entity->meshes[0];
	auto findBatch = normalEntities.find(entityModel);

	if (!(findBatch == normalEntities.end())) {
		std::vector<Entity*>* batch = &normalEntities.at(entityModel);
		batch->push_back(entity);
	}
	else {
		const std::vector<Entity*> newBatch = { entity };
		normalEntities.insert(std::make_pair(entityModel, newBatch));
	}
}

void MasterRenderer::processTerrain(Terrain* terrain) {
	terrains.push_back(terrain);
}


void MasterRenderer::cleanUp() {
	shader->cleanUp();
	terrainShader->cleanUp();
	skyboxRenderer->getShader()->cleanUp();
	normalShader->cleanUp();
	shadowMapRenderer->cleanUp();
}

void MasterRenderer::prepare() {
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(SKY_RED, SKY_GREEN, SKY_BLUE, 1);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, getShadowMapTexture());
}
void MasterRenderer::getBlendFactor() {
	time = skyboxRenderer->getTime();
	
	float blendFactor;
	if (time >= 0 && time < 5000) {
		blendFactor = 1.f;
	}
	else if (time >= 5000 && time < 8000) {
		blendFactor = 1 - ((time - 5000) / (8000 - 5000));
	}
	else if (time >= 8000 && time < 21000) {
		blendFactor = 0.f;
	}
	else {
		blendFactor = (time - 21000) / (24000 - 21000);
	}
	shader->loadBlendFactor(1-blendFactor);
	terrainShader->loadBlendFactor(1-blendFactor);
	normalShader->loadBlendFactor(1 - blendFactor);
}
