#version 430 core

layout(local_size_x = 64, local_size_y = 1, local_size_z = 1) in;

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

layout(binding = 0) buffer
bufferAnimationIndex {
	int animationIndices[];
};

layout(binding = 1) buffer
bufferObjectDead
{
	int isDead[];
};

layout(binding = 2) buffer
bufferObjsCollisionStatus
{
	int objsCollisionStatus[];
};

layout(binding = 3) buffer
bufferHerdAttackingCount
{
	int herdAttackingCounts[];
};

uniform int herdAttackTypes[32];
uniform int herdOffset[32];
uniform int herdCounts[32];
uniform vec4 herdDirections[32];
uniform int herdCount;

uint index;
int herdIndex;

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

void main()
{
	index = gl_GlobalInvocationID.x + gl_GlobalInvocationID.y * gl_NumWorkGroups.x * gl_WorkGroupSize.x;

	GetBufferOffset();

	int thisCollisionState = objsCollisionStatus[index];
	int thisAnimationState = animationIndices[index];
	int dead = isDead[index];
	int attackType = herdAttackTypes[herdIndex];
	vec4 herdDirection = herdDirections[herdIndex];
	bool isStop = dot(herdDirection, herdDirection) == 0;


	if (dead == 0)
	{
		if (thisAnimationState != State_Death)
		{
			int allyCollisionIndex = 0;

			//if it over 0, means collision with allies, need to get allycollision index
			if (thisCollisionState > 0)
			{
				allyCollisionIndex = thisCollisionState - Collision_With_Ally;
				thisCollisionState = Collision_With_Ally;
			}

			if (thisCollisionState == Collision_With_Enemy)
			{
				//animationIndices[index] = State_Attack;
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
						if(isStop)
							animationIndices[index] = State_Idle;
						else 
							animationIndices[index] = State_Run;
					}
				}
			}
			else
			{
				if (thisCollisionState == Collision_With_Ally)
				{
					if (animationIndices[allyCollisionIndex] != State_Run)
						animationIndices[index] = State_Idle;
				}
				else
				{
					if (isStop)
						animationIndices[index] = State_Idle;
					else
						animationIndices[index] = State_Run;
				}
			}
		}

	}
	
}