#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <unordered_map>
#include <fstream>
#include <string>

#define numVAOs 1

GLuint renderingProgram;
GLuint vao[numVAOs];


bool pre_init();
void init(GLFWwindow* window);
void display_blue_pixel(GLFWwindow* window, double currentTime);

GLuint createShaderProgram();
std::string readShaderSource(const char* file_path);

void printShaderLog(GLuint shader);
void printProgramLog(int prog);
bool checkOpenGLError();

int main()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //主版本号为4
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //次版本号为3

	if (!pre_init() || !glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	GLFWwindow* mainWindow = glfwCreateWindow(800, 600, "Chapter3_Baisic", NULL, NULL);
	glfwMakeContextCurrent(mainWindow);

	if (glewInit() != GLEW_OK) //glewInit must call after glfwCreateWindow(...)
	{
		exit(EXIT_FAILURE);
	}

	glfwSwapInterval(1);

	init(mainWindow);

	while (!glfwWindowShouldClose(mainWindow))
	{
		glfwSwapBuffers(mainWindow); //开启垂直同步
		glfwPollEvents();
		display_blue_pixel(mainWindow, glfwGetTime());
	}

	glfwDestroyWindow(mainWindow);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

bool pre_init()
{
	return true;
}

void init(GLFWwindow* window)
{
	renderingProgram = createShaderProgram();
	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);
}

void display_blue_pixel(GLFWwindow* window, double currentTime)
{
	glUseProgram(renderingProgram);
	glPointSize(30.f);
	glDrawArrays(GL_POINTS, 0, 1);
}

GLuint createShaderProgram()
{
	std::string vshaderSource_file = readShaderSource("VertexShader.glsl");
	std::string fshaderSource_file = readShaderSource("FragShader.glsl");
	const char* vshaderSource = vshaderSource_file.c_str();
	const char* fshaderSource = fshaderSource_file.c_str();

	//const char* vshaderSource =
	//	"#version 430 \n"
	//	"void main(void) \n"
	//	"{ gl_Position = vec4(0.0, 0.0, 0.0, 1.0);}";


	//const char* fshaderSource =
	//	"#version 430 \n"
	//	"out vec4 color; \n"
	//	"void main(void) \n"
	//	"{ color = vec4(0.0, 0.0, 1.0, 1.0);}";

	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

	std::unordered_map<GLuint, GLint>shader_compiled_map;
	shader_compiled_map.insert(std::pair<GLuint, GLint>(vShader, 0));
	shader_compiled_map.insert(std::pair<GLuint, GLint>(fShader, 0));

	glShaderSource(vShader, 1, &vshaderSource, NULL);
	glShaderSource(fShader, 1, &fshaderSource, NULL);
	glCompileShader(vShader);
	glCompileShader(fShader);
	checkOpenGLError();

	// 捕获着色器编译错误
	for (std::pair<const GLuint, GLint>& shader_compiled : shader_compiled_map)
	{
		glGetShaderiv(shader_compiled.first, GL_COMPILE_STATUS, &shader_compiled.second);
		if (shader_compiled.second != 1)
		{
			std::cout << "shader [" << shader_compiled.first << "] compile failed" << std::endl;
			printShaderLog(shader_compiled.first);
		}
	}


	GLuint vfProgram = glCreateProgram();
	glAttachShader(vfProgram, vShader);
	glAttachShader(vfProgram, fShader);
	glLinkProgram(vfProgram);
	//捕获着色器链接错误
	checkOpenGLError();
	GLint linked;
	glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
	if (linked != 1)
	{
		std::cout << "linking failed" << std::endl;
		printProgramLog(vfProgram);
	}

	return vfProgram;

}

std::string readShaderSource(const char* filePath)
{
	std::string content;
	std::ifstream fileStream(filePath, std::ios::in);
	std::string line = "";
	while (!fileStream.eof())
	{
		getline(fileStream, line);
		content.append(line + "\n");
	}
	fileStream.close();
	return content;
}

void printShaderLog(GLuint shader)
{
	int len = 0;
	int chWrittn = 0;
	char* log;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
	if (len > 0)
	{
		log = (char*)malloc(len);
		glGetShaderInfoLog(shader, len, &chWrittn, log);
		std::cout << "Shader Info Log:\n" << log << std::endl;
		free(log);
	}
}

void printProgramLog(int prog)
{
	int len = 0;
	int chWrittn = 0;
	char* log;
	glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
	if (len > 0)
	{
		log = (char*)malloc(len);
		glGetProgramInfoLog(prog, len, &chWrittn, log);
		std::cout << "Program Info Log:\n" << log << std::endl;
		free(log);
	}
}

bool checkOpenGLError()
{
	bool foundError = false;
	int glErr = glGetError();
	while (glErr != GL_NO_ERROR)
	{
		std::cout << "glError: " << glErr << std::endl;
		foundError = true;
		glErr = glGetError();
	}
	return foundError;
}