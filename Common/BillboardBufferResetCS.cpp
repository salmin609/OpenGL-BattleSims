/*
 * Author		: Ryan Kim.
 * Date			: 2023-03-17
 * Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#include "BillboardBufferResetCS.h"

#include <string>

#include "BillboardCollisionCheckCS.h"
#include "BillboardMovingCS.h"
#include "BillboardObjectManager.h"
#include "Buffer.hpp"
#include "BufferManager.h"
#include "CSBufferNames.h"
#include "DataTypes.h"
#include "Herd.h"
#include "HerdManager.h"
#include "Shader.h"

BillboardBufferResetCS::BillboardBufferResetCS(Shader* shader_, HerdManager* herdManager_,
                                               BillboardObjectManager* boObjManager_) : ComputeShaderClass(shader_), herdManager(herdManager_), boObjManager(boObjManager_)
{
	BillboardBufferResetCS::PopulateBuffers();
	BillboardBufferResetCS::SetShaderUniforms();
}

BillboardBufferResetCS::~BillboardBufferResetCS()
{
}

void BillboardBufferResetCS::ResetBuffer()
{
	SendBuffersAndUniforms(0.f);

	boObjManager->boMovingCS->csBuffers->
		GetBuffer(ToInt(MoveCS::herdAttackingCounts))->BindStorage(ToInt(ResetCS::herdAttackingCounts));

	boObjManager->boCollisionCheckCS->csBuffers->
		GetBuffer(ToInt(CollisionCheckCS::objsCollisionStatus))->BindStorage(ToInt(ResetCS::objsCollisionStatus));

	Dispatch(herdManager->totalRenderingAmount / 64);
}

void BillboardBufferResetCS::SetShaderUniforms()
{
	int& herdCount = herdManager->GetHerdCount();
	shader->AddUniformValues("herdCount", ShaderValueType::Int, &herdCount);

	for (int i = 0; i < herdCount; ++i)
	{
		Herd* herd = herdManager->GetHerd(i);
		herd->herdOffset = herdManager->herdOffset[i];

		const std::string uName = "herdOffset[" + std::to_string(i) + "]";
		shader->AddUniformValues(uName, ShaderValueType::Int, &herd->herdOffset);

		const std::string uName2 = "herdCounts[" + std::to_string(i) + "]";
		shader->AddUniformValues(uName2, ShaderValueType::Int, &herd->count);
	}
}

void BillboardBufferResetCS::PopulateBuffers()
{
}
