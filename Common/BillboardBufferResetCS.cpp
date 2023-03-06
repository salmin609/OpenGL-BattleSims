#include "BillboardBufferResetCS.h"

#include "BillboardCollisionCheckCS.h"
#include "BillboardMovingCS.h"
#include "BillboardObjectManager.h"
#include "Buffer.hpp"
#include "BufferManager.h"
#include "CSBufferNames.h"
#include "HerdManager.h"

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
}

void BillboardBufferResetCS::PopulateBuffers()
{
}
