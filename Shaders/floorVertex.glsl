#version 330 core
layout(location = 0) in vec3 aPos;


uniform mat4 projViewModelMat;

void main()
{
	gl_Position = projViewModelMat * vec4(aPos, 1.0);
}