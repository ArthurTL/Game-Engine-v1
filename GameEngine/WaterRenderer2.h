#ifndef __WATERRENDERER2_H__
#define __WATERRENDERER2_H__

#include "WaterShader.h"
#include "Loader.h"
#include "WaterTile.h"
#include "Maths.h"
#include <map>
#include <vector>


class WaterRenderer {

private:
	static const string DUDV_MAP;
	static const string NORMAL_MAP;
	static const float WAVE_SPEED;
	GLint dudvTexture;
	GLuint normalMap;
	float moveFactor = 0;
	RawModel* quad;
	WaterShader* shader;
	WaterFrameBuffers* fbos;
	void prepareRender(Camera camera, std::vector<Light*> lights);
	void unbind();
	void setupVAO(Loader loader);

public:
	WaterRenderer(Loader loader, WaterShader* shader_, Camera camera,WaterFrameBuffers* fbos);
	void render(vector<WaterTile> water, Camera camera, std::vector<Light*> lights);


};






#endif