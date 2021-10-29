#ifndef __SHADOWERENDERER_H__
#define __SHADOWERENDERER_H__
#include "ShadowShader.h"
#include "Entity.h"
#include <map>

class ShadowMapEntityRenderer {

private:
	mat4* projectionViewMatrix;
	ShadowShader* shader;
	void bindModel(RawModel model);
	void prepareInstance(Entity entity);
public:
	
	ShadowMapEntityRenderer(ShadowShader * shader_, mat4* projectionViewMatrix_):
		shader(shader_), projectionViewMatrix(projectionViewMatrix_){}
	
	void render(std::map<TexturedModel*, std::vector<Entity*>> entities);



};



#endif