#ifndef __GUITEXTURE_H__
#define __GUITEXTURE_H_
#include "GLFW/glfw3.h"
#include "glm.hpp"

class GuiTexture {

private:
	GLuint texture;
	glm::vec2 position;
	glm::vec2 scale;
public:

	GuiTexture(GLuint texture_, glm::vec2 position_, glm::vec2 scale_) : texture(texture_), position(position_), scale(scale_) {};
	inline GLuint getTexture() const { return texture; }
	inline glm::vec2 getPosition() const { return position; }
	inline glm::vec2 getScale() const { return scale; }
};











#endif
