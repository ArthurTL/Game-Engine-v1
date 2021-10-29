#include "EntityRenderer.h"


EntityRenderer::EntityRenderer(StaticShader *shader_) {
	shader = shader_;
}


void EntityRenderer::render(std::map<TexturedModel *, std::vector<Entity*>> entities, mat4 toShadowMapSpace) {
	shader->loadToShadowMapSpace(toShadowMapSpace);
	for (const auto &pair : entities) {
	 	TexturedModel* model = pair.first;
		
		prepareTexturedModel(*model);
		std::vector<Entity*> batch = entities.at(model);
		for (Entity* entity : batch) {
			prepareInstance(*entity);
			if (entity->useTriangleStrips) {
				glDrawArrays(GL_TRIANGLES, 0,model->rawModel.vertexCount);
			}
			else {
				glDrawElements(GL_TRIANGLES, model->rawModel.vertexCount, GL_UNSIGNED_INT, 0);
			}
		}
	unbindTexturedModel();
	}
}

void EntityRenderer::prepareTexturedModel(TexturedModel texturedModel) {
	RawModel rawModel = texturedModel.rawModel;
	glBindVertexArray(rawModel.vaoID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	ModelTexture texture = texturedModel.texture;
	shader->loadNumberOfRows(texture.numberOfRows);
	if (texture.getHasTransparency()) {
		glDisable(GL_CULL_FACE);
	}
	shader->loadFakeLightingVariable(texture.getUseFakeLighting());
	shader->loadShineVariables(texture.getShineDamper(), texture.getReflectivity(), texture.getAmbiantColor());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texturedModel.texture.textureID);

}
void EntityRenderer::unbindTexturedModel() {
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindVertexArray(0);
}

void EntityRenderer::prepareInstance(Entity entity) {
	mat4 transformationMatrix = Maths::createTransformationMatrix(entity.position, entity.rx, entity.ry, entity.rz, entity.scale);
	shader->loadTransformationMatrix(transformationMatrix);
	shader->loadOffset(entity.getTextureXOffset(), entity.getTextureYOffset());
}

