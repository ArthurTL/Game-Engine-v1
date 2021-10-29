#ifndef __SHADOWMRENDERER_H__
#define __SHADOWMRENDERER_H__
#include "ShadowMapEntityRenderer.h"
#include "ShadowFrameBuffer.h"
#include "ShadowBox.h"

class ShadowMapMasterRenderer {

private:
	static const int SHADOW_MAP_SIZE = 4096;

	ShadowFrameBuffer* shadowFbo;
	ShadowShader* shader;
	ShadowBox* shadowBox;
	ShadowMapEntityRenderer* entityRenderer;
	mat4 projectionMatrix= mat4(1.f);
	mat4* lightViewMatrix= new mat4(1.f);
	mat4* projectionViewMatrix= new mat4(1.f);


	void prepare(vec3 lightDirection, ShadowBox* box);
	void updateOrthoProjectionMatrix(float width, float height, float length,float width2, float height2, float length2);
	void updateLightViewMatrix(vec3 direction, vec3 center);
	
	void finish();

public:
	ShadowMapMasterRenderer(Camera* camera);
	void render(std::map<TexturedModel*, std::vector<Entity*>> entities, std::map<TexturedModel*, std::vector<Entity*>> normalEntities, Light sun,Camera* camera);
	inline mat4 getToShadowMapSpaceMatrix() { return  *projectionViewMatrix; }
	inline GLuint getShadowMap() const { return shadowFbo->getShadowMap(); }
	inline mat4* getLightSpaceTransform() { return lightViewMatrix; }
	void cleanUp();
};



#endif