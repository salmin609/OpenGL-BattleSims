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
bufferObjsPos {
	vec4 objsPoses[];
};

layout(binding = 3) buffer
bufferTargetingCounts {
	int attackedCount[];
};

layout(binding = 4) buffer
bufferObjectDead {
	int isDead[];
};

//layout(binding = 5) buffer
//bufferHerdNumCount {
//	int herdNumCount[];
//};

#define State_Idle 0
#define State_Attack 1
#define State_Pain 2
#define State_Run 3
#define State_Death 4

uniform float dt;
uniform int herdCount;

uniform int herdCounts[32];
uniform int herdOffset[32];

int herdIndex;
uint index;
//
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
	int attackedNum = attackedCount[index];
	int animIndex = animationIndex[index];
	float deathTimer = 7.f;
	float term = 1.5f;
	float acDeath = deathTimer + term;

	GetBufferOffset();

	if (attackedNum > 0)
	{
		time[index] += (dt * attackedNum);
		attackedCount[index] = 0;
	}
	else
	{
		if (animIndex == State_Death)
		{
			time[index] += dt;
		}
	}

	if (time[index] > deathTimer && time[index] < acDeath)
	{
		animationIndex[index] = State_Death;
	}
	else if (time[index] > acDeath)
	{
		isDead[index] = 1;
		//herdNumCount[herdIndex]--;
	}
}