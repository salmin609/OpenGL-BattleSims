#version 430 core

layout(local_size_x = 64, local_size_y = 1, local_size_z = 1) in;


layout(binding = 0) buffer
bufferReached {
	int animationIndex[];
};

layout(std430, binding = 1) buffer
bufferObjPos1 {
	vec4 obj1Pos[];
};

layout(std430, binding = 2) buffer
bufferObjPos2 {
	vec4 obj2Pos[];
};

layout(std430, binding = 3) buffer
bufferObjDirection1 {
	vec4 herdDirection1[];
};

layout(std430, binding = 4) buffer
bufferObjDirection2 {
	vec4 herdDirection2[];
};

layout(binding = 5) buffer
bufferTimeCheck {
	float time[];
};

layout(binding = 6) buffer
bufferRandSpeed {
	float randSpeed[];
};

layout(binding = 7) buffer
bufferTargetEnemyPosition {
	vec4 targetEnemyPos[];
};

layout(binding = 8) buffer
bufferTargetingCounts {
	int attackedCount[];
};

layout(binding = 9) buffer
bufferPrevState {
	int prevAnimationStates[];
};

#define State_Idle 0
#define State_Attack 1
#define State_Pain 2
#define State_Run 3
#define State_Death 4

uniform int herdOffset[32];
uniform float dt;
uniform int herdCount;
float attackRange = 15.f;
float radius = 10.f;
int posBufferIndex = 0;


#define MAX_COUNT_PER_HERD 1280

void CopyToHerdPosArray(inout vec4 herdPos[MAX_COUNT_PER_HERD], inout vec4 otherHerdPos[MAX_COUNT_PER_HERD])
{
	if (posBufferIndex == 0)
	{
		for (int i = 0; i < MAX_COUNT_PER_HERD; ++i)
		{
			herdPos[i] = obj1Pos[i];
			otherHerdPos[i] = obj2Pos[i];
		}
	}
	else if(posBufferIndex == 1)
	{
		for (int i = 0; i < MAX_COUNT_PER_HERD; ++i)
		{
			herdPos[i] = obj2Pos[i];
			otherHerdPos[i] = obj1Pos[i];
		}
	}
	
}

void MoveToward(inout vec4 pos, vec4 direction, float moveSpeed)
{
	pos = pos + direction * dt * moveSpeed;
	pos.w = 1.f;
}

bool CheckCollision(vec4 pos, vec4 otherPos, float range)
{
	float distance = distance(pos, otherPos);

	if (distance < range)
		return true;
	return false;
}

void GetBufferOffset(inout uint index)
{
	for (int i = herdCount - 1; i >= 0; --i)
	{
		int offset = herdOffset[i];

		if (index >= offset)
		{
			posBufferIndex = i;
			index -= offset;
			break;
		}
	}
}



bool CheckCollisionWithEnemy(vec4 pos, inout vec4 otherHerdPos[MAX_COUNT_PER_HERD], inout vec4 direction)
{
	for (int i = 0; i < MAX_COUNT_PER_HERD; ++i)
	{
		vec4 otherPos = otherHerdPos[i];

		if (CheckCollision(pos, otherPos, attackRange))
		{
			if (posBufferIndex == 0)
			{
				attackedCount[i + MAX_COUNT_PER_HERD]++;
			}
			else if (posBufferIndex == 1) 
			{
				attackedCount[i]++;
			}


			direction = otherPos - pos;
			return true;
		}
	}
	return false;
}

bool CheckCollisionWithAllyInForthDirection(uint index, vec4 direction, inout vec4 herdPos[MAX_COUNT_PER_HERD], float speed, inout int collisionIndex)
{
	vec4 nextPos = herdPos[index];
	MoveToward(nextPos, direction, speed);
	
	for (int i = 0; i < MAX_COUNT_PER_HERD; ++i)
	{
		if (i != index)
		{
			int animationState;
			if (posBufferIndex == 0)
				animationState = animationIndex[index];
			else if (posBufferIndex == 1)
				animationState = animationIndex[index + MAX_COUNT_PER_HERD];
				
			vec4 otherPos = herdPos[i];

			if(otherPos.y > 0.f)
				if (CheckCollision(nextPos, otherPos, radius))
				{
					collisionIndex = i;
					return true;
				}
		}
	}
	return false;
}

void FindNearEnemy(inout vec4 direction, vec4 pos, vec4 otherHerdPos[MAX_COUNT_PER_HERD])
{
	float shortestDistanceSoFar = 1000.f;
	int shortestOneIndex = 0;
	for (int i = 0; i < MAX_COUNT_PER_HERD; ++i)
	{
		vec4 otherPos = otherHerdPos[i];

		float distance = distance(pos, otherPos);

		if (distance < shortestDistanceSoFar)
		{
			shortestDistanceSoFar = distance;
			shortestOneIndex = i;
		}
	}

	vec4 shortestOnePos = otherHerdPos[shortestOneIndex + 1];

	vec4 currentToShortestOne = shortestOnePos - pos;
	direction = currentToShortestOne;
}


void main(void)
{
	uint index = gl_GlobalInvocationID.x + gl_GlobalInvocationID.y * gl_NumWorkGroups.x * gl_WorkGroupSize.x;
	uint globalIndex = index;
	prevAnimationStates[globalIndex] = animationIndex[globalIndex];
	
	float timer = time[globalIndex];

	if (timer < 5.f)
	{
		vec4 herdPos[MAX_COUNT_PER_HERD];
		vec4 otherHerdPos[MAX_COUNT_PER_HERD];
		float speed = randSpeed[globalIndex];


		GetBufferOffset(index);
		CopyToHerdPosArray(herdPos, otherHerdPos);	

		vec4 direction;

		if (posBufferIndex == 0)
			direction = herdDirection1[index];
		else
			direction = herdDirection2[index];

		vec4 pos = herdPos[index];

		bool collisionWithEnemy;

		if (posBufferIndex == 0)
			collisionWithEnemy = CheckCollisionWithEnemy(pos, otherHerdPos, herdDirection1[index]);
		else if (posBufferIndex == 1)
			collisionWithEnemy = CheckCollisionWithEnemy(pos, otherHerdPos, herdDirection2[index]);


		int allyCollisionIndex = 0;
		bool collisionWithAlly = CheckCollisionWithAllyInForthDirection(index, direction, herdPos, speed, allyCollisionIndex);

		if (collisionWithEnemy == false && collisionWithAlly == false)
		{
			animationIndex[globalIndex] = State_Run;
			if (posBufferIndex == 0)
				MoveToward(obj1Pos[index], direction, speed);
			else if (posBufferIndex == 1)
				MoveToward(obj2Pos[index], direction, speed);
		}
		else
		{
			//reached[reachedBufferIndex] = 1;
			int animIndex = animationIndex[globalIndex];
			if (animIndex != State_Death)
			{
				if (collisionWithEnemy)
					animationIndex[globalIndex] = State_Attack;
				else if (collisionWithAlly)
				{
					if (posBufferIndex == 1)
						allyCollisionIndex += 1280;

					if(animationIndex[allyCollisionIndex] != State_Run)
						animationIndex[globalIndex] = State_Idle;
				}
			}
		}
	}

}
