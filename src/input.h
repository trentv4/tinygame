#ifndef INPUT_H
#define INPUT_H

#include "display.h"

typedef struct Input {
	int mousePosX;
	int mousePosY;
	int mouseWheelPos;
	int bind_moveForwardCount;
	int bind_moveBackwardCount;
	int bind_moveLeftCount;
	int bind_moveRightCount;
} Input;

typedef struct Binds {
	int bind_moveForward;
	int bind_moveBackward;
	int bind_moveLeft;
	int bind_moveRight;
} Binds;

Binds* input_getBinds();
Input* input_getInput();
void input_update(Display* window);

#endif