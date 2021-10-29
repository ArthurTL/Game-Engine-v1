#ifndef __TSHADER_H__
#define __TSHADER_H__
#include "ShaderProgram.h"


class TerrainShader : public ShaderProgram{

private:
	static const int MAX_LIGHTS = 5;
	static const string& VERTEX_FILE;
	static const string& FRAGMENT_FILE;
	GLuint location_transformationMatrix;
	GLuint location_projectionMatrix;
	GLuint location_viewMatrix;
	GLuint location_lightPosition[MAX_LIGHTS];
	GLuint location_lightColor[MAX_LIGHTS];
	GLuint location_attenuation[MAX_LIGHTS];
	GLuint location_shineDamper;
	GLuint location_reflectivity;
	GLuint location_skyColor;
	GLuint location_backgroundTexture;
	GLuint location_rTexture;
	GLuint location_gTexture;
	GLuint location_bTexture;
	GLuint location_blendMap;
	GLuint location_blendFactor;
	GLuint location_plane;
	GLuint location_toShadowMapSpace;
	GLuint location_shadowMap;
public:
	TerrainShader() :
		ShaderProgram(VERTEX_FILE, FRAGMENT_FILE) {
		getAllUniformLocations();
	};


	void loadTransformationMatrix(glm::mat4);
	void loadProjectionMatrix(Camera camera);
	void loadViewMatrix(Camera camera);
	void loadLights(std::vector<Light*> lights);
	void loadShineVariables(float damper, float reflectivity);
	void const loadBlendFactor(float blend);
	void loadSkyColor(float r, float g, float b);
	void loadClipPlane(glm::vec4 plane);
	void loadToShadowMapSpace(glm::mat4 matrix);

	void connectTextureUnits();
protected:
	void bindAttributes() override;
	void getAllUniformLocations() override;


};






#endif
