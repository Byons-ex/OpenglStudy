#include <Windows.h>
#include <glad\glad.h>
#include <GLFW\glfw3.h>

void FrameBufferSizeCallback(GLFWwindow *window, int w, int h);
void InputProcess(GLFWwindow *window);

int main(int argc, char *argv[])
{
	// ��ʼ��glfw����
	glfwInit();
	
	// ָ��OpenGL�����汾�ź��Ӱ汾��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow *window = glfwCreateWindow(1280, 720, "GLFW WINDOW", NULL, NULL);
	if (window == NULL)
	{
		MessageBoxW(0, L"���ڴ���ʧ��", L"����", MB_ICONERROR);
		glfwTerminate();
		return -1;
	}

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

	// ��Ⱦѭ��
	while (!glfwWindowShouldClose(window))
	{
		InputProcess(window); // ���������¼�

		// ����
		glClearColor(0.0f, 1.0f, 0.0f, 1.0f); 
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window); //��������
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
}
