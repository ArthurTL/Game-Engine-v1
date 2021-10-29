#ifndef __SSHADER_H__
#define __SSHADER_H__
#include "ShaderProgram.h"


class StaticShader : public ShaderProgram {
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
	GLuint location_ambiantColor;
	GLuint location_useFakeLighting;
	GLuint location_skyColor;
	GLuint location_numberOfRows;
	GLuint location_offset;
	GLuint location_blendFactor;
	GLuint location_plane;
	GLuint location_toShadowMapSpace;
	GLuint location_shadowMap;

public:
	StaticShader() :
		ShaderProgram(VERTEX_FILE, FRAGMENT_FILE) {
		getAllUniformLocations();
	};


	void loadTransformationMatrix(glm::mat4);
	void loadProjectionMatrix(Camera camera);
	void loadViewMatrix(Camera camera);
	void loadToShadowMapSpace(mat4 matrix);
	void loadLights(std::vector<Light*> lights);
	void loadShineVariables(float damper, float reflectivity, glm::vec3 Ka);
	void loadFakeLightingVariable(bool useFake);
	void loadSkyColor(float r, float g, float b);
	void loadNumberOfRows(int numberOfRows);
	void loadOffset(float x, float y);
	void loadClipPlane(glm::vec4 plane);
	void loadBlendFactor(float b);
	void connectTextureUnits();
protected:
	void bindAttributes() override;
	void getAllUniformLocations() override;
};
#endif
