#ifndef __SHADOWSHADER_H__
#define __SHADOWSHADER_H__
#include "ShaderProgram.h"


class ShadowShader: public ShaderProgram {
	
private:
	GLuint location_mvpMatrix =0;

public:
	ShadowShader() :ShaderProgram("shadowVertexShader.glsl", "shadowFragmentShader.glsl") { getAllUniformLocations(); }

	inline void loadMvpMatrix(mat4 mvpMatrix) {
		ShaderProgram::loadMatrix(location_mvpMatrix, mvpMatrix);
	}
	inline void bindAttributes() override { 
		ShaderProgram::bindAttribute(0, "in_position"); 
		ShaderProgram::bindAttribute(1, "in_textureCoords");
	}
	inline void getAllUniformLocations() override {
		location_mvpMatrix = ShaderProgram::getUniformLocation("mvpMatrix");
	}
};







#endif
