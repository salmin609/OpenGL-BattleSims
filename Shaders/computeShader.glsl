#version 430 core

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

layout(binding = 0) buffer
bufferInNodeTransforms {
	mat4 inNodeTransforms[];
};

layout(binding = 1) buffer
bufferOutNodeTransforms {
	mat4 outNodeTransforms[];
};

layout(binding = 2) buffer
bufferInTotalOrder {
	int orders[];
};

layout(binding = 3) buffer
bufferInOrderStartIndex {
	int startIndexes[];
};

layout(binding = 4) buffer
bufferInOrderEndIndex {
	int endIndexes[];
};

layout(binding = 5) buffer
bufferInContainAnimation {
	int containAnimations[];
};

layout(binding = 6) buffer
bufferInTotalScalingFactors {
	vec4 scalingKeys[];
};

layout(binding = 7) buffer
bufferInTotalTranslationFactors {
	vec4 translationKeys[];
};

layout(binding = 8) buffer
bufferInTotalRotationFactors {
	vec4 rotationKeys[];
};

layout(binding = 9) buffer
bufferInKeyFactorsStartIndex {
	int factorStartIndexes[];
};

layout(binding = 10) buffer
bufferInKeyFactorsEndIndex {
	int factorEndIndexes[];
};

layout(binding = 11) buffer
bufferInKeyFactorsTimeStamps {
	float factorTimeStamps[];
};

layout(binding = 12) buffer
bufferInOffsetMatrix {
	mat4 offsetMatrixes[];
};

layout(binding = 13) buffer
bufferInNodeContainOffsetMatrix {
	int containOffsetMatrixes[];
};

layout(binding = 14) buffer
bufferInBoneIndex {
	int boneIndexes[];
};

layout(binding = 15) buffer
bufferOutFinalTransform {
	mat4 finalTransforms[];
};


uniform int inTransformsSize;
uniform float animationTimeTicks;

mat4 identityMat = mat4(1.0, 0.0, 0.0, 0.0,  // 1. column
	0.0, 1.0, 0.0, 0.0,  // 2. column
	0.0, 0.0, 1.0, 0.0,  // 3. column
	0.0, 0.0, 0.0, 1.0); // 4. column

vec4 QuaternionInterpolate(vec4 pStart, vec4 pEnd, float factor)
{
	vec4 result;
	float cosom = pStart.x * pEnd.x + pStart.y * pEnd.y + pStart.z * pEnd.z + pStart.w * pEnd.w;

	vec4 end = pEnd;

	if (cosom < 0.0f)
	{
		cosom = -cosom;
		end.x = -end.x;
		end.y = -end.y;
		end.z = -end.z;
		end.w = -end.w;
	}

	float sclp, sclq;

	if (1.f - cosom > 0.0001f)
	{
		float omega, sinom;
		omega = acos(cosom); // extract theta from dot product's cos theta
		sinom = sin(omega);
		sclp = sin(1.0f - factor) * omega / sinom;
		sclq = sin(factor * omega) / sinom;
	}
	else
	{
		sclp = 1.f - factor;
		sclq = factor;
	}

	result.x = sclp * pStart.x + sclq * end.x;
	result.y = sclp * pStart.y + sclq * end.y;
	result.z = sclp * pStart.z + sclq * end.z;
	result.w = sclp * pStart.w + sclq * end.w;
	
	return result;
}

mat4 TranslationMatrix(vec4 translationFactor)
{
	mat4 result;

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			result[i][j] = 0.f;

			if (i == j)
				result[i][j] = 1.f;
		}
	}

	result[3][0] = translationFactor.x;
	result[3][1] = translationFactor.y;
	result[3][2] = translationFactor.z;

	return result;
}

mat4 ScalingMatrix(vec4 scalingFactor) 
{
	mat4 result;

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			result[i][j] = 0.f;

			if (i == j)
			{
				if (i == 0)
					result[i][j] = scalingFactor.x;

				else if (i == 1)
					result[i][j] = scalingFactor.y;

				else if (i == 2)
					result[i][j] = scalingFactor.z;

				else if (i == 3)
					result[i][j] = 1.f;
			}
		}
	}
	return result;
}

