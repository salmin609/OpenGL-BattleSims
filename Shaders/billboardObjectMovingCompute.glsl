/*
 * Author		: Ryan Kim.
 * Date			: 2023-03-17
 * Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#version 430 core

layout(local_size_x = 64, local_size_y = 1, local_size_z = 1) in;


layout(std430, binding = 0) buffer
bufferObjsPos
{
	vec4 objsPoses[];
};

layout(binding = 1) readonly buffer
bufferObjectDead
{
	int isDead[];
};

layout(binding = 2) writeonly buffer
bufferHerdReachedDestination
{
	int herdReachedDestination[];
};

layout(binding = 3) buffer
bufferHerdAttackingCount
{
	int herdAttackingCounts[];
};

layout(binding = 4) buffer
bufferObjsCollisionStatus
{
	int objsCollisionStatus[];
};

layout(binding = 5) buffer
bufferObjsDirections
{
	vec4 objsDirections[];
};


#define State_Idle 0
#define State_Attack 1
#define State_Pain 2
#define State_Run 3
#define State_Death 4

#define Collision_With_Ally -1
#define Collision_With_Enemy -2
#define Collision_None -3

#define Attack_Melee 101
#define Attack_Ranged 102

uniform int herdOffset[32];
uniform int herdCounts[32];
uniform int herdSides[32];
uniform vec4 herdDirections[32];
uniform float herdSpeeds[32];
uniform vec4 herdDestinations[32];
uniform int herdWidths[32];
uniform int herdAttackTypes[32];

uniform float dt;
uniform int herdCount;
float radius = 15.f;
int herdIndex;
uint index;

void MoveToward(inout vec4 pos, vec4 direction, float moveSpeed)
{
	pos = pos + normalize(direction) * dt * moveSpeed;
	pos.w = 1.f;
}

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

int GetHerdIndex(int index_)
{
	int result = 0;

	for (int i = herdCount - 1; i >= 0; --i)
	{
		int offset = herdOffset[i];

		if (index_ >= offset)
		{
			result = i;
			break;
		}
	}

	return result;
}


void main(void)
{
	index = gl_GlobalInvocationID.x + gl_GlobalInvocationID.y * gl_NumWorkGroups.x * gl_WorkGroupSize.x;

	GetBufferOffset();
	int herdAttackingNum = herdAttackingCounts[herdIndex];
	int herdNum = herdCounts[herdIndex];
	int attackType = herdAttackTypes[herdIndex];
	vec4 herdDirection = herdDirections[herdIndex];
	float speed = herdSpeeds[herdIndex];

	int dead = isDead[index];
	bool isStop = dot(herdDirection, herdDirection) == 0;
	int thisCollisionState = objsCollisionStatus[index];

	int herdIndexStart = herdOffset[herdIndex];

	int herdNoneCollisionCount = 0;

	if (dead == 0)
	{
		if (isStop == false)
		{
			if (thisCollisionState == Collision_None)
			{
				objsDirections[index] = herdDirection;
				MoveToward(objsPoses[index], herdDirection, speed);

				float dist = distance(objsPoses[index], herdDestinations[herdIndex]);

				if (dist < 10.f)
					herdReachedDestination[herdIndex] = 1;
			}

		}
	}
}