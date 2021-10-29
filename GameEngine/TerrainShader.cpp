#include "TerrainShader.h"

const string& TerrainShader::VERTEX_FILE = "terrainVertexShader.glsl";
const string& TerrainShader::FRAGMENT_FILE = "terrainFragmentShader.glsl";

void TerrainShader::bindAttributes() {
	ShaderProgram::bindAttribute(0, "position");
	ShaderProgram::bindAttribute(1, "vtextureCoords");
	ShaderProgram::bindAttribute(2, "normals");

}

void TerrainShader::loadTransformationMatrix(glm::mat4 matrix) {
	ShaderProgram::loadMatrix(location_transformationMatrix, matrix);
}
void TerrainShader::loadToShadowMapSpace(glm::mat4 matrix) {
	ShaderProgram::loadMatrix(location_toShadowMapSpace, matrix);
}

void TerrainShader::loadProjectionMatrix(Camera camera) {
	mat4 projectionMatrix = camera.computeProjectionMatrix();
	ShaderProgram::loadMatrix(location_projectionMatrix, projectionMatrix);
}

void TerrainShader::loadViewMatrix(Camera camera) {
	mat4 viewMatrix = camera.computeViewMatrix();
	ShaderProgram::loadMatrix(location_viewMatrix, viewMatrix);
}
void TerrainShader::loadLights(std::vector<Light*> lights) {
	for (int i = 0; i < MAX_LIGHTS; i++) {
		if (i < lights.size()) {
			ShaderProgram::loadVector(location_lightPosition[i], lights[i]->getPosition());
			ShaderProgram::loadVector(location_lightColor[i], lights[i]->getColor());
			ShaderProgram::loadVector(location_attenuation[i], lights[i]->getAttenuation());
		}
		else {
			ShaderProgram::loadVector(location_lightPosition[i], vec3(0.0));
			ShaderProgram::loadVector(location_lightColor[i], vec3(0.0));
			ShaderProgram::loadVector(location_attenuation[i], vec3(1.0,0.0,0.0));
		}
	}
}

void TerrainShader::loadShineVariables(float damper, float reflectivity) {
	ShaderProgram::loadFloat(location_shineDamper, damper);
	ShaderProgram::loadFloat(location_reflectivity, reflectivity);

}

void const TerrainShader::loadBlendFactor(float blend)
{
	ShaderProgram::loadFloat(location_blendFactor, blend);
}

void TerrainShader::loadSkyColor(float r, float g, float b) {
	ShaderProgram::loadVector(location_skyColor, vec3(r, g, b));
}
void TerrainShader::loadClipPlane(glm::vec4 plane) {
	ShaderProgram::load4DVector(location_plane, plane);
}
void TerrainShader::connectTextureUnits() {
	ShaderProgram::loadInt(location_backgroundTexture, 0);
	ShaderProgram::loadInt(location_rTexture, 1);
	ShaderProgram::loadInt(location_gTexture, 2);
	ShaderProgram::loadInt(location_bTexture, 3);
	ShaderProgram::loadInt(location_blendMap, 4);
	ShaderProgram::loadInt(location_shadowMap, 5);
}


void TerrainShader::getAllUniformLocations() {
	location_transformationMatrix = ShaderProgram::getUniformLocation("transformationMatrix");
	location_projectionMatrix = ShaderProgram::getUniformLocation("projectionMatrix");
	location_viewMatrix = ShaderProgram::getUniformLocation("viewMatrix");
	location_shineDamper = ShaderProgram::getUniformLocation("shineDamper");
	location_reflectivity = ShaderProgram::getUniformLocation("reflectivity");
	location_skyColor = ShaderProgram::getUniformLocation("skyColor");
	location_backgroundTexture = ShaderProgram::getUniformLocation("backgroundTexture");
	location_rTexture = ShaderProgram::getUniformLocation("rTexture");
	location_gTexture = ShaderProgram::getUniformLocation("gTexture");
	location_bTexture = ShaderProgram::getUniformLocation("bTexture");
	location_blendMap = ShaderProgram::getUniformLocation("blendMap");
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
