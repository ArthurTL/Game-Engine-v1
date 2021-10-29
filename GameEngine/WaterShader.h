#ifndef __WATERSHADER_H__
#define __WATERSHADER_H__

#include "ShaderProgram.h"
#include "WaterFrameBuffers.h"
class WaterShader : public ShaderProgram{

private:

	GLuint location_modelMatrix;
	GLuint location_viewMatrix;
	GLuint location_projectionMatrix;
	GLuint location_reflectionTexture;
	GLuint location_refractionTexture;
	GLuint location_dudvMap;
	GLuint location_moveFactor;
	GLuint location_cameraPosition;
	GLuint location_normalMap;
	GLuint location_lightColor[5];
	GLuint location_lightPosition[5];
	GLuint location_attenuation[5];
	GLuint location_depthMap;

public:
	WaterShader():ShaderProgram("waterVertexShader.glsl", "waterFragmentShader.glsl") {
		getAllUniformLocations();
	}
	inline void const loadModelMatrix(glm::mat4 matrix) {
		ShaderProgram::loadMatrix(location_modelMatrix, matrix);}	
	inline void const loadProjectionMatrix(Camera camera) {
		mat4 projectionMatrix = camera.computeProjectionMatrix();
		ShaderProgram::loadMatrix(location_projectionMatrix, projectionMatrix);}
	inline void const loadViewMatrix(Camera camera) {
		mat4 viewMatrix = camera.computeViewMatrix();
		ShaderProgram::loadMatrix(location_viewMatrix, viewMatrix);
		ShaderProgram::loadVector(location_cameraPosition, camera.getPosition());
	}
	inline void const connectTextureUnits() {
		ShaderProgram::loadInt(location_reflectionTexture, 0);
		ShaderProgram::loadInt(location_refractionTexture, 1);
		ShaderProgram::loadInt(location_dudvMap, 2);
		ShaderProgram::loadInt(location_normalMap, 3);
		ShaderProgram::loadInt(location_depthMap, 4);
	}
	inline void const loadMoveFactor(float factor) {
		ShaderProgram::loadFloat(location_moveFactor, factor);
	}
	inline void const loadLights(std::vector<Light*> lights) {
		for (int i = 0; i < 5; i++) {
			if (i < lights.size()) {
				ShaderProgram::loadVector(location_lightPosition[i], lights[i]->getPosition());
				ShaderProgram::loadVector(location_lightColor[i], lights[i]->getColor());
				ShaderProgram::loadVector(location_attenuation[i], lights[i]->getAttenuation());
			}
			else {
				ShaderProgram::loadVector(location_lightPosition[i], vec3(0.0));
				ShaderProgram::loadVector(location_lightColor[i], vec3(0.0));
				ShaderProgram::loadVector(location_attenuation[i], vec3(0.0));
			}
		}
	}
protected:
	
	void bindAttributes() override{
		bindAttribute(0, "position");
	}

	void getAllUniformLocations() override{
		location_projectionMatrix = getUniformLocation("projectionMatrix");
		location_viewMatrix = getUniformLocation("viewMatrix");
		location_modelMatrix = getUniformLocation("modelMatrix");
		location_reflectionTexture = getUniformLocation("reflectionTexture");
		location_refractionTexture = getUniformLocation("refractionTexture");
		location_dudvMap = getUniformLocation("dudvMap");
		location_moveFactor = getUniformLocation("moveFactor");
		location_cameraPosition = getUniformLocation("cameraPosition");
		location_normalMap = getUniformLocation("normalMap");
		location_depthMap = getUniformLocation("depthMap");
		for (int i = 0; i < 5; i++) {
			location_lightPosition[i] = ShaderProgram::getUniformLocation("lightPosition[" + std::to_string(i) + "]");
			location_lightColor[i] = ShaderProgram::getUniformLocation("lightColor[" + std::to_string(i) + "]");
			location_attenuation[i] = ShaderProgram::getUniformLocation("attenuation[" + std::to_string(i) + "]");
		}
	}

};





#endif