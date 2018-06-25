#ifndef DISPLAY_H
#define DISPLAY_H

GLFWwindow* display_createDisplay(int width, int height);

void display_tick();
void display_destroy();

int display_isWindowClosing();

#endif