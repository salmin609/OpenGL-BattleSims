#version 430 core

layout(local_size_x = 64, local_size_y = 1, local_size_z = 1) in;


layout(binding = 0) buffer
bufferReached {
	int animationIndex[];
};

layout(std430, binding = 1) buffer
bufferObjsPos {
	vec4 objsPoses[];
};

layout(std430, binding = 2) buffer
bufferObjsDirection {
	vec4 objsDirections[];
};


layout(binding = 3) buffer
bufferTimeCheck {
	float time[];
};

layout(binding = 4) buffer
bufferRandSpeed {
	float randSpeed[];
};

layout(binding = 5) buffer
bufferTargetEnemyPosition {
	vec4 targetEnemyPos[];
};

layout(binding = 6) buffer
bufferTargetingCounts {
	int attackedCount[];
};

layout(binding = 7) buffer
bufferObjectDead {
	int isDead[];
};




#define State_Idle 0
#define State_Attack 1
#define State_Pain 2
#define State_Run 3
#define State_Death 4

uniform int herdOffset[32];
uniform int herdCounts[32];
uniform int herdSides[32];
uniform float dt;
uniform int herdCount;
float attackRange = 30.f;
float radius = 15.f;
int herdIndex;
uint index;

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



bool CheckCollisionWithEnemy()
{
	vec4 pos = objsPoses[index];
	int herdIndexOffset = 0;
	//uint closestEnemyIndexSoFar = 0;
	//float closestDistanceBtwEnemySoFar = 10000.f;
	int thisObjSide = herdSides[herdIndex];

	for (int i = 0; i < herdCount; ++i)
	{
		int herdNums = herdCounts[i];
		int otherHerdSide = herdSides[i];

		if (thisObjSide != otherHerdSide)
		{
			for (int j = 0; j < herdNums; ++j)
			{
				uint othersIndex = uint(herdIndexOffset + j);
				vec4 otherPos = objsPoses[othersIndex];
				int dead = isDead[othersIndex];

				if (dead == 0)
				{
					/*float dist = distance(pos, otherPos);

					if (dist < closestDistanceBtwEnemySoFar)
					{
						closestEnemyIndexSoFar = othersIndex;
						closestDistanceBtwEnemySoFar = dist;
					}*/

					if (CheckCollision(pos, otherPos, attackRange))
					{
						attackedCount[othersIndex]++;
						objsDirections[index] = otherPos - pos;
						return true;
					}

				}

			}
		}
		herdIndexOffset += herdNums;
	}

	//objsDirections[index] = objsPoses[closestEnemyIndexSoFar] - pos;
	return false;
}

bool CheckCollisionWithAllyInForthDirection(float speed, inout int allyCollisionIndex)
{
	vec4 direction = objsDirections[index];
	vec4 nextPos = objsPoses[index];
	MoveToward(nextPos, direction, speed);
	int herdIndexOffset = 0;
	int thisObjSide = herdSides[herdIndex];

	for (int i = 0; i < herdCount; ++i)
	{
		int herdNums = herdCounts[i];
		int otherObjSide = herdSides[i];

		if (thisObjSide == otherObjSide)
		{
			for (int j = 0; j < herdNums; ++j)
			{
				uint othersInSameHerdIndex = (herdIndexOffset + j);

				if (othersInSameHerdIndex != index)
				{
					int dead = isDead[othersInSameHerdIndex];
					vec4 otherPos = objsPoses[othersInSameHerdIndex];

					if (dead == 0)
					{
						if (CheckCollision(nextPos, otherPos, radius))
						{
							allyCollisionIndex = int(othersInSameHerdIndex);
							return true;
						}
					}
				}

			}
		}
		herdIndexOffset += herdNums;
	}
	return false;
}

void main(void)
{
	index = gl_GlobalInvocationID.x + gl_GlobalInvocationID.y * gl_NumWorkGroups.x * gl_WorkGroupSize.x;

	float timer = time[index];
	float speed = randSpeed[index];
	vec4 direction = objsDirections[index];

	GetBufferOffset();

	bool collisionWithEnemy = CheckCollisionWithEnemy();
	int allyCollisionIndex = 0;
	bool collisionWithAlly = CheckCollisionWithAllyInForthDirection(speed, allyCollisionIndex);


	if (collisionWithEnemy == false && collisionWithAlly == false)
	{
		animationIndex[index] = State_Run;
		MoveToward(objsPoses[index], direction, speed);
	}
	else
	{
		int animIndex = animationIndex[index];
		if (animIndex != State_Death)
		{
			if (collisionWithEnemy)
				animationIndex[index] = State_Attack;
			else if (collisionWithAlly)
			{
				if (animationIndex[allyCollisionIndex] != State_Run)
					animationIndex[index] = State_Idle;
			}
		}
	}
}