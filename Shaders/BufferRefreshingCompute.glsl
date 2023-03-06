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

void main(void)
{
	uint index = gl_GlobalInvocationID.x + 
		gl_GlobalInvocationID.y * gl_NumWorkGroups.x * gl_WorkGroupSize.x;

	herdAttackingCounts[index] = 0;
	objsCollisionStatus[index] = 0;
}