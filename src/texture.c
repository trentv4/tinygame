#include <stdio.h>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#define DXT1 0x31545844
#define DXT3 0x33545844
#define DXT5 0x35545844

#define GL_DXT1 GL_COMPRESSED_RGBA_S3TC_DXT1_EXT
#define GL_DXT3 GL_COMPRESSED_RGBA_S3TC_DXT3_EXT
#define GL_DXT5 GL_COMPRESSED_RGBA_S3TC_DXT5_EXT

GLuint texture_loadDDS(char* path)
{
	unsigned char header[124];
	FILE* file = fopen(path, "rb");
	if(file == NULL)
	{
		printf("File at path %s is unable to be opened.", path);
		exit(-1);
	}

	char filecode[4];
	fread(filecode, 1, 4, file);
	if(strncmp(filecode, "DDS ", 4) != 0)
	{
		fclose(file);
		printf("File at path %s is not a DDS file.", path);
		exit(-1);
	}

	// Read and interpret the file header (bytes 0 -> 124)
	fread(&header, 124, 1, file);

	unsigned int height = *(unsigned int*)&(header[8]);
	unsigned int width = *(unsigned int*)&(header[12]);
	unsigned int linearSize	= *(unsigned int*)&(header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
	unsigned int formatCode = *(unsigned int*)&(header[80]);

	// Read and interpret the raw texture data (bytes 125 -> bufferSize)

	unsigned char* buffer;
	unsigned int bufferSize;

	bufferSize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	buffer = (unsigned char*) malloc(bufferSize * sizeof(char));

	fread(buffer, 1, bufferSize, file);
	fclose(file);

	// Determine what DXT format it's in (DXT1 / DXT3 / DXT5)

	unsigned int format;
	switch(formatCode)
	{
		case DXT1:
			format = GL_DXT1;
			break;
		case DXT3:
			format = GL_DXT3;
			break;
		case DXT5:
			format = GL_DXT5;
			break;
		default:
			free(buffer);
			printf("File at path %s is not a DXT1/3/5 formatted file.", path);
			exit(-1);
	}

	// Pass it to OpenGL

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Sets we unpack on byte-alignment (mode 1)

	// DXT1 uses 8-byte blocks, RGB instead of DXT3/5's RGBA and 16-byte blocks
	unsigned int blockSize = (format == GL_DXT1) ? 8 : 16;
	unsigned int offset = 0;
	for(int level = 0; level < mipMapCount; level++)
	{
		unsigned int size = ( (width+3) / 4) * ( (height+3) / 4) * blockSize;
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height, 0, size, buffer + offset);

		offset += size;
		width /= 2;
		height /= 2;
	}

	free(buffer);
	return textureID;
}