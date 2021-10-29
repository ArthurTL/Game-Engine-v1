#ifndef __NSHADER_H__
#define __NSHADER_H__
#include "ShaderProgram.h"


class NormalShader: public ShaderProgram {
private:

	static const int MAX_LIGHTS = 5;
	static const string& VERTEX_FILE;
	static const string& FRAGMENT_FILE;
	GLuint location_transformationMatrix;
	GLuint location_projectionMatrix;
	GLuint location_viewMatrix;
	GLuint location_lightPositionEyeSpace[MAX_LIGHTS];
	GLuint location_lightColor[MAX_LIGHTS];
	GLuint location_attenuation[MAX_LIGHTS];
	GLuint location_shineDamper;
	GLuint location_reflectivity;
	GLuint location_ambiantColor;
	GLuint location_skyColor;
	GLuint location_offset;
	GLuint location_blendFactor;
	GLuint location_modelTexture=0;
	GLuint location_normalMap=0;
	GLuint location_plane;
	GLuint location_toShadowMapSpace;
	GLuint location_shadowMap;

public:
	NormalShader() :
		ShaderProgram(VERTEX_FILE, FRAGMENT_FILE) {
		getAllUniformLocations();
	};


	void loadTransformationMatrix(glm::mat4);
	void loadToShadowMapSpace(glm::mat4);
	void loadProjectionMatrix(Camera camera);
	void loadViewMatrix(Camera camera);
	void connectTextureUnits();
	void loadShineVariables(float damper, float reflectivity, glm::vec3 Ka);
	void loadSkyColor(float r, float g, float b);
	void loadOffset(float x, float y);
	void loadBlendFactor(float b);
	void loadLights(std::vector<Light*>, Camera camera);
	void loadClipPlane(glm::vec4 plane);
	glm::vec3 getEyeSpacePosition(Light* light, glm::mat4 viewMatrix);


protected:
	
	
	void bindAttributes() override;
	void getAllUniformLocations() override;
};
#endif