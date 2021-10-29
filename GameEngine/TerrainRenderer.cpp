#include "TerrainRenderer.h"

void TerrainRenderer::render(std::vector<Terrain*> terrains, mat4 toShadowSpace) {
	shader->loadToShadowMapSpace(toShadowSpace);
	for (Terrain* terrain : terrains) {
		prepareTerrain(*terrain);
		loadModelMatrix(*terrain);
		glDrawElements(GL_TRIANGLES, (*terrain).model->vertexCount, GL_UNSIGNED_INT, 0);
		unbindTexturedModel();
	}
}

void TerrainRenderer::prepareTerrain(Terrain terrain) {
	const RawModel *rawModel = terrain.model;
	glBindVertexArray(rawModel->vaoID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	bindTextures(terrain);
	shader->loadShineVariables(1,0);
}

void TerrainRenderer::bindTextures(Terrain terrain) {
	TerrainTexturePack* texturePack = terrain.texturePack;
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texturePack->getBackgroundTexture().getTextureID());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texturePack->getrTexture().getTextureID());
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texturePack->getgTexture().getTextureID());
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, texturePack->getbTexture().getTextureID());
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, terrain.blendMap->getTextureID());
}
void TerrainRenderer::unbindTexturedModel() {
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindVertexArray(0);
}

void TerrainRenderer::loadModelMatrix(Terrain terrain) {
	const mat4 transformationMatrix = Maths::createTransformationMatrix(vec3(terrain.x,0,terrain.z),0, 0, 0, 1);
	shader->loadTransformationMatrix(transformationMatrix);
}

