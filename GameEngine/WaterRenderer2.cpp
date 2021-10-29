#include "WaterRenderer2.h"

const string WaterRenderer::DUDV_MAP = "waterDUDV.png";
const string WaterRenderer::NORMAL_MAP = "normalMap.png";
const float WaterRenderer::WAVE_SPEED = 0.03f;

WaterRenderer::WaterRenderer(Loader loader, WaterShader* shader_, Camera camera, WaterFrameBuffers *fbos_) {
	fbos = fbos_;
	shader = shader_;
	dudvTexture = loader.loadTexture(DUDV_MAP);
	normalMap = loader.loadTexture(NORMAL_MAP);
	shader->start();
	shader->connectTextureUnits();
	shader->loadProjectionMatrix(camera);
	shader->stop();
	setupVAO(loader);
}

void WaterRenderer::render(vector<WaterTile> water, Camera camera, std::vector<Light*> lights) {
	prepareRender(camera,lights);
	for (WaterTile tile : water) {
		mat4 modelMatrix = Maths::createTransformationMatrix(
			vec3(tile.getX(), tile.getHeight(), tile.getZ()), 0, 0, 0, WaterTile::TILE_SIZE);
		shader->loadModelMatrix(modelMatrix);
		glDrawArrays(GL_TRIANGLES, 0, quad->vertexCount);
	}
	unbind();
}

void WaterRenderer::prepareRender(Camera camera, std::vector<Light*> lights) {
	shader->start();
	shader->loadViewMatrix(camera);
	moveFactor += WAVE_SPEED * WindowManager::getFrameTimeSeconds();
	moveFactor = fmod(moveFactor,1.);
	shader->loadMoveFactor(moveFactor);
	shader->loadLights(lights);
	glBindVertexArray(quad->vaoID);
	glEnableVertexAttribArray(0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fbos->getReflectionTexture());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, fbos->getRefractionTexture());
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, dudvTexture);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, normalMap);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, fbos->getRefractionDepthTexture());

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
void WaterRenderer::unbind() {
	glDisable(GL_BLEND);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	shader->stop();
}

void WaterRenderer::setupVAO(Loader loader) {
	std::vector<float> vertices = { -1, -1, -1, 1, 1, -1, 1, -1, -1, 1, 1, 1 };
	quad = loader.loadToVAO(vertices, 2);
}
