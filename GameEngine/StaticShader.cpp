#include "StaticShader.h"


const string& StaticShader::VERTEX_FILE = "vertexShader.glsl";
const string& StaticShader::FRAGMENT_FILE = "fragmentShader.glsl";

void StaticShader::bindAttributes()  {
	ShaderProgram::bindAttribute(0, "position");
	ShaderProgram::bindAttribute(1, "vtextureCoords");
	ShaderProgram::bindAttribute(2, "normals");

}

void StaticShader::loadTransformationMatrix(glm::mat4 matrix){
	ShaderProgram::loadMatrix(location_transformationMatrix, matrix);
}
void StaticShader::loadToShadowMapSpace(glm::mat4 matrix) {
	ShaderProgram::loadMatrix(location_toShadowMapSpace, matrix);
}

void StaticShader::loadProjectionMatrix(Camera camera){
	mat4 projectionMatrix = camera.computeProjectionMatrix();
	ShaderProgram::loadMatrix(location_projectionMatrix, projectionMatrix);
}

void StaticShader::loadViewMatrix(Camera camera) {
	mat4 viewMatrix = camera.computeViewMatrix();
	ShaderProgram::loadMatrix(location_viewMatrix, viewMatrix);
}
void StaticShader::loadLights(std::vector<Light*> lights) {
	for (int i = 0; i < MAX_LIGHTS; i++) {
		if (i < lights.size()) {
			ShaderProgram::loadVector(location_lightPosition[i], lights[i]->getPosition());
			ShaderProgram::loadVector(location_lightColor[i], lights[i]->getColor());
			ShaderProgram::loadVector(location_attenuation[i], lights[i]->getAttenuation());
		}
		else {
			ShaderProgram::loadVector(location_lightPosition[i], vec3(0.0));
			ShaderProgram::loadVector(location_lightColor[i], vec3(0.0));
			ShaderProgram::loadVector(location_attenuation[i], vec3(1.,0.,0.));
		}
	}
}

void StaticShader::loadShineVariables(float damper, float reflectivity, glm::vec3 ambiantColor) {
	ShaderProgram::loadFloat(location_shineDamper, damper);
	ShaderProgram::loadFloat(location_reflectivity, reflectivity);
	ShaderProgram::loadVector(location_ambiantColor, ambiantColor);

}

void StaticShader::loadFakeLightingVariable(bool useFake) {
	ShaderProgram::loadBoolean(location_useFakeLighting, useFake);
}


void StaticShader::loadSkyColor(float r, float g, float b) {
	ShaderProgram::loadVector(location_skyColor, vec3(r, g, b));
}

void StaticShader::loadNumberOfRows(int numberOfRows) {
	ShaderProgram::loadFloat(location_numberOfRows, (float)numberOfRows);
}

void StaticShader::loadOffset(float x, float y) {
	ShaderProgram::load2DVector(location_offset, vec2(x,y));
}
void StaticShader::loadBlendFactor(float blend){
	ShaderProgram::loadFloat(location_blendFactor, blend);
}

void StaticShader::loadClipPlane(glm::vec4 plane) {
	ShaderProgram::load4DVector(location_plane, plane);
}
void StaticShader::connectTextureUnits() {
	ShaderProgram::loadInt(location_shadowMap, 5);
	
}

void StaticShader::getAllUniformLocations() {
	location_transformationMatrix = ShaderProgram::getUniformLocation("transformationMatrix");
	location_projectionMatrix = ShaderProgram::getUniformLocation("projectionMatrix");
	location_viewMatrix = ShaderProgram::getUniformLocation("viewMatrix");
	location_shineDamper = ShaderProgram::getUniformLocation("shineDamper");
	location_reflectivity = ShaderProgram::getUniformLocation("reflectivity");
	location_ambiantColor = ShaderProgram::getUniformLocation("ambiantColor");
	location_useFakeLighting = ShaderProgram::getUniformLocation("useFakeLighting");
	location_skyColor = ShaderProgram::getUniformLocation("skyColor");
	location_numberOfRows = ShaderProgram::getUniformLocation("numberOfRows");
	location_offset = ShaderProgram::getUniformLocation("offset");
	location_blendFactor = ShaderProgram::getUniformLocation("blendFactor");
	location_plane = ShaderProgram::getUniformLocation("plane");
	location_toShadowMapSpace = ShaderProgram::getUniformLocation("toShadowMapSpace");
	location_shadowMap = ShaderProgram::getUniformLocation("shadowMap");

	for (int i = 0; i < MAX_LIGHTS; i++) {
		location_lightPosition[i] = ShaderProgram::getUniformLocation("lightPosition[" + std::to_string(i) + "]");
		location_lightColor[i] = ShaderProgram::getUniformLocation("lightColor[" + std::to_string(i) + "]");
		location_attenuation[i] = ShaderProgram::getUniformLocation("attenuation[" + std::to_string(i) + "]");
	}
	
}