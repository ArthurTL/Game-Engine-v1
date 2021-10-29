#ifndef __WATERFBUFFERS_H__
#define __WATERFBUFFERS_H__

#include <glad/glad.h>
#include "WindowManager.h"

class WaterFrameBuffers {

private:
	static const int REFLECTION_WIDTH = 320;
	static const int REFLECTION_HEIGHT = 180;
	static const int REFRACTION_WIDTH = 1600;
	static const int REFRACTION_HEIGHT = 900;
	GLuint reflectionFrameBuffer;
	GLuint reflectionTexture;
	GLuint reflectionDepthBuffer;
	GLuint refractionFrameBuffer;
	GLuint refractionTexture;
	GLuint refractionDepthTexture;

	void initialiseReflectionFrameBuffer();
	void initialiseRefractionFrameBuffer();
	void bindFrameBuffer(GLuint frameBuffer, GLuint width,GLuint height);
	GLuint createFrameBuffer();
	GLuint createTextureAttachment(GLuint width, GLuint height);
	GLuint createDepthTextureAttachment(GLuint width, GLuint height);
	GLuint createDepthBufferAttachment(GLuint width, GLuint height);
public:
	WaterFrameBuffers();
	void cleanUp();
	void bindReflectionFrameBuffer();
	void bindRefractionFrameBuffer();
	void unbindCurrentFrameBuffer();
	inline GLuint getReflectionTexture() const { return reflectionTexture; }
	inline GLuint getRefractionTexture() const { return refractionTexture; }
	inline GLuint getRefractionDepthTexture() const { return refractionDepthTexture; }


};

#endif