#include "Utils.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"

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

GLuint brickTexture;

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

	GLFWwindow* mainWindow = glfwCreateWindow(800, 600, "Chapter5_", NULL, NULL);
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
		display(mainWindow, glfwGetTime());
	}

	glfwDestroyWindow(mainWindow);
	glfwTerminate();
	exit(EXIT_SUCCESS);

	return 0;
}

void setupVertices()
{

	float pyramidVertexPositions[54] = {
		-1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  1.0f,  0.0f, //front
		 1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  1.0f,  0.0f, //right
		 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  1.0f,  0.0f, //behind
		-1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  1.0f,  0.0f, //left
		-1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f
	};

	float pyramidTextureCoords[36] = {
		0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, //前侧面，右侧面
		0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, //后侧面，左侧面
		0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f  //底面
	};

	glGenVertexArrays(VAO_NUMS, VAO);
	glBindVertexArray(VAO[0]);

	glGenBuffers(VBP_NUMS, VBO);	//VBO[0]用于顶点坐标，VBO[1]用于纹理坐标

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVertexPositions), pyramidVertexPositions, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidTextureCoords), pyramidTextureCoords, GL_STATIC_DRAW);
}

void init(GLFWwindow* window)
{
	glClearColor(1.0f, 1.0f, 0.f, 1.0f);

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

	 brickTexture = Utils::loadTextureData("wall.jpg");
	//glGenTextures(1, &brickTexture);

	//int width, height, nrChannels;
	//unsigned char* data = stbi_load("wall.jpg", &width, &height, &nrChannels, 0);
	//glBindTexture(GL_TEXTURE_2D, brickTexture);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	//stbi_image_free(data);
	Utils::checkOpenGLError();
}

void display(GLFWwindow* window, double currentTime)
{
	glClear(GL_DEPTH_BUFFER_BIT);													//清除深度缓冲区
	glClear(GL_COLOR_BUFFER_BIT);													//清除颜色缓冲区
	glUseProgram(renderingProgram);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);							//将第0个属性关联到该缓冲区，一次读3个，每个读一个float大小
	glEnableVertexAttribArray(0);													//启用第0个顶点属性

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);							//关联第1个属性 (texCoord)
	glEnableVertexAttribArray(1);													

	//关联纹理到顶点属性
	glBindTexture(GL_TEXTURE_2D, brickTexture);
	glActiveTexture(GL_TEXTURE0);	//可用纹理单元取决于显卡，这里使用第一个纹理单元
	
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	

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

	//glEnable(GL_TEXTURE_2D);
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
