#include <stdio.h>  
#include "GLFW/glfw3.h"

int main()  
{  
	printf("Program started\n");
	if(!glfwInit())
	{
		printf("GLFW was not initialized.\n");
	}
	printf("GLFW initialized.\n");
    return 0;
}   
