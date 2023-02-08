#version 430 core

layout(local_size_x = 128, local_size_y = 1, local_size_z = 1) in;

layout(binding = 0) buffer
bufferOutFrameBufferUsage {
	int frameBufferUsingIndex[];
};

layout(binding = 1) buffer
bufferObjPos1 {
	vec4 obj1Pos[];
};

layout(binding = 2) buffer
bufferObjPos2 {
	vec4 obj2Pos[];
};

layout(binding = 3) buffer
bufferObjPos3 {
	vec4 obj3Pos[];
};

layout(binding = 4) buffer
bufferObjPos4 {
	vec4 obj4Pos[];
};

layout(binding = 5) buffer
bufferHerdDirection1 {
	vec4 herdDirection1[];
};

layout(binding = 6) buffer
bufferHerdDirection2 {
	vec4 herdDirection2[];
};

uniform int herdOffset[32];
uniform int herdCount;
uniform vec3 camPos;
uniform vec3 camFront;
uniform vec3 camRight;
uniform vec3 camUp;
uniform float aspect;
uniform float fovY;
uniform float zNear;
uniform float zFar;
uniform int bufferSize;


struct Plan 
{
	vec3 normal;
	float distance;
};

struct Frustum
{
	Plan topFace;
	Plan bottomFace;

	Plan rightFace;
	Plan leftFace;

	Plan farFace;
	Plan nearFace;
};

struct SphereBV {
	vec3 center;
	float radius;
};


Plan GetPlan(vec3 p1, vec3 norm);
float GetSignedDistanceToPlan(Plan plan, vec3 pos);
Frustum GetFrustumPlans();
SphereBV GetSPV(vec3 pos, float rad);
bool isOnOrForwardPlan(Plan plan, SphereBV spv);
bool isOnFrustum(Frustum camFrustum, SphereBV spv);

Plan GetPlan(vec3 p1, vec3 norm)
{
	Plan result;
	result.normal = normalize(norm);
	result.distance = dot(result.normal, p1);

	return result;
}

Frustum GetFrustumPlans()
{
	Frustum result;

	float halfVside = zFar * tan(fovY * 0.5f);
	float halfHside = halfVside * aspect;
	vec3 frontMultFar = zFar * camFront;

	result.nearFace = GetPlan(camPos + zNear * camFront, camFront);
	result.farFace = GetPlan(camPos + frontMultFar, -camFront);
	result.rightFace = GetPlan(camPos, 
		cross(camUp, frontMultFar + camRight * halfHside));
	result.leftFace = GetPlan(camPos, 
		cross(frontMultFar - camRight * halfHside, camUp));
	result.topFace = GetPlan(camPos, 
		cross(camRight, frontMultFar - camUp * halfVside));
	result.bottomFace = GetPlan(camPos, 
		cross(frontMultFar + camUp * halfVside, camRight));

	return result;
}

SphereBV GetSPV(vec3 pos, float rad)
{
	SphereBV spv;
	spv.center = pos;
	spv.radius = rad;

	return spv;
}

float GetSignedDistanceToPlan(Plan plan, vec3 pos)
{
	return dot(plan.normal, pos) - plan.distance;
}

bool isOnOrForwardPlan(Plan plan, SphereBV spv)
{
	return GetSignedDistanceToPlan(plan, spv.center) > -spv.radius;
}

bool isOnFrustum(Frustum camFrustum, SphereBV spv)
{
	return (isOnOrForwardPlan(camFrustum.leftFace, spv) &&
		isOnOrForwardPlan(camFrustum.rightFace, spv) &&
		isOnOrForwardPlan(camFrustum.farFace, spv) &&
		isOnOrForwardPlan(camFrustum.nearFace, spv) &&
		isOnOrForwardPlan(camFrustum.topFace, spv) &&
		isOnOrForwardPlan(camFrustum.bottomFace, spv));
}


float pi = 3.14159f;

float Convert(float radian)
{
	return (radian * (180.f / pi));
}

void GetBufferOffset(inout int posBufferIndex, inout uint index)
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

int GetUsingFrameBufferIndex(vec3 boPos, vec3 boDirection)
{
	int result = 0;
	vec3 boToCam = camPos - boPos;
	boToCam.y = 0.f;

	float cosTheta = dot(boToCam, boDirection) / (length(boToCam) * length(boDirection));
	float angle = acos(cosTheta);
	float angleInDegree = Convert(angle);

	vec3 camDir = -boDirection;
	vec3 targetDir = boPos - camPos;
	vec3 crossResult = cross(camDir, targetDir);


	if (angleInDegree >= 0.f && angleInDegree <= 22.5f)
		result = 0;
	else if (angleInDegree >= 157.5f && angleInDegree <= 180.f)
		result = 1;
	else
	{
		if (crossResult.y < 0)
		{
			if (angleInDegree >= 22.5f && angleInDegree <= 67.5f)
				result = 4;
			else if (angleInDegree >= 112.5f && angleInDegree <= 157.5f)
				result = 6;
			else
				result = 2;
		}
		else
		{
			if (angleInDegree >= 22.5f && angleInDegree <= 67.5f)
				result = 5;
			else if (angleInDegree >= 112.5f && angleInDegree <= 157.5f)
				result = 7;
			else
				result = 3;
		}
	}

	return result;
}

void GetBoPosAndDirection(int posBufferIndex, uint index, inout vec3 boPos, inout vec3 boDirection)
{
	if (posBufferIndex == 0)
	{
		boPos = vec3(obj1Pos[index]);
		boDirection = vec3(herdDirection1[index]);
	}
	else if (posBufferIndex == 1)
	{
		boPos = vec3(obj2Pos[index]);
		boDirection = vec3(herdDirection2[index]);
	}
	else if (posBufferIndex == 2)
	{
		boPos = vec3(obj3Pos[index]);
	}
	else if (posBufferIndex == 3)
	{
		boPos = vec3(obj4Pos[index]);
	}
}

void main(void)
{
	uint index = gl_GlobalInvocationID.x + gl_GlobalInvocationID.y * gl_NumWorkGroups.x * gl_WorkGroupSize.x;
	uint wholeBufferIndex = index;

	frameBufferUsingIndex[index] = -1;

	vec3 boPos;
	int posBufferIndex = 0;
	vec3 boDirection;

	GetBufferOffset(posBufferIndex, index);
	GetBoPosAndDirection(posBufferIndex, index, boPos, boDirection);

	float distance = distance(boPos, camPos);

	if (distance < 1000.f)
	{
		Frustum camFrustum = GetFrustumPlans();
		SphereBV spv = GetSPV(boPos, 0.1f);

		if (isOnFrustum(camFrustum, spv))
			frameBufferUsingIndex[wholeBufferIndex] = GetUsingFrameBufferIndex(boPos, boDirection);
	}
}