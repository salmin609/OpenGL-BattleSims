#version 430 core

layout(local_size_x = 64, local_size_y = 1, local_size_z = 1) in;


layout(std430, binding = 0) readonly buffer
bufferObjsPos {
	vec4 objsPoses[];
};

layout(binding = 1) buffer
bufferRangedTimeCheck {
	float rangedTimer[];
};

layout(binding = 2) readonly buffer
bufferAnimationIndices
{
	int animationIndices[];
};

uniform int herdAttackTypes[32];
uniform int herdOffset[32];
int herdIndex;
uint index;
uniform int herdCount;
uniform float dt;

#define Attack_Melee 101
#define Attack_Ranged 102

#define State_Idle 0
#define State_Attack 1
#define State_Pain 2
#define State_Run 3
#define State_Death 4

void GetBufferOffset()
{
	for (int i = herdCount - 1; i >= 0; --i)
	{
		int offset = herdOffset[i];

		if (index >= uint(offset))
		{
			herdIndex = i;
			break;
		}
	}
}

void main(void)
{
	index = gl_GlobalInvocationID.x + gl_GlobalInvocationID.y * gl_NumWorkGroups.x * gl_WorkGroupSize.x;

	GetBufferOffset();

	int attackType = herdAttackTypes[herdIndex];
	int animationIndex = animationIndices[index];


	if (attackType == Attack_Ranged)
	{
		if (animationIndex == State_Attack)
		{
			rangedTimer[index] += dt;

			

		}
	}
}