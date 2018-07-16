#include <stdio.h>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "texture.h"
#include "object.h"

Object* object_buildObject(GLfloat vertexBufferData[], int vertexCount, GLfloat uvBufferData[], int uvCount, char* texturePath)
{
	// Create the array object (holds buffers)
	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	GLuint vertexBufferID;
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertexCount, vertexBufferData, GL_STATIC_DRAW);

	GLuint uvBufferID;
	glGenBuffers(1, &uvBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
	glBufferData(GL_ARRAY_BUFFER, uvCount, uvBufferData, GL_STATIC_DRAW);

	Object* object = malloc(sizeof(Object));
	object-> vertexBufferID = vertexBufferID;
	object-> uvBufferID = uvBufferID;
	object-> textureID = texture_loadDDS(texturePath);
	object-> triangleCount = vertexCount / 12;

	printf("Created object with texture \"%s\" with %d triangles (%d vertices)\n", 
		texturePath, object-> triangleCount, vertexCount);
	return object;
}