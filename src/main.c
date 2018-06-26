#include <stdio.h>  
#include "GLFW/glfw3.h"
#include "display.h"

int main()  
{  
	printf("Program started\n");
	Display* display = display_createDisplay(1280, 720);

	while(!display_isWindowClosing(display))
	{
		display_tick(display);
		display_drawRectangle(0.5, 0, 0, 0, 0, 1, 1);
	}

	display_destroy(display);
	return 0;
}
