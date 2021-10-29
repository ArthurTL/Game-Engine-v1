#ifndef __SKYSHADER_H__
#define __SKYSHADER_H__
#include "ShaderProgram.h"

class SkyboxShader: public ShaderProgram {

private:
	GLuint location_projectionMatrix = 0;
	GLuint location_viewMatrix = 0;
	GLuint location_fogColor = 0;
	GLuint location_cubeMap = 0;
	GLuint location_cubeMap2 = 0;
	GLuint location_blendFactor = 0;
	const float ROTATE_SPEED = 0.25f;
	float rotation = 0.f;

public:
	SkyboxShader() :ShaderProgram("skyboxVertexShader.glsl", "skyboxFragmentShader.glsl") { getAllUniformLocations(); };
	inline void const loadProjectionMatrix(Camera camera) {
		mat4 projectionMatrix = camera.computeProjectionMatrix();
		ShaderProgram::loadMatrix(location_projectionMatrix, projectionMatrix);
	}
	inline void const loadViewMatrix(Camera camera) {
		mat4 viewMatrix = camera.computeViewMatrix();
		viewMatrix[3][0] = 0.;
		viewMatrix[3][1] = 0.;
		viewMatrix[3][2] = 0.;
		rotation += ROTATE_SPEED * WindowManager::getFrameTimeSeconds();
		viewMatrix = glm::rotate(viewMatrix, radians(rotation), vec3(0., 1., 0.));
		ShaderProgram::loadMatrix(location_viewMatrix, viewMatrix);
	}
	inline void const loadFogColor(float r,float g,float b) {
		ShaderProgram::loadVector(location_fogColor, vec3(r,g,b));
	}
	inline void const loadBlendFactor(float blend) {
		ShaderProgram::loadFloat(location_blendFactor, blend);
	}
	inline void connectTextureUnits() {
		ShaderProgram::loadInt(location_cubeMap, 0);
		ShaderProgram::loadInt(location_cubeMap2, 1);
	}

protected:
	inline void getAllUniformLocations() override {
		location_projectionMatrix = ShaderProgram::getUniformLocation("projectionMatrix");
		location_viewMatrix = ShaderProgram::getUniformLocation("viewMatrix");
		location_fogColor = ShaderProgram::getUniformLocation("fogColor");
		location_cubeMap = ShaderProgram::getUniformLocation("cubeMap");
		location_cubeMap2 = ShaderProgram::getUniformLocation("cubeMap2");
		location_blendFactor = ShaderProgram::getUniformLocation("blendFactor");
	}
	inline void bindAttributes() override {
		ShaderProgram::bindAttribute(0, "position");
	}




};





#endif
