#ifndef __RENDERER_H__
#define __RENDERER_H__

//#include <GL\glew.h>
#include "Entity.h"
#include "StaticShader.h"
#include "Maths.h"
#include <map>
#include <vector>




class EntityRenderer{

private:
	StaticShader *shader;

	void prepareTexturedModel(TexturedModel model);
	void unbindTexturedModel();
	void prepareInstance(Entity entity);
	
public:
	EntityRenderer(StaticShader *shader_);
	
	void render(std::map<TexturedModel*, std::vector<Entity*>>entities, mat4 toShadowSpace);

};
#endif


