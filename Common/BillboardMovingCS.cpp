/*
 * Author		: Ryan Kim.
 * Date			: 2023-03-17
 * Copyright � 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#include "BillboardMovingCS.h"

#include "BillboardAttackCS.h"
#include "BillboardCollisionCheckCS.h"
#include "BillboardObjectManager.h"
#include "Buffer.hpp"
#include "BufferManager.h"
#include "Herd.h"
#include "HerdManager.h"
#include "Shader.h"
#include "glm/vec4.hpp"
#include "CSBufferNames.h"
#include "ModelKinds.h"

BillboardMovingCS::BillboardMovingCS(Shader* boMovingShader_, HerdManager* herdManager_,
                                     BillboardObjectManager* boObjManager_): ComputeShaderClass(boMovingShader_), boObjManager(boObjManager_)
{
	herdManager = herdManager_;
	BillboardMovingCS::PopulateBuffers();
	BillboardMovingCS::SetShaderUniforms();
	animationIndexBuffer = new int[herdManager->totalRenderingAmount];
	herdReachedBuffer = new int[herdManager->GetHerdCount()];
}

BillboardMovingCS::~BillboardMovingCS()
{
	delete[] animationIndexBuffer;
	delete[] herdReachedBuffer;
}

void BillboardMovingCS::SetShaderUniforms()
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

		const std::string uName3 = "herdSides[" + std::to_string(i) + "]";
		shader->AddUniformValues(uName3, ShaderValueType::Int, &herd->side);

		const std::string uName4 = "herdDirections[" + std::to_string(i) + "]";
		shader->AddUniformValues(uName4, ShaderValueType::Vec4, &herd->herdDirection);

		const std::string uName5 = "herdSpeeds[" + std::to_string(i) + "]";
		shader->AddUniformValues(uName5, ShaderValueType::Float, &herd->speed);

		const std::string uName6 = "herdDestinations[" + std::to_string(i) + "]";
		shader->AddUniformValues(uName6, ShaderValueType::Vec4, &herd->herdDestination);

		const std::string uName7 = "herdAttackRanges[" + std::to_string(i) + "]";
		shader->AddUniformValues(uName7, ShaderValueType::Float, &herd->attackRange);

		const std::string uName8 = "herdAttackTypes[" + std::to_string(i) + "]";
		shader->AddUniformValues(uName8, ShaderValueType::Int, &herd->attackType);

		const std::string uName9 = "herdWidths[" + std::to_string(i) + "]";
		shader->AddUniformValues(uName9, ShaderValueType::Int, &herd->herdWidth);
	}
}


void BillboardMovingCS::PopulateBuffers()
{
	const int herdCount = herdManager->GetHerdCount();

	std::vector<int> herdReachedDest;

	for(int i = 0; i < herdCount; ++i)
	{
		herdReachedDest.push_back(0);
	}

	csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(int) * herdCount,
		GL_DYNAMIC_DRAW, herdReachedDest.data(), ToInt(MoveCS::herdReachedDestination)));

	csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(int) * herdCount,
		GL_DYNAMIC_DRAW, herdReachedDest.data(), ToInt(MoveCS::herdAttackingCounts)));
}

void BillboardMovingCS::Move(float dt) const
{
	SendBuffersAndUniforms(dt);

	boObjManager->boAttackCS->csBuffers->GetBuffer(ToInt(AttackCS::isDead))->
		BindStorage(ToInt(MoveCS::isDead));
	boObjManager->boCollisionCheckCS->csBuffers->GetBuffer(ToInt(CollisionCheckCS::objsCollisionStatus))->
		BindStorage(ToInt(MoveCS::objsCollisionStatus));

	herdManager->posBuffer->BindStorage(ToInt(MoveCS::objsPoses));
	herdManager->directionBuffer->BindStorage(ToInt(MoveCS::objsDirections));

	Dispatch(herdManager->totalRenderingAmount / 64);
}

