#include "ShadowMapEntityRenderer.h"


void ShadowMapEntityRenderer::render(std::map<TexturedModel*, std::vector<Entity*>> entities){
	for (const auto& pair : entities) {
		TexturedModel* model = pair.first;
		RawModel rawModel = model->rawModel;
		bindModel(rawModel);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, model->texture.textureID);
		if (model->texture.getHasTransparency()) {
			glDisable(GL_CULL_FACE);
		}
		std::vector<Entity*> batch = entities.at(model);
		for (Entity* entity : batch) {
			prepareInstance(*entity);
			glDrawElements(GL_TRIANGLES, model->rawModel.vertexCount, GL_UNSIGNED_INT, 0);
			
		}
		if (model->texture.getHasTransparency()) {
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
		}
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glBindVertexArray(0);
	}
}

void ShadowMapEntityRenderer::bindModel(RawModel rawModel) {
	glBindVertexArray(rawModel.vaoID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}

void ShadowMapEntityRenderer::prepareInstance(Entity entity) {
	mat4 modelMatrix = Maths::createTransformationMatrix(entity.position,
		entity.rx, entity.ry, entity.rz, entity.scale);
	mat4 mvpMatrix = *projectionViewMatrix * modelMatrix;

	shader->loadMvpMatrix(mvpMatrix);
}