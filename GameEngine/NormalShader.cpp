#include "NormalShader.h"


const string& NormalShader::VERTEX_FILE = "normalVertexShader.glsl";
const string& NormalShader::FRAGMENT_FILE = "normalFragmentShader.glsl";

void NormalShader::bindAttributes() {
	ShaderProgram::bindAttribute(0, "position");
	ShaderProgram::bindAttribute(1, "vtextureCoords");
	ShaderProgram::bindAttribute(2, "normals");
	ShaderProgram::bindAttribute(3, "tangent");

}

void NormalShader::loadTransformationMatrix(glm::mat4 matrix) {
	ShaderProgram::loadMatrix(location_transformationMatrix, matrix);
}

void NormalShader::loadToShadowMapSpace(glm::mat4 matrix) {
	ShaderProgram::loadMatrix(location_toShadowMapSpace, matrix);
}


void NormalShader::loadProjectionMatrix(Camera camera) {
	mat4 projectionMatrix = camera.computeProjectionMatrix();
	ShaderProgram::loadMatrix(location_projectionMatrix, projectionMatrix);
}

void NormalShader::loadViewMatrix(Camera camera) {
	mat4 viewMatrix = camera.computeViewMatrix();
	ShaderProgram::loadMatrix(location_viewMatrix, viewMatrix);
}
void NormalShader::loadLights(std::vector<Light*> lights, Camera camera) {
	mat4 viewMatrix = camera.computeViewMatrix();
	for (int i = 0; i < MAX_LIGHTS; i++) {
		if (i < lights.size()) {
			ShaderProgram::loadVector(location_lightPositionEyeSpace[i], getEyeSpacePosition(lights[i],viewMatrix));
			ShaderProgram::loadVector(location_lightColor[i], lights[i]->getColor());
			ShaderProgram::loadVector(location_attenuation[i], lights[i]->getAttenuation());
		}
		else {
			ShaderProgram::loadVector(location_lightPositionEyeSpace[i], vec3(0.0));
			ShaderProgram::loadVector(location_lightColor[i], vec3(0.0));
			ShaderProgram::loadVector(location_attenuation[i], vec3(1., 0., 0.));
		}
	}
}

void NormalShader::loadShineVariables(float damper, float reflectivity, glm::vec3 ambiantColor) {
	ShaderProgram::loadFloat(location_shineDamper, damper);
	ShaderProgram::loadFloat(location_reflectivity, reflectivity);
	ShaderProgram::loadVector(location_ambiantColor, ambiantColor);

}


void NormalShader::loadSkyColor(float r, float g, float b) {
	ShaderProgram::loadVector(location_skyColor, vec3(r, g, b));
}

void NormalShader::loadOffset(float x, float y) {
	ShaderProgram::load2DVector(location_offset, vec2(x, y));
}
void NormalShader::loadBlendFactor(float blend) {
	ShaderProgram::loadFloat(location_blendFactor, blend);
}
void NormalShader::loadClipPlane(glm::vec4 plane) {
	ShaderProgram::load4DVector(location_plane, plane);
}

void NormalShader::getAllUniformLocations() {
	location_transformationMatrix = ShaderProgram::getUniformLocation("transformationMatrix");
	location_projectionMatrix = ShaderProgram::getUniformLocation("projectionMatrix");
	location_viewMatrix = ShaderProgram::getUniformLocation("viewMatrix");
	location_shineDamper = ShaderProgram::getUniformLocation("shineDamper");
	location_reflectivity = ShaderProgram::getUniformLocation("reflectivity");
	location_ambiantColor = ShaderProgram::getUniformLocation("ambiantColor");
	location_skyColor = ShaderProgram::getUniformLocation("skyColor");
	location_offset = ShaderProgram::getUniformLocation("offset");
	location_blendFactor = ShaderProgram::getUniformLocation("blendFactor");
	location_normalMap = ShaderProgram::getUniformLocation("normalMap");
	location_modelTexture = ShaderProgram::getUniformLocation("modelTexture");
	location_plane = ShaderProgram::getUniformLocation("plane");
	location_shadowMap = ShaderProgram::getUniformLocation("shadowMap");
	location_toShadowMapSpace = ShaderProgram::getUniformLocation("toShadowMapSpace");

	for (int i = 0; i < MAX_LIGHTS; i++) {
		location_lightPositionEyeSpace[i] = ShaderProgram::getUniformLocation("lightPositionEyeSpace[" + std::to_string(i) + "]");
		location_lightColor[i] = ShaderProgram::getUniformLocation("lightColor[" + std::to_string(i) + "]");
		location_attenuation[i] = ShaderProgram::getUniformLocation("attenuation[" + std::to_string(i) + "]");
	}

}
glm::vec3 NormalShader::getEyeSpacePosition(Light* light, glm::mat4 viewMatrix)
{
	glm::vec3 position = light->getPosition();
	glm::vec4 eyeSpacePos = glm::vec4(position, 1.0);
	eyeSpacePos = viewMatrix * eyeSpacePos;

	return glm::vec3(eyeSpacePos.x,eyeSpacePos.y,eyeSpacePos.z);
}

void NormalShader::connectTextureUnits() {
	ShaderProgram::loadInt(location_modelTexture, 0);
	ShaderProgram::loadInt(location_normalMap, 1);
	ShaderProgram::loadInt(location_shadowMap, 5);

}