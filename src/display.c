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

static GLfloat vertexBufferData[] = {
    -1.0f,-1.0f,-1.0f, // triangle 1 : begin
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, // triangle 1 : end
    1.0f, 1.0f,-1.0f, // triangle 2 : begin
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f, // triangle 2 : end
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
};

static const GLfloat colorBufferData[] = {
    0.583f,  0.771f,  0.014f,
    0.609f,  0.115f,  0.436f,
    0.327f,  0.483f,  0.844f,
    0.822f,  0.569f,  0.201f,
    0.435f,  0.602f,  0.223f,
    0.310f,  0.747f,  0.185f,
    0.597f,  0.770f,  0.761f,
    0.559f,  0.436f,  0.730f,
    0.359f,  0.583f,  0.152f,
    0.483f,  0.596f,  0.789f,
    0.559f,  0.861f,  0.639f,
    0.195f,  0.548f,  0.859f,
    0.014f,  0.184f,  0.576f,
    0.771f,  0.328f,  0.970f,
    0.406f,  0.615f,  0.116f,
    0.676f,  0.977f,  0.133f,
    0.971f,  0.572f,  0.833f,
    0.140f,  0.616f,  0.489f,
    0.997f,  0.513f,  0.064f,
    0.945f,  0.719f,  0.592f,
    0.543f,  0.021f,  0.978f,
    0.279f,  0.317f,  0.505f,
    0.167f,  0.620f,  0.077f,
    0.347f,  0.857f,  0.137f,
    0.055f,  0.953f,  0.042f,
    0.714f,  0.505f,  0.345f,
    0.783f,  0.290f,  0.734f,
    0.722f,  0.645f,  0.174f,
    0.302f,  0.455f,  0.848f,
    0.225f,  0.587f,  0.040f,
    0.517f,  0.713f,  0.338f,
    0.053f,  0.959f,  0.120f,
    0.393f,  0.621f,  0.362f,
    0.673f,  0.211f,  0.457f,
    0.820f,  0.883f,  0.371f,
    0.982f,  0.099f,  0.879f
};

static GLuint vertexBuffer;
static GLuint colorBuffer;

static const GLchar* vertexShaderSrc = 
	"#version 330 core\n"
	"layout(location = 0) in vec3 vertexPosition_modelSpace;\n"
	"layout(location = 1) in vec3 vertexColor;\n"
	"uniform mat4 mvp;\n"
	"out vec3 fragmentColor;\n"
	"void main() {\n"
		"gl_Position = mvp * vec4(vertexPosition_modelSpace, 1);\n"
		"fragmentColor = vertexColor;\n"
	"}\n\0";

static const GLchar* fragmentShaderSrc = 
	"#version 330 core\n"
	"in vec3 fragmentColor;"
	"out vec3 color;\n"
	"void main(){\n"
		"color = fragmentColor;\n"
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

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	
	GLuint vertexArrayID;
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData), vertexBufferData, GL_STATIC_DRAW);

	glGenBuffers(1, &colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colorBufferData), colorBufferData, GL_STATIC_DRAW);

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

	vec3 eye = {4,3,2};
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

	// vec3 vertexPosition_modelSpace
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

	// vec3 color
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

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