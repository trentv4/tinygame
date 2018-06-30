#include <stdio.h>  
#include "display.h"

int main()  
{  
	printf("Program started\n");
	Display* display = display_createDisplay(1280, 720);
	
	while(!display_isWindowClosing(display))
	{
		display_tick(display);
	}

	display_destroy(display);
	return 0;
}
