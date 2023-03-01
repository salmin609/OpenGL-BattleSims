#version 430 core

layout(local_size_x = 64, local_size_y = 1, local_size_z = 1) in;


layout(binding = 0) buffer
bufferAnimationIndices
{
	int animationIndices[];
};

layout(std430, binding = 1) buffer
bufferObjsPos
{
	vec4 objsPoses[];
};

layout(std430, binding = 2) buffer
bufferObjsDirection
{
	vec4 objsDirections[];
};


layout(binding = 3) buffer
bufferTimeCheck
{
	float time[];
};

layout(binding = 4) buffer
bufferTargetEnemyPosition
{
	vec4 targetEnemyPos[];
};

layout(binding = 5) buffer
bufferTargetingCounts
{
	int attackedCount[];
};

layout(binding = 6) buffer
bufferObjectDead
{
	int isDead[];
};

layout(binding = 7) buffer
bufferHerdReachedDestination
{
	int herdReachedDestination[];
};

layout(binding = 8) buffer
bufferHerdAttackingCount
{
	int herdAttackingCounts[];
};

layout(binding = 9) buffer
bufferObjsCollisionStatus
{
	int objsCollisionStatus[];
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
uniform float herdAttackRanges[32];
uniform int herdAttackTypes[32];
uniform int herdWidths[32];

uniform float dt;
uniform int herdCount;
float attackRange;
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

	float timer = time[index];
	vec4 direction = objsDirections[index];
	int animIndex = animationIndices[index];

	GetBufferOffset();
	vec4 herdDirection = herdDirections[herdIndex];
	float speed = herdSpeeds[herdIndex];
	attackRange = herdAttackRanges[herdIndex];

	int dead = isDead[index];
	bool isStop = dot(herdDirection, herdDirection) == 0;
	int attackType = herdAttackTypes[herdIndex];


	if (dead == 0)
	{
		if (isStop == false)
		{
			int allyCollisionIndex = 0;
			int collisionState = objsCollisionStatus[index];

			if (collisionState > 0)
			{
				allyCollisionIndex = collisionState - Collision_With_Ally;
				collisionState = Collision_With_Ally;
			}

			if (collisionState == Collision_With_Enemy)
			{
				if (attackType == Attack_Melee)
				{
					animationIndices[index] = State_Attack;
				}
				else
				{
					int herdNums = herdCounts[herdIndex];

					//all herd members attacking
					if (herdAttackingCounts[herdIndex] == herdNums)
					{
						animationIndices[index] = State_Attack;
					}
					else
					{
						animationIndices[index] = State_Run;
						objsDirections[index] = herdDirection;
						MoveToward(objsPoses[index], herdDirection, speed);

						float dist = distance(objsPoses[index], herdDestinations[herdIndex]);

						if (dist < 10.f)
							herdReachedDestination[herdIndex] = 1;
					}
				}
			}
			else
			{
				if (collisionState == Collision_With_Ally)
				{
					if (animationIndices[allyCollisionIndex] != State_Run)
						animationIndices[index] = State_Idle;
				}
				else
				{
					animationIndices[index] = State_Run;
					objsDirections[index] = herdDirection;
					MoveToward(objsPoses[index], herdDirection, speed);

					float dist = distance(objsPoses[index], herdDestinations[herdIndex]);

					if (dist < 10.f)
						herdReachedDestination[herdIndex] = 1;
				}
			}
		}
		else
		{
			int collisionState = objsCollisionStatus[index];

			if (collisionState == Collision_With_Enemy)
			{
				animationIndices[index] = State_Attack;
			}
			else
			{
				animationIndices[index] = State_Idle;
			}
		}
	}

}