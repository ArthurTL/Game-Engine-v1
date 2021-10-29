#ifndef __GUISHADER_H__
#define __GUISHADER_H__
#include "ShaderProgram.h"

class GuiShader : public ShaderProgram {

private:
	GLuint location_transfomationMatrix=0;

public:
	GuiShader() :ShaderProgram("guiVertexShader.glsl", "guiFragmentShader.glsl") { getAllUniformLocations(); };
	inline void const loadTransformationMatrix(glm::mat4 matrix) { 
		ShaderProgram::loadMatrix(location_transfomationMatrix, matrix);}

protected:
	inline void getAllUniformLocations() override { 
		location_transfomationMatrix = ShaderProgram::getUniformLocation("transformationMatrix");}
	inline void bindAttributes() override {
		ShaderProgram::bindAttribute(0, "position");
	}
};










#endif
