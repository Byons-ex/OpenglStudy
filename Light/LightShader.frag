#version 330 core

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform float ambientStrenght;

void main()
{
	//�����Դ�������淶����������
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);  
	//���������������ֵ�����Ϲ�Դ��ɫֵ
	float diff = max(dot(norm, lightDir), 0.0);  //����û������
	vec3 diffuse = diff * lightColor;
	FragColor = vec4(objectColor * (lightColor * (ambientStrenght + diffuse)), 1.0f);
}