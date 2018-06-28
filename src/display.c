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

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	// if needed, glfwWindowHint glfw_context_version_maj/min 2 for min version
	GLFWwindow* window = glfwCreateWindow(width, height, "Window", NULL, NULL);
	if(!window) printf("Unable to create window");
	printf("Window created.");

	glfwMakeContextCurrent(window);
	//glad goes here to load extensions if needed

	glMatrixMode(GL_PROJECTION);
	glDisable(GL_DEPTH_TEST);
	glOrtho(0, width, height, 0, 1, -1);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glMatrixMode(GL_MODELVIEW);

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
	glBegin(GL_TRIANGLE_STRIP);

	glTexCoord2f(0, 0);
	glVertex2d(x, y);

	glTexCoord2f(1, 0);
	glVertex2d(x + x2, y);

	glTexCoord2f(0, 1);
	glVertex2d(x, y + y2);

	glTexCoord2f(1, 1);
	glVertex2d(x + x2, y + y2);

	glEnd();
	glPopMatrix();
}

int display_isWindowClosing(Display* display)
{
	return glfwWindowShouldClose(display-> window);
}