#include "BillboardAttackCS.h"

#include "BillboardAnimationChangeCS.h"
#include "BillboardMovingCS.h"
#include "BillboardObjectManager.h"
#include "Buffer.hpp"
#include "BufferManager.h"
#include "Herd.h"
#include "HerdManager.h"
#include "Shader.h"
#include "CSBufferNames.h"

BillboardAttackCS::BillboardAttackCS(Shader* shader_, HerdManager* herdManager_,
	BillboardObjectManager* boObjManager_):
ComputeShaderClass(shader_), herdManager(herdManager_), boObjManager(boObjManager_)
{
	//timers = new float[herdManager->totalRenderingAmount];
	isDead = new int[herdManager->totalRenderingAmount];

	BillboardAttackCS::PopulateBuffers();
	BillboardAttackCS::SetShaderUniforms();
}

BillboardAttackCS::~BillboardAttackCS()
{
	//delete[] timers;
	delete[] isDead;
}

void BillboardAttackCS::AttackComputation(float dt) const
{
	SendBuffersAndUniforms(dt);
	boObjManager->boAnimChangeCS->csBuffers->
		GetBuffer(ToInt(AnimationChangeCS::animationIndices))->BindStorage(ToInt(AttackCS::animationIndices));

	//boObjManager->csBuffers->GetBuffer(ToInt(TotalBuffer::animationIndices))
	//	->BindStorage(ToInt(AttackCS::animationIndices));

	//boObjManager->csBuffers->GetBuffer(ToInt(TotalBuffer::time))
	//	->BindStorage(ToInt(AttackCS::time));

	//boObjManager->csBuffers->GetBuffer(ToInt(TotalBuffer::isDead))
	//	->BindStorage(ToInt(AttackCS::isDead));

	//boObjManager->csBuffers->GetBuffer(ToInt(TotalBuffer::attackedCount))
	//	->BindStorage(ToInt(AttackCS::attackedCount));

	herdManager->posBuffer->BindStorage(ToInt(AttackCS::objsPoses));

	Dispatch(herdManager->totalRenderingAmount / 64);
}

void BillboardAttackCS::SetShaderUniforms()
{
}

void BillboardAttackCS::PopulateBuffers()
{
	std::vector<float> timers;
	for(int i = 0; i < herdManager->totalRenderingAmount; ++i)
	{
		timers.push_back(0.f);
	}
	csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(float) * herdManager->totalRenderingAmount,
		GL_DYNAMIC_DRAW, timers.data(), ToInt(AttackCS::time)));

	std::vector<int> isDeadVec;

	for(int i = 0; i < herdManager->totalRenderingAmount; ++i)
	{
		isDeadVec.push_back(0);
	}

	csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(int) * herdManager->totalRenderingAmount,
		GL_DYNAMIC_DRAW, isDeadVec.data(), ToInt(AttackCS::isDead)));

	std::vector<int> attackedCount;
	for (int i = 0; i < herdManager->totalRenderingAmount; ++i)
	{
		attackedCount.push_back(0);
	}

	csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(int) * herdManager->totalRenderingAmount,
		GL_DYNAMIC_DRAW, attackedCount.data(), ToInt(AttackCS::attackedCount)));
}
