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
TestBuffer {
	vec4 check[];
};

uniform vec4 herdBoDirectionAndOffsets[32];
uniform float dt;
uniform int herdCount;


void main(void)
{
	uint index = gl_GlobalInvocationID.x + gl_GlobalInvocationID.y * gl_NumWorkGroups.x * gl_WorkGroupSize.x;

	vec4 boPosInVec4;

	int posBufferIndex;
	int bufferOffset;
	for (int i = herdCount - 1; i >= 0; --i)
	{
		vec4 herdboDirectionAndOffset = herdBoDirectionAndOffsets[i];

		if (index >= herdboDirectionAndOffset.w)
		{
			posBufferIndex = i;
			bufferOffset = int(herdboDirectionAndOffset.w);
			break;
		}
	}
}
