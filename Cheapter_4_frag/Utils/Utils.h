#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <unordered_map>
#include <fstream>

class Utils
{
public:
	static std::string readShaderSource(const char* file_path);

	static void printShaderLog(GLuint shader);

	static void printProgramLog(int prog);

	static bool checkOpenGLError();
};