#include <stdio.h>
#include <stdlib.h>
#include "glad/glad.h"
#include "display.h"
#include "linmath.h"

void callback_key(GLFWwindow* window, int key, int scancode, int action, int mods) {}

void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

static float vertexBufferData[] = {
	-1, -1, 0,
	1, -1, 0,
	0, 1, 0,
};
static GLuint vertexBuffer;

static const GLchar* vertexShaderSrc = 
	"#version 330 core\n"
	"layout(location = 0) in vec3 vertexPosition_modelSpace;\n"
	"uniform mat4 mvp;\n"
	"void main() {\n"
		"gl_Position = mvp * vec4(vertexPosition_modelSpace, 1);\n"
	"}\n\0";

static const GLchar* fragmentShaderSrc = 
	"#version 330 core\n"
	"out vec3 color;\n"
	"void main(){\n"
		"color = vec3(1,0,0);\n"
	"}\n\0";

int buildProgram()
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderID, 1, &vertexShaderSrc, NULL);
	glCompileShader(vertexShaderID);

	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderID, 1, &fragmentShaderSrc, NULL);
	glCompileShader(fragmentShaderID);

	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);
	printf("Linked program\n");

	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragmentShaderID);
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	return programID;
}

Display* display_createDisplay(int width, int height)
{
	if(!glfwInit()) printf("GLFW was not initialized.\n");

	printf("GLFW initialized.\n");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwSetErrorCallback(error_callback);
	
	GLFWwindow* window = glfwCreateWindow(width, height, "Window", NULL, NULL);
	if(!window) printf("Unable to create window");
	printf("Window created.\n");

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
	
	// callbacks
	glfwSetKeyCallback(window, callback_key);
	glfwSwapInterval(1);

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	
	GLuint vertexArrayID;
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData), vertexBufferData, GL_STATIC_DRAW);

	GLuint programID = buildProgram();
	
	Display* display = malloc(sizeof(Display));
	display-> window = window;
	display-> width = width;
	display-> height = height;
	display-> programID = programID;

	return display;
}

void display_tick(Display* display)
{
	glfwSwapBuffers(display-> window);
	glfwPollEvents();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(display-> programID);


	mat4x4 model, view, projection, mvp;

	vec3 eye = {4,3,3};
	vec3 target = {0,0,0};
	vec3 orientation = {0,1,0};

	mat4x4_perspective(projection, 45, (float) display-> width / (float) display-> height, 0.1, 100);
	mat4x4_identity(model);
	mat4x4_look_at(view, eye, target, orientation);
	mat4x4_identity(mvp);
	mat4x4_mul(mvp, mvp, projection);
	mat4x4_mul(mvp, mvp, view);
	mat4x4_mul(mvp, mvp, model);

	GLuint matrixID = glGetUniformLocation(display-> programID, "mvp");
	glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0][0]);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
	glDrawArrays(GL_TRIANGLES, 0, sizeof(vertexBufferData) / 12);
	glDisableVertexAttribArray(0);
}

void display_destroy(Display* display)
{
	glfwDestroyWindow(display-> window);
	glfwTerminate();
}

int display_isWindowClosing(Display* display)
{
	return glfwWindowShouldClose(display-> window);
}