#version 330 core
layout (location = 0) in vec3 aPos;
out vec4 ourColor;

void main()
{
	gl_Position = vec4(aPos, 1.0f);
	ourColor = vec4(0.1f, 0.5f, 0.43f, 1.0f);
}