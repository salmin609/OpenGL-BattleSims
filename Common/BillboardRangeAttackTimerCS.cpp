/*
 * Author		: Ryan Kim.
 * Date			: 2023-03-17
 * Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */
#include "BillboardRangeAttackTimerCS.h"

#include "BillboardAnimationChangeCS.h"
#include "BillboardMovingCS.h"
#include "BillboardObjectManager.h"
#include "Buffer.hpp"
#include "BufferManager.h"
#include "CSBufferNames.h"
#include "Herd.h"
#include "HerdManager.h"
#include "Shader.h"

BillboardRangeAttackTimerCS::BillboardRangeAttackTimerCS(Shader* boRangeAttackTimer_, HerdManager* herdManager_,
                                                         BillboardObjectManager* boObjManager_):
	ComputeShaderClass(boRangeAttackTimer_), herdManager(herdManager_), boObjManager(boObjManager_)
{
	BillboardRangeAttackTimerCS::PopulateBuffers();
	BillboardRangeAttackTimerCS::SetShaderUniforms();
}

BillboardRangeAttackTimerCS::~BillboardRangeAttackTimerCS()
{
}

void BillboardRangeAttackTimerCS::TimeCheck(float dt) const
{
	SendBuffersAndUniforms(dt);

	herdManager->posBuffer->BindStorage(ToInt(RangeAttackTimerCS::objsPoses));

	boObjManager->boAnimChangeCS->csBuffers->GetBuffer(ToInt(AnimationChangeCS::animationIndices))
		->BindStorage(ToInt(RangeAttackTimerCS::animationIndices));

	Dispatch(herdManager->totalRenderingAmount / 64);
}

void BillboardRangeAttackTimerCS::SetShaderUniforms()
{
	int& herdCount = herdManager->GetHerdCount();
	shader->AddUniformValues("herdCount", ShaderValueType::Int, &herdCount);

	for (int i = 0; i < herdCount; ++i)
	{
		Herd* herd = herdManager->GetHerd(i);
		herd->herdOffset = herdManager->herdOffset[i];

		const std::string uName8 = "herdAttackTypes[" + std::to_string(i) + "]";
		shader->AddUniformValues(uName8, ShaderValueType::Int, &herd->attackType);

		const std::string uName = "herdOffset[" + std::to_string(i) + "]";
		shader->AddUniformValues(uName, ShaderValueType::Int, &herd->herdOffset);
	}
}

void BillboardRangeAttackTimerCS::PopulateBuffers()
{
	std::vector<float> t;

	for(int i = 0; i < herdManager->totalRenderingAmount; ++i)
	{
		t.push_back(0.f);
	}

	csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(float) * herdManager->totalRenderingAmount,
		GL_DYNAMIC_DRAW, t.data(), ToInt(RangeAttackTimerCS::rangeTimer)));
}
