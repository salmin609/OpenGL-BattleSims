#version 430 core

layout(local_size_x = 64, local_size_y = 1, local_size_z = 1) in;

#define State_Idle 0
#define State_Attack 1
#define State_Pain 2
#define State_Run 3
#define State_Death 4

layout(binding = 0) buffer
bufferAnimationIndex {
	int animationIndex[];
};

layout(binding = 1) buffer
bufferAnimationState {
	int prevAnimationIndex[];
};

layout(binding = 2) buffer
bufferNeedReset {
	int needReset[];
};



void main()
{
	uint index = gl_GlobalInvocationID.x + gl_GlobalInvocationID.y * gl_NumWorkGroups.x * gl_WorkGroupSize.x;

	if (needReset[index] == 1)
	{
		animationIndex[index] = prevAnimationIndex[index];
	}
	needReset[index] = 0;
}