#include "Utils.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

GLuint renderingProgram;

void setupVertices();
void init(GLFWwindow* window);
void display(GLFWwindow* window, double currentTime);
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
AActor Camera, Cube_A;
glm::mat4 mvMat, pMat, mMat, vMat;


int main()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //���汾��Ϊ4
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //�ΰ汾��Ϊ3

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
		glfwSwapBuffers(mainWindow); //������ֱͬ��
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
	float vertexPositions[108] = {
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

	glGenVertexArrays(VAO_NUMS, VAO);
	glBindVertexArray(VAO[0]);

	glGenBuffers(VBP_NUMS, VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);														//ָ��VBO[0]ΪActive Buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);	//�����ݴ�vPositions���Ƶ���ǰ��Active Buffer
}

void init(GLFWwindow* window)
{
	renderingProgram = createShaderProgram();

	Camera.Location[0] = 0.0f;
	Camera.Location[1] = 0.0f;
	Camera.Location[2] = 8.0f;

	Cube_A.Location[0] = 0.0f;
	Cube_A.Location[1] = -2.0f;
	Cube_A.Location[2] = 0.0f;

	setupVertices();
}

void display(GLFWwindow* window, double currentTime)
{
	glClear(GL_DEPTH_BUFFER_BIT);													//�����Ȼ�����
	glClear(GL_COLOR_BUFFER_BIT);													//�����ɫ������
	glUseProgram(renderingProgram);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);							//����0�����Թ������û�������һ�ζ�3����ÿ����һ��float��С
	glEnableVertexAttribArray(0);													//���õ�0����������

	//��ȡShader��uniform������λ��
	GLint mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	GLint projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

	//Project Matrix
	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);						//1.0472f = 60 degrees

	// View-Matrix
	vMat = glm::translate(glm::mat4(1.0f), -glm::vec3(Camera.Location[0], Camera.Location[1], Camera.Location[2]));

	// Model-Matrix
	mMat = glm::translate(glm::mat4(1.0f), glm::vec3(Cube_A.Location[0], Cube_A.Location[1], Cube_A.Location[2]));

	mvMat = vMat * mMat;

	//�������ݵ�uniform��
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

	//����VBO����������
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 36);
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

	// ������ɫ���������
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
	//������ɫ�����Ӵ���
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