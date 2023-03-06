#include "BillboardCollisionCheckCS.h"

#include "BillboardAttackCS.h"
#include "BillboardMovingCS.h"
#include "BufferManager.h"
#include "HerdManager.h"
#include "Shader.h"
#include "BillboardObjectManager.h"
#include "Buffer.hpp"
#include "CSBufferNames.h"
#include "Herd.h"

BillboardCollisionCheckCS::BillboardCollisionCheckCS(Shader* shader_, HerdManager* herdManager_,
                                                     BillboardObjectManager* boObjManager_) :
ComputeShaderClass(shader_), herdManager(herdManager_), boObjManager(boObjManager_)
{
	BillboardCollisionCheckCS::PopulateBuffers();
	BillboardCollisionCheckCS::SetShaderUniforms();
}

BillboardCollisionCheckCS::~BillboardCollisionCheckCS()
{
	
}

void BillboardCollisionCheckCS::CollisionCheck(float dt) const
{
	SendBuffersAndUniforms(dt);

	boObjManager->boAttackCS->csBuffers->GetBuffer(ToInt(AttackCS::attackedCount))
		->BindStorage(ToInt(CollisionCheckCS::attackedCount));

	boObjManager->boMovingCS->csBuffers->GetBuffer(ToInt(MoveCS::herdAttackingCounts))
		->BindStorage(ToInt(CollisionCheckCS::herdAttackingCounts));

	boObjManager->boAttackCS->csBuffers->GetBuffer(ToInt(AttackCS::isDead))
		->BindStorage(ToInt(CollisionCheckCS::isDead));

	//boObjManager->csBuffers->GetBuffer(ToInt(TotalBuffer::objsCollisionStatus))
	//	->BindStorage(ToInt(CollisionCheckCS::objsCollisionStatus));

	//boObjManager->csBuffers->GetBuffer(ToInt(TotalBuffer::attackedCount))
	//	->BindStorage(ToInt(CollisionCheckCS::attackedCount));

	//boObjManager->csBuffers->GetBuffer(ToInt(TotalBuffer::herdAttackingCounts))
	//	->BindStorage(ToInt(CollisionCheckCS::herdAttackingCounts));

	//boObjManager->csBuffers->GetBuffer(ToInt(TotalBuffer::isDead))
	//	->BindStorage(ToInt(CollisionCheckCS::isDead));

	herdManager->posBuffer->BindStorage(ToInt(CollisionCheckCS::objsPoses));
	herdManager->directionBuffer->BindStorage(ToInt(CollisionCheckCS::objsDirections));

	Dispatch(herdManager->totalRenderingAmount / 64);
}


void BillboardCollisionCheckCS::SetShaderUniforms()
{
	int& herdCount = herdManager->GetHerdCount();
	shader->AddUniformValues("herdCount", ShaderValueType::Int, &herdCount);

	for (int i = 0; i < herdCount; ++i)
	{
		Herd* herd = herdManager->GetHerd(i);
		herd->herdOffset = herdManager->herdOffset[i];

		const std::string uName4 = "herdDirections[" + std::to_string(i) + "]";
		shader->AddUniformValues(uName4, ShaderValueType::Vec4, &herd->herdDirection);

		const std::string uName5 = "herdSpeeds[" + std::to_string(i) + "]";
		shader->AddUniformValues(uName5, ShaderValueType::Float, &herd->speed);

		const std::string uName3 = "herdSides[" + std::to_string(i) + "]";
		shader->AddUniformValues(uName3, ShaderValueType::Int, &herd->side);

		const std::string uName2 = "herdCounts[" + std::to_string(i) + "]";
		shader->AddUniformValues(uName2, ShaderValueType::Int, &herd->count);

		const std::string uName7 = "herdAttackRanges[" + std::to_string(i) + "]";
		shader->AddUniformValues(uName7, ShaderValueType::Float, &herd->attackRange);

		const std::string uName = "herdOffset[" + std::to_string(i) + "]";
		shader->AddUniformValues(uName, ShaderValueType::Int, &herd->herdOffset);
	}
}

void BillboardCollisionCheckCS::PopulateBuffers()
{
	std::vector<int> collisionStatusVec;
	for (int i = 0; i < herdManager->totalRenderingAmount; ++i)
	{
		collisionStatusVec.push_back(0);
	}
	csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(int) * herdManager->totalRenderingAmount,
		GL_DYNAMIC_DRAW, collisionStatusVec.data(), ToInt(CollisionCheckCS::objsCollisionStatus)));
}
