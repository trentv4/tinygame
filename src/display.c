#include <stdio.h>  
#include "GLFW/glfw3.h"

void callback_key(GLFWwindow* window, int key, int scancode, int action, int mods) 
{

}

GLFWwindow* display_createDisplay(int width, int height)
{
	if(!glfwInit()) printf("GLFW was not initialized.\n");

	printf("GLFW initialized.\n");

	// if needed, glfwWindowHint glfw_context_version_maj/min 2 for min version
	GLFWwindow* window = glfwCreateWindow(width, height, "Window", NULL, NULL);
	if(!window) printf("Unable to create window");
	printf("Window created.");

	glfwMakeContextCurrent(window);
	//glad goes here to load extensions if needed

	// callbacks
	glfwSetKeyCallback(window, callback_key);
	glfwSwapInterval(1);

	return window;
}

void display_tick(GLFWwindow* window, int width, int height)
{
	glfwSwapBuffers(window);
	glfwPollEvents();
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT);
}

void display_destroy(GLFWwindow* window)
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

int display_isWindowClosing(GLFWwindow* window)
{
	return glfwWindowShouldClose(window);
}