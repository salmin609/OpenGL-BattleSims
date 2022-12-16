#version 330 core

layout(location = 0) in vec3 pos;

out vec3 texCoords;
uniform mat4 projection;
uniform mat4 cam;

void main()
{
	texCoords = pos;
	vec4 aPos = projection * cam * vec4(pos, 1.0);
	gl_Position = aPos.xyww;
}