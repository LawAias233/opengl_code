#include "Utils/Utils.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

GLuint renderingProgram;

void setupVertices();
void init(GLFWwindow* window);
void display(GLFWwindow* window, double currentTime);
void display_with_multiple_model(GLFWwindow* window, double currentTime);
GLuint createShaderProgram();

const GLuint VAO_NUMS = 1;
const GLuint VBP_NUMS = 2;

GLuint VAO[VAO_NUMS];
GLuint VBO[VBP_NUMS];

struct AActor {
	float Location[3];
	float Rotation[3];
	float Scale[3];
};

float aspect;
int width, height;
AActor ACamera, ACube_A, APyramid_A;
glm::mat4 mvMat, pMat, mMat, vMat;


int main()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //主版本号为4
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //次版本号为3

	if (!glfwInit())
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
		//display(mainWindow, glfwGetTime());
		display_with_multiple_model(mainWindow, glfwGetTime());
	}

	glfwDestroyWindow(mainWindow);
	glfwTerminate();
	exit(EXIT_SUCCESS);

	return 0;
}

void setupVertices()
{
	float cubeVertexPositions[108] = {
		-1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f, 
		 1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f, -1.0f, 
		 1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f, 
		 1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f, 
		-1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f, 
		-1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f, 
		-1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f, 
		-1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f, 
		 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, 
		-1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f
	};

	float pyramidVertexPositions[54] = {
		-1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  1.0f,  0.0f, //front
		 1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  1.0f,  0.0f, //right
		 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  1.0f,  0.0f, //behind
		-1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  1.0f,  0.0f, //left
		-1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f
	};

	glGenVertexArrays(VAO_NUMS, VAO);
	glBindVertexArray(VAO[0]);

	glGenBuffers(VBP_NUMS, VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);																//指定VBO[0]为Active Buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexPositions), cubeVertexPositions, GL_STATIC_DRAW);	//把数据从vPositions复制到当前的Active Buffer

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVertexPositions), pyramidVertexPositions, GL_STATIC_DRAW);
}

void init(GLFWwindow* window)
{
	renderingProgram = createShaderProgram();

	ACamera.Location[0] = 0.0f;
	ACamera.Location[1] = 0.0f;
	ACamera.Location[2] = 8.0f;

	ACube_A.Location[0] = 0.0f;
	ACube_A.Location[1] = -2.0f;
	ACube_A.Location[2] = 0.0f;

	APyramid_A.Location[0] = 0.0f;
	APyramid_A.Location[1] = 2.0f;
	APyramid_A.Location[2] = 0.0f;

	setupVertices();
}

void display(GLFWwindow* window, double currentTime)
{
	glClear(GL_DEPTH_BUFFER_BIT);													//清除深度缓冲区
	glClear(GL_COLOR_BUFFER_BIT);													//清除颜色缓冲区
	glUseProgram(renderingProgram);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);							//将第0个属性关联到该缓冲区，一次读3个，每个读一个float大小
	glEnableVertexAttribArray(0);													//启用第0个顶点属性

	//获取Shader中uniform变量的位置
	GLint mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	GLint projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

	//Project Matrix
	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);						//1.0472f = 60 degrees

	// View-Matrix
	vMat = glm::translate(glm::mat4(1.0f), -glm::vec3(ACamera.Location[0], ACamera.Location[1], ACamera.Location[2]));

	// Model-Matrix
	mMat = glm::translate(glm::mat4(1.0f), glm::vec3(ACube_A.Location[0], ACube_A.Location[1], ACube_A.Location[2]));

	mvMat = vMat * mMat;

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

	//关联VBO到顶点属性
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void display_with_multiple_model(GLFWwindow* window, double currentTime)
{
	glClear(GL_DEPTH_BUFFER_BIT);													//清除深度缓冲区
	glClear(GL_COLOR_BUFFER_BIT);													//清除颜色缓冲区
	glUseProgram(renderingProgram);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);							//将第0个属性关联到该缓冲区，一次读3个，每个读一个float大小
	glEnableVertexAttribArray(0);													//启用第0个顶点属性

	//获取Shader中uniform变量的位置
	GLint mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	GLint projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

	//Project Matrix
	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);						//1.0472f = 60 degrees
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

	// View-Matrix
	vMat = glm::translate(glm::mat4(1.0f), -glm::vec3(ACamera.Location[0], ACamera.Location[1], ACamera.Location[2]));

	// Draw Cuba Model
	mMat = glm::translate(glm::mat4(1.0f), glm::vec3(ACube_A.Location[0], ACube_A.Location[1], ACube_A.Location[2]));
	mvMat = vMat * mMat;
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));

	//关联VBO到顶点属性
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// Draw Pyramid Model
	mMat = glm::translate(glm::mat4(1.0f), glm::vec3(APyramid_A.Location[0], APyramid_A.Location[1], APyramid_A.Location[2]));
	mvMat = mMat * vMat;

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 18);
}


GLuint createShaderProgram()
{
	std::string vshaderSource_file = Utils::readShaderSource("VertexShader.glsl");
	std::string fshaderSource_file = Utils::readShaderSource("FragShader.glsl");
	const char* vshaderSource = vshaderSource_file.c_str();
	const char* fshaderSource = fshaderSource_file.c_str();


	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

	std::unordered_map<GLuint, GLint>shader_compiled_map;
	shader_compiled_map.insert(std::pair<GLuint, GLint>(vShader, 0));
	shader_compiled_map.insert(std::pair<GLuint, GLint>(fShader, 0));

	glShaderSource(vShader, 1, &vshaderSource, NULL);
	glShaderSource(fShader, 1, &fshaderSource, NULL);
	glCompileShader(vShader);
	glCompileShader(fShader);
	Utils::checkOpenGLError();

	// 捕获着色器编译错误
	for (std::pair<const GLuint, GLint>& shader_compiled : shader_compiled_map)
	{
		glGetShaderiv(shader_compiled.first, GL_COMPILE_STATUS, &shader_compiled.second);
		if (shader_compiled.second != 1)
		{
			std::cout << "shader [" << shader_compiled.first << "] compile failed" << std::endl;
			Utils::printShaderLog(shader_compiled.first);
		}
	}


	GLuint vfProgram = glCreateProgram();
	glAttachShader(vfProgram, vShader);
	glAttachShader(vfProgram, fShader);
	glLinkProgram(vfProgram);
	//捕获着色器链接错误
	Utils::checkOpenGLError();
	GLint linked;
	glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
	if (linked != 1)
	{
		std::cout << "linking failed" << std::endl;
		Utils::printProgramLog(vfProgram);
	}

	return vfProgram;

}
