#include "WindowManager.h"
float WindowManager::delta = 0.;
double WindowManager::lastFrameTime = 0.;
GLuint WindowManager::WIDTH = 1600;
GLuint WindowManager::HEIGHT = 900;

GLFWwindow* WindowManager::createWindow() {

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	
	GLFWwindow* window = glfwCreateWindow(WindowManager::WIDTH, WindowManager::HEIGHT, "Fox World", NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	WindowManager::lastFrameTime = getCurrentTime();

	return window;
};

void WindowManager::updateWindow(GLFWwindow* window) {
	glfwSwapBuffers(window);
	glfwPollEvents();
	double currentFrameTime = getCurrentTime();
	WindowManager::delta = (currentFrameTime - lastFrameTime)/1000.;
	WindowManager::lastFrameTime = currentFrameTime;
}

void WindowManager::closeWindow(GLFWwindow* window) {
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

double WindowManager::getCurrentTime()
{
	return glfwGetTime()*1000.;
}
