#version 430 core

layout(local_size_x = 128, local_size_y = 1, local_size_z = 1) in;


layout(std430, binding = 1) buffer
bufferObjPos1 {
	vec4 obj1Pos[];
};

layout(std430, binding = 2) buffer
bufferObjPos2 {
	vec4 obj2Pos[];
};

layout(binding = 3) buffer
bufferReached {
	int reached[];
};

uniform vec4 herdBoDirectionAndOffsets[32];
uniform float dt;
uniform int herdCount;
vec3 boDirection;
float speed = 18.f;
float distanceCheck = 20.f;
#define MAX_COUNT_PER_HERD 1280




void CopyArrayToOtherArray(inout vec4 destination[MAX_COUNT_PER_HERD], inout vec4 source[MAX_COUNT_PER_HERD])
{
	for (int i = 0; i < MAX_COUNT_PER_HERD; i++)
	{
		destination[i] = source[i];
	}
}

void MoveTowardBODirection(inout vec4 pos, vec4 direction, float moveSpeed)
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


void main(void)
{
	uint index = gl_GlobalInvocationID.x + gl_GlobalInvocationID.y * gl_NumWorkGroups.x * gl_WorkGroupSize.x;
	uint reachedBufferIndex = index;

	vec4 boPosInVec4;
	reached[index] = 0;
	
	int posBufferIndex;
	int bufferOffset = 0;
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

	vec4 herdPos[MAX_COUNT_PER_HERD];
	vec4 otherHerdPos[MAX_COUNT_PER_HERD];
	
	if (posBufferIndex == 0)
	{
		CopyArrayToOtherArray(herdPos, obj1Pos);
		CopyArrayToOtherArray(otherHerdPos, obj2Pos);
	}
	else
	{
		CopyArrayToOtherArray(herdPos, obj2Pos);
		CopyArrayToOtherArray(otherHerdPos, obj1Pos);
	}

	vec4 pos = herdPos[index];
	bool doesReached = false;

	for (int i = 0; i < MAX_COUNT_PER_HERD; ++i)
	{
		vec4 otherPos = otherHerdPos[i];

		if (CheckReached(pos, otherPos))
		{
			doesReached = true;
			reached[reachedBufferIndex] = 1;
			break;
		}
	}
	if (doesReached == false)
	{
		if(posBufferIndex == 0)
			MoveTowardBODirection(obj1Pos[index], vec4(boDirection, 1.f), speed);
		else if (posBufferIndex == 1)
			MoveTowardBODirection(obj2Pos[index], vec4(boDirection, 1.f), speed);

	}
}
