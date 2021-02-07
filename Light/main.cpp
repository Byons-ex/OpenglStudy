#include <Windows.h>
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "../SDK/Shader.h"
#include <stb_image.h>
#include <iostream>
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"

#pragma comment(lib, "legacy_stdio_definitions.lib")
#pragma comment(lib, "glfw3.lib")

using namespace glm;

Camera camera;

float lastFrame = 0.0f;
float deltaTime = 0.0f;

void FrameBufferSizeCallback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
}

void InputProcess(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	deltaTime = glfwGetTime() - lastFrame;
	lastFrame += deltaTime;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) 
{
	camera.ProcessMouseScroll(yoffset);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	static float lastX, lastY;
	static float yaw = 0.0f, pitch = 0.0f;

	static bool firstMouse = true;
	if (firstMouse) {  //���ó�ʼλ�ã���ֹͻȻ����ĳ��������
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = lastX - xpos;   //�����ˣ��ڴ����У���ߵ�����С���ұߵ����꣬��������Ҫһ�����ĽǶ�
	float yoffset = lastY - ypos;   //ͬ�����ڴ����У���������������������꣬����������̧ͷ��ʱ����Ҫһ�����ĽǶ�
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.5f;  //��ת����
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)  //���Ͽ����ܳ���90��
		pitch = 89.0f;
	if (pitch < -89.0f)  //���¿�Ҳ���ܳ���90��
		pitch = -89.0f;

	camera.ProcessMouseMovement(xoffset, yoffset, pitch);
}

constexpr unsigned SCR_WIDTH = 1280;
constexpr unsigned SCR_HEIGHT = 720;

int main(int argc, char* argv[])
{
	// ��ʼ��glfw����
	glfwInit();

	// ָ��OpenGL�����汾�ź��Ӱ汾��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "GLFW WINDOW", NULL, NULL);
	if (window == NULL)
	{
		MessageBoxW(0, L"���ڴ���ʧ��", L"����", MB_ICONERROR);
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window); // �����ڵ������Ļ�������Ϊ���̵߳������Ļ�����

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// ��ʼ��glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		MessageBoxW(0, L"��ʼ��GLADʧ��", L"����", MB_ICONERROR);
		glfwTerminate();
		return -1;
	}

	// ע�ᴰ�ڻص�����
	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);

	glfwSwapInterval(1);

	float vertices[] = {
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	Shader lightShader("Light01.vert", "LightShader.frag");
	Shader lampShader("Light01.vert", "LampShader.frag");

	unsigned VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	unsigned VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// �����ʽ
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	mat4 lightCubeModel;
	lightCubeModel = translate(lightCubeModel, vec3(0, 0, 0));
	lightCubeModel = glm::rotate(lightCubeModel, glm::radians(45.0f), glm::vec3(1.0f, 1.0f, 0.0f));

	mat4 lampCubeModel;
	lampCubeModel = translate(lampCubeModel, vec3(1.2f, 1.0f, 2.0f));
	lampCubeModel = glm::scale(lampCubeModel, glm::vec3(0.2f));

	

	glEnable(GL_DEPTH_TEST);

	deltaTime = glfwGetTime();
	lastFrame = deltaTime;

	// ��Ⱦѭ��
	while (!glfwWindowShouldClose(window))
	{
		InputProcess(window); // ���������¼�

		// ����
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mat4 view = camera.GetViewMatrix();
		mat4 projection = perspective(camera.Zoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		lightShader.Use();
		glBindVertexArray(VAO);
		lightShader.SetVec3("objectColor", 0.4, 0.1, 0.1);
		lightShader.SetVec3("lightColor", 1.0f, 1.0f, 1.0f);
		lightShader.SetMat4("model", glm::value_ptr(lightCubeModel));
		lightShader.SetMat4("view", glm::value_ptr(view));
		lightShader.SetMat4("projection", glm::value_ptr(projection));

		glDrawArrays(GL_TRIANGLES, 0, 36);

		lampShader.Use();
		glBindVertexArray(VAO);
		lampShader.SetMat4("model", glm::value_ptr(lampCubeModel));
		lampShader.SetMat4("view", glm::value_ptr(view));
		lampShader.SetMat4("projection", glm::value_ptr(projection));

		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents(); // ��ѯ�¼�
	}

	glfwTerminate();

	return 0;
}