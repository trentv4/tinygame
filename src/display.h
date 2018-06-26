#ifndef DISPLAY_H
#define DISPLAY_H

typedef struct Display {
	GLFWwindow* window;
	int width;
	int height;
} Display;

Display* display_createDisplay(int width, int height);

void display_tick();
void display_destroy();
void display_drawRectangle(float r, float g, float b, int x, int y, int x2, int y2);

int display_isWindowClosing();

#endif