#ifndef __RAWMODEL_H__
#define __RAWMODEL_H__

#include <glad/glad.h>
#include <GLFW\glfw3.h>


class RawModel {

public:
	RawModel(GLuint vaoID_, GLuint vertexCount_) : vaoID(vaoID_), vertexCount(vertexCount_) {};
	GLuint vaoID;
	GLuint vertexCount;
};
#endif
