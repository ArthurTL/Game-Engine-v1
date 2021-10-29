#include "ShadowFrameBuffer.h"


void ShadowFrameBuffer::initialiseFrameBuffer() {
	fbo = createFrameBuffer();
	shadowMap = createDepthBufferAttachment(WIDTH, HEIGHT);
	unbindFrameBuffer();
}


GLuint ShadowFrameBuffer::createFrameBuffer() {
	GLuint frameBuffer;
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	return frameBuffer;
}

GLuint ShadowFrameBuffer::createDepthBufferAttachment(GLuint width, GLuint height){
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, width, height, 0,
		GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,texture, 0);
	return texture;
}
void ShadowFrameBuffer::bindFrameBuffer(){
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glViewport(0, 0, WIDTH, HEIGHT);
}
void ShadowFrameBuffer::unbindFrameBuffer(){
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, WindowManager::WIDTH, WindowManager::HEIGHT);
}

void  ShadowFrameBuffer::cleanUp() {
	glDeleteFramebuffers(1,&fbo);
	glDeleteTextures(1,&shadowMap);
}