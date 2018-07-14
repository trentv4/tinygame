#ifndef TEXTURE_H
#define TEXTURE_H

#include "GLFW/glfw3.h"

typedef struct Texture {
	int width;
	int height;
} Texture;

GLuint texture_loadDDS(char* path);
#endif