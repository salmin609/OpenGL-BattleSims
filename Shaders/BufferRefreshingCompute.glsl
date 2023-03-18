#version 430 core

layout(local_size_x = 64, local_size_y = 1, local_size_z = 1) in;

layout(binding = 0) writeonly buffer
bufferHerdAttackingCount
{
	int herdAttackingCounts[];
};

layout(std430, binding = 1) writeonly buffer
bufferObjsCollisionStatus
{
	int objsCollisionStatus[];
};

//layout(std430, binding = 2) writeonly buffer
//bufferHerdMovable
//{
//	int herdMovable[];
//};

uint index;
uniform int herdCount;

uniform int herdCounts[32];
uniform int herdOffset[32];

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

void main(void)
{
	index = gl_GlobalInvocationID.x + 
		gl_GlobalInvocationID.y * gl_NumWorkGroups.x * gl_WorkGroupSize.x;

	GetBufferOffset();

	//herdMovable[herdIndex] = 1;

	herdAttackingCounts[index] = 0;
	objsCollisionStatus[index] = 0;
}