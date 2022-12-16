/*
 * Author		: Ryan Kim.
 * Date			: 2022-10-07
 * Description	: Structs for store
 *				  BoneInfo : store offset between bones, finalTransformation
 *				  BasicMeshEntry: store number of total indices, baseVertex, index, materialIndex
 *				  to use in glDrawCall();
 *
 *				  Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#pragma once

#include <glm/mat4x4.hpp>

#define INVALID_MATERIAL 0xFFFFFFFF

struct BasicMeshEntry {
	BasicMeshEntry()
	{
		NumIndices = 0;
		BaseVertex = 0;
		BaseIndex = 0;
		MaterialIndex = INVALID_MATERIAL;
	}
	unsigned int NumIndices;
	unsigned int BaseVertex;
	unsigned int BaseIndex;
	unsigned int MaterialIndex;
};
struct BoneInfo
{
	glm::mat4 offsetMat;
	glm::mat4 finalTransform;

	BoneInfo(const glm::mat4& Offset)
	{
		offsetMat = Offset;
		finalTransform = glm::mat4(0.f);
		//FinalTransformation.SetZero();
	}
};