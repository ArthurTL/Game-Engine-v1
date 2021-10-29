#ifndef __TTEXTURE_H__
#define __TTEXTURE_H__
#include "GLFW/glfw3.h"

class TerrainTexture {

private:
	const GLuint textureID;

public:
	TerrainTexture(GLuint textureID_) : textureID(textureID_) {};
	inline GLuint getTextureID() const { return textureID; };
};






#endif
