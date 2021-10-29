#ifndef __WM_H__
#define __WM_H__

#include <GLFW\glfw3.h>
#include <stdlib.h>

class WindowManager {

private:
	
	static const GLuint FPS_CAP = 120;
	static double lastFrameTime;
	static float delta;


public:
	static GLuint WIDTH;
	static GLuint HEIGHT;
	static GLFWwindow* createWindow();
	static void updateWindow(GLFWwindow*);
	static void closeWindow(GLFWwindow*);
	static double getCurrentTime();
	static inline float const getFrameTimeSeconds() { return delta; };
};
#endif
#pragma once
