#version 430

out vec4 FragColor;
uniform vec4 color;

void main()
{
	//FragColor = vec4(1.f, 1.f, 1.f, 1.f);
	FragColor = color;
}