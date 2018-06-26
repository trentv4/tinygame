#include <stdio.h>
#include <stdlib.h>
#include "GLFW/glfw3.h"
#include "display.h"

void callback_key(GLFWwindow* window, int key, int scancode, int action, int mods) 
{

}

Display* display_createDisplay(int width, int height)
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

	Display* display = malloc(sizeof(Display));
	display-> window = window;
	display-> width = width;
	display-> height = height;
	return display;
}

void display_tick(Display* display)
{
	glfwSwapBuffers(display-> window);
	glfwPollEvents();
	glfwGetFramebufferSize(display-> window, &(display-> width), &(display-> height));
	glViewport(0, 0, display-> width, display-> height);
	glClear(GL_COLOR_BUFFER_BIT);
}

void display_destroy(Display* display)
{
	glfwDestroyWindow(display-> window);
	glfwTerminate();
}

void display_drawRectangle(float r, float g, float b, int x, int y, int x2, int y2)
{
	glPushMatrix();
	glColor4f(r, g, b, 1);
	glTranslated(-1, -1, 0);
	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2f(0, 0);
	glVertex2d(0, 0);
	glTexCoord2f(1, 0);
	glVertex2d(x2, 0);
	glTexCoord2f(0, 1);
	glVertex2d(0, y2);
	glTexCoord2f(1, 1);
	glVertex2d(x2, y2);
	glEnd();
	glPopMatrix();
}

int display_isWindowClosing(Display* display)
{
	return glfwWindowShouldClose(display-> window);
}