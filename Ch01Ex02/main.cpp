#include <Windows.h>
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "Shader.h"
#include <stb_image.h>
#include <iostream>
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void FrameBufferSizeCallback(GLFWwindow *window, int w, int h);
void InputProcess(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

float factor = 1.0f;

constexpr unsigned SCR_WIDTH = 1280;
constexpr unsigned SCR_HEIGHT = 720;

float deltaTime = 0;
float lastFrame = 0;
glm::vec3 cameraPos = { 0, 0, 4 };
glm::vec3 cameraFront = { 0, 0, -1 };
glm::vec3 cameraUp = { 0, 1, 0 };

int main(int argc, char *argv[])
{
	// ��ʼ��glfw����
	glfwInit();
	
	// ָ��OpenGL�����汾�ź��Ӱ汾��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "GLFW WINDOW", NULL, NULL);
	if (window == NULL)
	{
		MessageBoxW(0, L"���ڴ���ʧ��", L"����", MB_ICONERROR);
		glfwTerminate();
		return -1;
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);

	glfwMakeContextCurrent(window); // �����ڵ������Ļ�������Ϊ���̵߳������Ļ�����

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

	Shader shader("Ch01Ex03.vert", "Ch01Ex04.frag");

	//�������㻺��
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

	unsigned indices[] = 
	{
		0, 1, 3,
		1, 2, 3,
	};

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
	
	//���������ʽ
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	unsigned EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	glActiveTexture(GL_TEXTURE0);
	// ������
	unsigned texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// ���������װ�͹��˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//��תͼƬ
	stbi_set_flip_vertically_on_load(true);

	//��������
	int width, height, nrChannels;
	unsigned char *data = stbi_load("funny.jpg", &width, &height, &nrChannels, 0);
	GLint format = (nrChannels == 3) ? GL_RGB : GL_RGBA;
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	glActiveTexture(GL_TEXTURE1);

	// ������
	unsigned texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	// ���������װ�͹��˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//��������
	int width1, height1, nrChannels1;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data1 = stbi_load("container.jpg", &width1, &height1, &nrChannels1, 0);
	GLint format1 = (nrChannels1 == 3) ? GL_RGB : GL_RGBA;
	if (data1)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, format1, width1, height1, 0, format1, GL_UNSIGNED_BYTE, data1);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	stbi_image_free(data1);

	shader.Use();
	shader.SetInt("texture1", 0);
	shader.SetInt("texture2", 1);

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

	lastFrame = glfwGetTime();

	// ��Ⱦѭ��
	while (!glfwWindowShouldClose(window))
	{
		float currTime = glfwGetTime();
		deltaTime = currTime - lastFrame;
		lastFrame = currTime;

		InputProcess(window); // ���������¼�

		// ����
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture1);

		shader.Use();
		shader.SetFloat("factor", factor);

		// �۲�任����
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		// ͶӰ�任����
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		shader.SetMat4("view", glm::value_ptr(view));
		shader.SetMat4("projection", glm::value_ptr(projection));
		
		glEnable(GL_DEPTH_TEST);
		glBindVertexArray(VAO);
		for (int i = 0; i < 10; ++i)
		{
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * glfwGetTime() * (i);
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			shader.SetMat4("model", glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glfwSwapBuffers(window);
		glfwPollEvents(); // ��ѯ�¼�
	}

	glfwTerminate();
	return 0;
}

/*
** ���ڴ�С���Ĵ���ص�
*/
void FrameBufferSizeCallback(GLFWwindow * window, int w, int h)
{
	glViewport(0, 0, w, h);
}

/*
** ���������¼�
*/
void InputProcess(GLFWwindow * window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		factor += 0.01f;
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		factor -= 0.01f;
	}

	float cameraSpeed = 5.0f * deltaTime; //�ƶ��ٶ�
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		// ���Front��UP�õ�һ��Right�������������һ����Ȼ���ٳ����ƶ�����
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraUp;

	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraUp;
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

	float sensitivity = 0.05f;  //��ת����
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)  //���Ͽ����ܳ���90��
		pitch = 89.0f;
	if (pitch < -89.0f)  //���¿�Ҳ���ܳ���90��
		pitch = -89.0f;

	glm::vec3 front;
	front.x = -sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = -cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	cameraFront = glm::normalize(front);
}
