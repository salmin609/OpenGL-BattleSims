/*
 * Author		: Ryan Kim.
 * Date			: 2023-03-17
 * Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#version 430 core

layout(local_size_x = 64, local_size_y = 1, local_size_z = 1) in;

layout(std430, binding = 0) writeonly buffer
bufferObjsCollisionStatus
{
	int objsCollisionStatus[];
};

layout(std430, binding = 1) readonly buffer
bufferObjsPos
{
	vec4 objsPoses[];
};

layout(std430, binding = 2) writeonly buffer
bufferObjsDirection
{
	vec4 objsDirections[];
};

layout(binding = 3) readonly buffer
bufferObjectDead
{
	int isDead[];
};

layout(binding = 4) buffer
bufferTargetingCounts
{
	int attackedCount[];
};

layout(binding = 5) buffer
bufferHerdAttackingCount
{
	int herdAttackingCounts[];
};

int herdIndex;
uint index;
float radius = 15.f;
float attackRange;


uniform int herdCount;
uniform float dt;

uniform vec4 herdDirections[32];
uniform float herdSpeeds[32];
uniform int herdSides[32];
uniform int herdCounts[32];
uniform float herdAttackRanges[32];
uniform int herdOffset[32];

#define Collision_With_Ally -1
#define Collision_With_Enemy -2
#define Collision_None -3

#define Attack_Melee 101
#define Attack_Ranged 102

void MoveToward(inout vec4 pos, vec4 direction, float moveSpeed)
{
	pos = pos + normalize(direction) * dt * moveSpeed;
	pos.w = 1.f;
}

bool CheckCollision(vec4 pos, vec4 otherPos, float range)
{
	float distance = distance(pos, otherPos);

	if (distance < range)
		return true;
	return false;
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

int CheckCollisionBtwEnemyAndAlly(inout int allyCollisionIndex)
{
	vec4 thisPos = objsPoses[index];
	vec4 nextPos = thisPos;
	vec4 herdDirection = herdDirections[herdIndex];
	float speed = herdSpeeds[herdIndex];
	MoveToward(nextPos, herdDirection, speed);
	int herdIndexOffset = 0;
	int thisObjSide = herdSides[herdIndex];

	for (int i = 0; i < herdCount; ++i)
	{
		int herdNums = herdCounts[i];
		int otherObjSide = herdSides[i];

		for (int j = 0; j < herdNums; ++j)
		{
			uint otherIndex = uint(herdIndexOffset + j);
			vec4 otherPos = objsPoses[otherIndex];
			int dead = isDead[otherIndex];

			if (dead == 0)
			{
				//ally obj
				if (otherObjSide == thisObjSide)
				{
					if (otherIndex != index)
					{
						if (CheckCollision(nextPos, otherPos, radius))
						{
							allyCollisionIndex = int(otherIndex);
							return Collision_With_Ally;
						}
					}
				}
				//enemy obj
				else
				{
					if (CheckCollision(thisPos, otherPos, attackRange))
					{
						attackedCount[otherIndex] += 1;
						objsDirections[index] = otherPos - thisPos;
						herdAttackingCounts[herdIndex] += 1;
						return Collision_With_Enemy;
					}
				}
			}
		}
		herdIndexOffset += herdNums;
	}
	return Collision_None;
}

void main(void)
{
	index = gl_GlobalInvocationID.x + gl_GlobalInvocationID.y * gl_NumWorkGroups.x * gl_WorkGroupSize.x;

	GetBufferOffset();
	attackRange = herdAttackRanges[herdIndex];

	int allyCollisionIndex = 0;
	int collisionStatus = CheckCollisionBtwEnemyAndAlly(allyCollisionIndex);

	//ally collision need to record index.
	if (collisionStatus == Collision_With_Ally)
	{
		collisionStatus += allyCollisionIndex;
	}

	objsCollisionStatus[index] = collisionStatus;

}