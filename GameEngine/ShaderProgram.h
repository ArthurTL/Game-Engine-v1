#ifndef __SPROGRAM_H__
#define __SPROGRAM_H__
#include <glad/glad.h>
#include <vec3.hpp>
#include <mat4x4.hpp>
#include <gtc/type_ptr.hpp>
#include "Camera.h"
#include "Light.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

using namespace std;

class ShaderProgram {

public:
	ShaderProgram(const string& vertexFile, const string& fragmentFile);
	void start();
	void stop();
	void cleanUp();
	
	virtual ~ShaderProgram();

private:
	GLuint programID;
	GLuint vertexShaderID;
	GLuint fragmentShaderID;

	static GLuint loadShader(const string &file, GLuint type);
	static string file2String(const string& filename);
protected:

	void bindAttribute(GLuint attribute, const string& variableName);
	virtual void bindAttributes()=0;
	virtual void getAllUniformLocations() {};
	GLuint getUniformLocation(const string& uniformName);
	void loadInt(GLuint location, int value);
	void loadFloat(GLuint location, float value);
	void loadVector(GLuint location, glm::vec3 vector);
	void loadBoolean(GLuint location, bool value);
	void loadMatrix(GLuint location, glm::mat4 matrix);
	void load2DVector(GLuint location, glm::vec2 vector);
	void load4DVector(GLuint location, glm::vec4 vector);
};
#endif

