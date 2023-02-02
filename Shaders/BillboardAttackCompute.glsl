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

uniform float dt;

void main(void)
{
	uint index = gl_GlobalInvocationID.x + gl_GlobalInvocationID.y * gl_NumWorkGroups.x * gl_WorkGroupSize.x;

	int animIndex = animationIndex[index];

	if(animIndex == 2)
		time[index] += dt;

	if (time[index] > 5.f)
		animationIndex[index] = 3;
}