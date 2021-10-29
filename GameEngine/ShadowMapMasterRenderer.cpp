#include "ShadowMapMasterRenderer.h"

ShadowMapMasterRenderer::ShadowMapMasterRenderer(Camera* camera){
	shader = new ShadowShader();
	shadowBox = new ShadowBox(lightViewMatrix, camera);
	shadowFbo = new ShadowFrameBuffer(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE);
	entityRenderer = new ShadowMapEntityRenderer(shader, projectionViewMatrix);
}

void ShadowMapMasterRenderer::render(std::map<TexturedModel*, std::vector<Entity*>> entities, std::map<TexturedModel*, std::vector<Entity*>> normalEntities, Light sun,Camera* camera){
	shadowBox->update();
	vec3 lightDirection = -sun.getPosition();
	prepare(lightDirection, shadowBox);
	entityRenderer->render(entities);
	entityRenderer->render(normalEntities);
	finish();
}
void ShadowMapMasterRenderer::cleanUp(){
	shader->cleanUp();
	shadowFbo->cleanUp();
}

void ShadowMapMasterRenderer::prepare(vec3 lightDirection, ShadowBox* box) {
	updateOrthoProjectionMatrix(box->getWidth(), box->getHeight(), box->getLength(), box->getWidth2(), box->getHeight2(), box->getLength2());
	updateLightViewMatrix(lightDirection, box->getCenter());
	
	*projectionViewMatrix = projectionMatrix * *lightViewMatrix;
	shadowFbo->bindFrameBuffer();
	//glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);
	shader->start();
}

void ShadowMapMasterRenderer::updateOrthoProjectionMatrix(float width, float height, float length, float width2, float height2, float length2) {
	
	projectionMatrix = mat4(1.0f);
	projectionMatrix[0][0] = 2.f / width;
	projectionMatrix[1][1] = 2.f / height;
	projectionMatrix[2][2] = -2.f / length;
	projectionMatrix[3][3] = 1.f;
	projectionMatrix[3][0] = -width2 / width;
	projectionMatrix[3][1] = -height2 / height;
	projectionMatrix[3][2] = length2 / length;
}
void ShadowMapMasterRenderer::updateLightViewMatrix(vec3 direction, vec3 center_){
	direction = glm::normalize(direction);
	vec3 center = -center_;
	mat4 lightViewMatrix_ = mat4(1.0f);
	float pitch = (float)acos(glm::length(vec2(direction.x, direction.z)));
	lightViewMatrix_ = glm::rotate(lightViewMatrix_, pitch, vec3(1.f, 0.f, 0.f));
	float yaw = (float)degrees(((float)atan(direction.x / direction.z)));
	yaw = direction.z > 0 ? yaw - 180 : yaw;
	lightViewMatrix_ = glm::rotate(lightViewMatrix_, (float) -radians(yaw), vec3(0.f, 1.f, 0.f));
	lightViewMatrix_ = glm::translate(lightViewMatrix_, center_);
	double scale = 0.0001;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			lightViewMatrix_[i][j] = std::floor(lightViewMatrix_[i][j]/ scale + 0.5) * scale;
		}
	}
	*lightViewMatrix = lightViewMatrix_;
}



void ShadowMapMasterRenderer::finish() {
	shader->stop();
	shadowFbo->unbindFrameBuffer();
}