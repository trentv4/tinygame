#include <stdio.h>  
#include "GLFW/glfw3.h"
#include "display.h"

int main()  
{  
	printf("Program started\n");
	GLFWwindow* window = display_createDisplay(1280, 720);

	while(!display_isWindowClosing(window))
	{
		display_tick(window, 1280, 720);
	}

	display_destroy();
	return 0;
}
