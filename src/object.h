#ifndef OBJECT_H
#define OBJECT_H

#include "GLFW/glfw3.h"

typedef struct Object {
	GLuint vertexBufferID;
	GLuint uvBufferID;
	GLuint textureID;
	int triangleCount;
} Object;

Object* object_buildObject(GLfloat vertexBufferData[], int vertexCount, GLfloat uvBufferData[], int uvCount, char* texturePath);
#endif