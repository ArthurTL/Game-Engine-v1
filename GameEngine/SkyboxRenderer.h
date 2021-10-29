#ifndef __SKYRENDERER_H__
#define __SKYRENDERER_H_
#include "Loader.h"
#include "Camera.h"
#include "SkyboxShader.h"

class SkyboxRenderer {

private:
	static const float SIZE;
	static std::vector<float> VERTICES;
	static const std::vector<string> TEXTURE_FILES;
	static const std::vector<string> NIGHT_TEXTURE_FILES;
	RawModel* cube;
	GLuint texture;
	GLuint nightTexture;
	SkyboxShader* shader;
	float time = 15000.;
	void bindTextures();
public:

	SkyboxRenderer(Loader loader, Camera camera);

	void render(Camera camera, float r, float g, float b);
	inline SkyboxShader* getShader() const{ return shader; }
	inline float getTime() const { return time; }
	void cleanUp();
};


#endif
