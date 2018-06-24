#include <stdio.h>  
#include "GLFW/glfw3.h"

int main()  
{  
	printf("Program started\n");
	if(!glfwInit()) printf("GLFW was not initialized.\n");

	printf("GLFW initialized.\n");

	// if needed, glfwWindowHint glfw_context_version_maj/min 2 for min version
	GLFWwindow* window = glfwCreateWindow(1280, 720, "Window", NULL, NULL);
	if(!window) printf("Unable to create window");
	printf("Window created.");

	glfwMakeContextCurrent(window);
	//glad goes here to load extensions if needed

	while(!glfwWindowShouldClose(window))
	{

	}

	glfwDestroyWindow(window);
	glfwTerminate();
    return 0;
}  