#version 430 core

layout(local_size_x = 64, local_size_y = 1, local_size_z = 1) in;


layout(binding = 0) writeonly buffer
bufferReached {
	int reached[];
};

layout(std430, binding = 1) buffer
bufferObjPos1 {
	vec4 obj1Pos[];
};

layout(std430, binding = 2) buffer
bufferObjPos2 {
	vec4 obj2Pos[];
};

//layout(std430, binding = 3) buffer
//herdPosBuffer {
//	vec4 herdPos[];
//};
//
//layout(std430, binding = 4) buffer
//otherHerdPosBuffer {
//	vec4 otherHerdPos[];
//};



uniform vec4 herdBoDirectionAndOffsets[32];
uniform float dt;
uniform int herdCount;
vec3 boDirection;
float speed = 18.f;
float distanceCheck = 20.f;
#define MAX_COUNT_PER_HERD 1280

void CopyToHerdPosArray(int posBufferIndex, inout vec4 herdPos[MAX_COUNT_PER_HERD], inout vec4 otherHerdPos[MAX_COUNT_PER_HERD])
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

bool CheckReached(vec4 pos, vec4 otherPos)
{
	float distance = distance(pos, otherPos);

	if (distance < distanceCheck)
		return true;
	return false;
}

void GetBufferOffset(inout int posBufferIndex, inout int bufferOffset, inout uint index)
{
	for (int i = herdCount - 1; i >= 0; --i)
	{
		vec4 herdboDirectionAndOffset = herdBoDirectionAndOffsets[i];

		if (index >= herdboDirectionAndOffset.w)
		{
			boDirection = vec3(herdboDirectionAndOffset);
			posBufferIndex = i;
			bufferOffset = int(herdboDirectionAndOffset.w);
			break;
		}
	}
	index -= bufferOffset;
}

bool CheckCollisionWithEnemy(vec4 pos, vec4 otherHerdPos[MAX_COUNT_PER_HERD])
{
	for (int i = 0; i < MAX_COUNT_PER_HERD; ++i)
	{
		vec4 otherPos = otherHerdPos[i];

		if (CheckReached(pos, otherPos))
			return true;
	}
	return false;
}

bool CheckCollisionWithAllie(uint index, vec4 herdPos[MAX_COUNT_PER_HERD])
{



}

void main(void)
{
	uint index = gl_GlobalInvocationID.x + gl_GlobalInvocationID.y * gl_NumWorkGroups.x * gl_WorkGroupSize.x;
	uint reachedBufferIndex = index;

	reached[reachedBufferIndex] = 0;
	
	int posBufferIndex = 0;
	int bufferOffset = 0;

	vec4 herdPos[MAX_COUNT_PER_HERD];
	vec4 otherHerdPos[MAX_COUNT_PER_HERD];

	GetBufferOffset(posBufferIndex, bufferOffset, index);
	CopyToHerdPosArray(posBufferIndex, herdPos, otherHerdPos);	

	vec4 pos = herdPos[index];

	bool doesReached = CheckCollisionWithEnemy(index, otherHerdPos, herdPos);

	//Need to move toward facing direction.
	if (doesReached == false)
	{
		if (posBufferIndex == 0)
			MoveToward(obj1Pos[index], vec4(boDirection, 1.f), speed);
		else if (posBufferIndex == 1)
			MoveToward(obj2Pos[index], vec4(boDirection, 1.f), speed);
	}
	//Change animation
	else
		reached[reachedBufferIndex] = 1;
}
