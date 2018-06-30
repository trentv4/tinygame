#ifndef DISPLAY_H
#define DISPLAY_H

#include "GLFW/glfw3.h"

typedef struct Display {
	GLFWwindow* window;
	int width;
	int height;
	GLuint programID;
} Display;

Display* display_createDisplay(int width, int height);

void display_tick();
void display_destroy();

int display_isWindowClosing();

#endif