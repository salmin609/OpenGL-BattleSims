#version 430 core

layout(local_size_x = 128, local_size_y = 1, local_size_z = 1) in;


layout(binding = 1) buffer
bufferObjPos1 {
	vec4 obj1Pos[];
};

layout(binding = 2) buffer
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

#define MAX_COUNT_PER_HERD 1280




//void CopyArrayToOtherPos(inout int destination[MAX_COUNT_PER_HERD], inout int source[MAX_COUNT_PER_HERD])
//{
//	for (int i = 0; i < MAX_COUNT_PER_HERD; i++)
//	{
//		otherPos[i] = pos[i];
//	}
//}

bool CheckReached()
{
	return true;
}


void main(void)
{
	uint index = gl_GlobalInvocationID.x + gl_GlobalInvocationID.y * gl_NumWorkGroups.x * gl_WorkGroupSize.x;

	vec4 boPosInVec4;
	reached[index] = 0;

	int posBufferIndex;
	int bufferOffset;
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

	bool isReached = false;

	vec4 pos;

	if (posBufferIndex == 0)
	{
		pos = obj1Pos[index];

		//obj1Pos
		for (int i = 0; i < 1280; ++i)
		{
			vec4 otherHerdPos = obj2Pos[i];

			float distance = distance(pos, otherHerdPos);

			if (distance < 20.f)
			{
				reached[index] = 1;
				isReached = true;
				break;
			}
		}
		if (isReached == false)
		{
			obj1Pos[index] = pos + vec4(boDirection, 1.f) * dt * speed;
			obj1Pos[index].w = 1.f;
		}

	}
	else if (posBufferIndex == 1)
	{
		uint newIndex = index - bufferOffset;
		pos = obj2Pos[newIndex];

		for (int i = 0; i < 1280; ++i)
		{
			vec4 otherHerdPos = obj1Pos[i];

			float distance = distance(pos, otherHerdPos);

			if (distance < 20.f)
			{
				reached[index] = 1;
				isReached = true;
				break;
			}
		}

		if (isReached == false)
		{
			//obj2Pos

			obj2Pos[newIndex] = pos + vec4(boDirection, 1.f) * dt * speed;
			obj2Pos[newIndex].w = 1.f;
		}
	}
}
