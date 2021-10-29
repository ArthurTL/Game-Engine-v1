#ifndef __NRENDERER_H__
#define __NRENDERER_H__

//#include <GL\glew.h>
#include "Entity.h"
#include "NormalShader.h"
#include "Maths.h"
#include <map>
#include <vector>




class NormalRenderer {

private:
	NormalShader* shader;

	void prepareTexturedModel(TexturedModel model);
	void unbindTexturedModel();
	void prepareInstance(Entity entity);

public:
	NormalRenderer(NormalShader* shader_);

	void render(std::map<TexturedModel*, std::vector<Entity*>>entities, glm::mat4 toShadowMapSpace);

};
#endif



