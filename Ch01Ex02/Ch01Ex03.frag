#version 330 core

in vec2 ourTexcoord;

out vec4 FragColor;  //��ɫ���

uniform sampler2D ourTexture;

void main()
{
	FragColor = texture(ourTexture, ourTexcoord);
}