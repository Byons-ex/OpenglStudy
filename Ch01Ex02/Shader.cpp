#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>

Shader::~Shader()
{
	glDeleteProgram(ID);
}

Shader::Shader(const GLchar * vertexPath, const GLchar * fragmentPath)
{
	std::ifstream verFile(vertexPath);
	if (!verFile.is_open())
	{
		std::cout << L"�򲻿���ɫ���ļ���"<< vertexPath << std::endl;
		return;
	}

	std::ifstream fragFile(fragmentPath);
	if (!fragFile.is_open())
	{
		std::cout << L"�򲻿���ɫ���ļ���" << fragmentPath << std::endl;
		return;
	}
	
	std::stringstream verStream, fragStream;
	verStream<< verFile.rdbuf();
	fragStream << fragFile.rdbuf();

	verFile.close();
	fragFile.close();

	std::string vertexCode = verStream.str();
	std::string fragmentCode = fragStream.str();
	auto vertexSource = vertexCode.c_str();
	auto fragmentSource = fragmentCode.c_str();

	int succeed;
	char infoLog[512];

	unsigned vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &succeed);
	if (!succeed)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "������ɫ������ʧ�ܣ� ������Ϣ��" << infoLog << std::endl;
	}

	unsigned fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &succeed);
	if (!succeed)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "Ƭ����ɫ������ʧ�ܣ� ������Ϣ��" << infoLog << std::endl;
	}

	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &succeed);
	if (!succeed)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "��ɫ������ʧ�ܣ� ������Ϣ��" << infoLog << std::endl;
	}
	
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}