mat4 RotationMatrix(vec4 quaternion)
{
	mat4 resMatrix;
	float x = quaternion.x;
	float y = quaternion.y;
	float z = quaternion.z;
	float w = quaternion.w;


	resMatrix[0][0] = (1.0f) - (2.0f) * (y * y + z * z);
	resMatrix[1][0] = (2.0f) * (x * y - z * w);
	resMatrix[2][0] = (2.0f) * (x * z + y * w);
	resMatrix[3][0] = 0.f;

	resMatrix[0][1] = (2.0f) * (x * y + z * w);
	resMatrix[1][1] = (1.0f) - (2.0f) * (x * x + z * z);
	resMatrix[2][1] = (2.0f) * (y * z - x * w);
	resMatrix[3][1] = 0.f;

	resMatrix[0][2] = (2.0f) * (x * z - y * w);
	resMatrix[1][2] = (2.0f) * (y * z + x * w);
	resMatrix[2][2] = (1.0f) - (2.0f) * (x * x + y * y);
	resMatrix[3][2] = 0.f;

	resMatrix[0][3] = 0.f;
	resMatrix[1][3] = 0.f;
	resMatrix[2][3] = 0.f;
	resMatrix[3][3] = 1.f;

	return resMatrix;
}

int GetFactorIndex(int index)
{
	int factorStartIndex = factorStartIndexes[index];
	int factorEndIndex = factorEndIndexes[index];

	int timeIndex;
	//get time indexes
	for (int i = factorStartIndex; i < factorEndIndex - 1; i++)
	{
		float t = factorTimeStamps[i + 1];

		if (animationTimeTicks < t)
		{
			timeIndex = i;
			break;
		}
	}

	int factorIndex = timeIndex;

	return factorIndex;
}

float GetTimeFactor(int factorIndex)
{
	float t1 = factorTimeStamps[factorIndex];
	float t2 = factorTimeStamps[factorIndex + 1];
	float dt = t2 - t1;
	float timeFactor = (animationTimeTicks - t1) / dt;

	return timeFactor;
}

mat4 GetInterpolatedTransform(int index)
{
	mat4 interpolatedTransform;

	int factorIndex = GetFactorIndex(int(index));
	float timeFactor = GetTimeFactor(factorIndex);

	vec4 scalingKey = scalingKeys[factorIndex];
	vec4 nextScalingKey = scalingKeys[factorIndex + 1];
	vec4 scalingDelta = nextScalingKey - scalingKey;

	vec4 translationKey = translationKeys[factorIndex];
	vec4 nextTranslationKey = translationKeys[factorIndex + 1];
	vec4 translationDelta = nextTranslationKey - translationKey;

	vec4 rotationKey = rotationKeys[factorIndex];
	vec4 nextRotationKey = rotationKeys[factorIndex + 1];
	vec4 interpolationResult = QuaternionInterpolate(rotationKey, nextRotationKey, timeFactor);

	vec4 scalingResultVec = scalingKey + scalingDelta * timeFactor;
	vec4 translationResultVec = translationKey + translationDelta * timeFactor;
	interpolationResult = normalize(interpolationResult);

	mat4 scalingMat = ScalingMatrix(scalingResultVec);
	mat4 translationMat = TranslationMatrix(translationResultVec);
	mat4 rotationMat = RotationMatrix(interpolationResult);

	interpolatedTransform = translationMat * rotationMat * scalingMat;

	return interpolatedTransform;
}

mat4 GetParentMatrix(int startIndex, int endIndex)
{
	mat4 parentTransform = identityMat;

	for (int i = startIndex; i < endIndex - 1; ++i)
	{
		int parentIndex = orders[i];
		int parentNeedInterpolation = containAnimations[parentIndex];

		if (parentNeedInterpolation == 1)
			parentTransform = parentTransform * GetInterpolatedTransform(int(parentIndex));
		else
			parentTransform = parentTransform * inNodeTransforms[parentIndex];
	}
	return parentTransform;
}

void SetFinalTransforms(mat4 nodeTransform, int index)
{
	int containOffsetMatrix = containOffsetMatrixes[index];

	if (containOffsetMatrix == 1)
	{
		int boneIndex = boneIndexes[index];
		mat4 offsetMatrix = offsetMatrixes[index];

		finalTransforms[boneIndex] = nodeTransform * offsetMatrix;
	}
}

void main(void)
{
	uint index = gl_GlobalInvocationID.x + gl_GlobalInvocationID.y * gl_NumWorkGroups.x * gl_WorkGroupSize.x;
	
	if (index > inTransformsSize)
		return;

	int startIndex = startIndexes[index];
	int endIndex = endIndexes[index];
	int containAnimation = containAnimations[index];
	int currentNodeIndex = orders[endIndex - 1];

	mat4 nodeTransform;

	//Means need interpolation
	if (containAnimation == 1)
		nodeTransform = GetInterpolatedTransform(int(index));
	else
		nodeTransform = inNodeTransforms[currentNodeIndex];

	mat4 parentTransform = GetParentMatrix(startIndex, endIndex);
	
	//Apply parentTransform to nodeTransform
	nodeTransform = parentTransform * nodeTransform;

	//Return final transform which is applied offsetMatrix
	SetFinalTransforms(nodeTransform, int(index));
}