#include <stdio.h>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "display.h"
#include "input.h"

static Input* globalInput;
static Binds* globalBinds;

// note: not currently set to handle dTime
// note: not yet implemented mouseWheelPos
void input_update(Display* display)
{
	if(globalInput == NULL)
	{
		globalInput = malloc(sizeof(Input));
		globalInput-> mousePosX = 0;
		globalInput-> mousePosY = 0;
		globalInput-> mouseWheelPos = 0;
		globalInput-> bind_moveForwardCount = 0;
		globalInput-> bind_moveBackwardCount = 0;
		globalInput-> bind_moveLeftCount = 0;
		globalInput-> bind_moveRightCount = 0;
	}

	if(globalBinds == NULL)
	{
		globalBinds = malloc(sizeof(Binds));
		globalBinds-> bind_moveForward = GLFW_KEY_W;
		globalBinds-> bind_moveBackward = GLFW_KEY_A;
		globalBinds-> bind_moveLeft = GLFW_KEY_S;
		globalBinds-> bind_moveRight = GLFW_KEY_D;
	}

	double tempX = 0, tempY = 0;
	glfwGetCursorPos(display-> window, &tempX, &tempY);
	globalInput-> mousePosX = (int) (tempX - (display-> width/2));
	globalInput-> mousePosY = (int) (tempY - (display-> height/2));
	glfwSetCursorPos(display-> window, display-> width/2, display-> height/2);

	globalInput-> bind_moveForwardCount = (glfwGetKey(display-> window, globalBinds-> bind_moveForward) == GLFW_PRESS) ? globalInput-> bind_moveForwardCount + 1 : 0;
	globalInput-> bind_moveBackwardCount = (glfwGetKey(display-> window, globalBinds-> bind_moveBackward) == GLFW_PRESS) ? globalInput-> bind_moveBackwardCount + 1 : 0;
	globalInput-> bind_moveLeftCount = (glfwGetKey(display-> window, globalBinds-> bind_moveLeft) == GLFW_PRESS) ? globalInput-> bind_moveLeftCount + 1 : 0;
	globalInput-> bind_moveRightCount = (glfwGetKey(display-> window, globalBinds-> bind_moveRight) == GLFW_PRESS) ? globalInput-> bind_moveRightCount + 1 : 0;
}

Binds* input_getBinds()
{
	return globalBinds;
}

Input* input_getInput()
{
	return globalInput;
}