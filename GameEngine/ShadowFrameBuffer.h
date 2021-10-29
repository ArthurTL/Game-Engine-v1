#ifndef __SHADOWFBUFFER_H__
#define __SHADOWFBUFFER_H__

#include <glad/glad.h>
#include "WindowManager.h"

class ShadowFrameBuffer {

private:
	const GLuint WIDTH;
	const GLuint HEIGHT;
	GLuint fbo;
	GLuint shadowMap;

	void initialiseFrameBuffer();
	static GLuint createFrameBuffer();
	static GLuint createDepthBufferAttachment(GLuint width, GLuint height);

public:
	ShadowFrameBuffer(GLuint width, GLuint height) : WIDTH(width), HEIGHT(height) { initialiseFrameBuffer(); }
	void unbindFrameBuffer();
	void bindFrameBuffer();
	inline GLuint getShadowMap() const { return shadowMap; }
	void cleanUp();
};





#endif
