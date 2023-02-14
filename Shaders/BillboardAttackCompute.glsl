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

layout(binding = 4) buffer
bufferTargetingCounts {
	int attackedCount[];
};

layout(binding = 5) buffer
bufferObjectDead {
	int isDead[];
};

#define State_Idle 0
#define State_Attack 1
#define State_Pain 2
#define State_Run 3
#define State_Death 4

uniform float dt;

void main(void)
{
	uint index = gl_GlobalInvocationID.x + gl_GlobalInvocationID.y * gl_NumWorkGroups.x * gl_WorkGroupSize.x;

	int attackedNum = attackedCount[index];

	//if (attackedNum > 0)
	//{
		int animIndex = animationIndex[index];

		time[index] += (dt * attackedNum);

		if (time[index] > 5.f && time[index] < 6.5f)
		{
			animationIndex[index] = State_Death;
		}
		else if (time[index] > 6.5f)
		{
			//if (index > 1280)
			//{
			//	obj2Pos[index - 1280].y = -10000.f;
			//}
			//else
			//{
			//	obj1Pos[index].y = -10000.f;
			//}
			isDead[index] = 1;
		}
		attackedCount[index] = 0;
	//}

}