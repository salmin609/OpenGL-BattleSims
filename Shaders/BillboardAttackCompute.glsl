#version 430 core

layout(local_size_x = 64, local_size_y = 1, local_size_z = 1) in;


layout(binding = 0) buffer
bufferTimeCheck {
	float time[];
};

layout(binding = 1) buffer
bufferAnimationIndex {
	int animationIndex[];
};

layout(std430, binding = 2) buffer
bufferObjPos1 {
	vec4 obj1Pos[];
};

layout(std430, binding = 3) buffer
bufferObjPos2 {
	vec4 obj2Pos[];
};

uniform float dt;

void main(void)
{
	uint index = gl_GlobalInvocationID.x + gl_GlobalInvocationID.y * gl_NumWorkGroups.x * gl_WorkGroupSize.x;

	int animIndex = animationIndex[index];

	if(animIndex == 1 || animIndex == 4)
		time[index] += dt;

	if (time[index] > 5.f && time[index] < 6.5f)
	{
		animationIndex[index] = 4;
	}
	else if (time[index] > 6.5f)
	{
		if (index > 1280)
		{
			obj2Pos[index - 1280].y = -10000.f;
		}
		else
		{
			obj1Pos[index].y = -10000.f;
		}
	}
}