#include "BillboardObjectManager.h"
#include <iostream>

#include "BillboardAnimationChangeCS.h"
#include "BillboardAttackCS.h"
#include "BillboardCollisionCheckCS.h"
#include "BillboardFrameBufferUsageCS.h"
#include "BillboardManager.h"
#include "BillboardMovingCS.h"
#include "Buffer.hpp"
#include "BufferManager.h"
#include "Camera.hpp"
#include "CSBufferNames.h"
#include "Herd.h"
#include "HerdManager.h"
#include "Shader.h"

BillboardObjectManager::BillboardObjectManager(Shader* boShader_, BillboardManager* boManager_,
												Shader* boFrameBufferUsageComputeShader,
                                               Camera* currentCam_,
												Shader* boMovingShader_,
												Shader* boAttackShader_,
												Shader* boChangeAnimationShader_,
												Shader* lineShader_,
												Shader* bbCollisionCheck_)
{
	boShader = boShader_;
	boManager = boManager_;
	currentCam = currentCam_;
	boFBusageComputeShader = boFrameBufferUsageComputeShader;
	herdManager = new HerdManager(boManager, boShader, lineShader_);
	boFBusageCS = new BillboardFrameBufferUsageCS(boFBusageComputeShader, currentCam, herdManager, this);
	boMovingCS = new BillboardMovingCS(boMovingShader_, herdManager, this);
	boAttackCS = new BillboardAttackCS(boAttackShader_, herdManager, this);
	boAnimChangeCS = new BillboardAnimationChangeCS(boChangeAnimationShader_, herdManager, this);
	boCollisionCheckCS = new BillboardCollisionCheckCS(bbCollisionCheck_, herdManager, this);
}

BillboardObjectManager::~BillboardObjectManager()
{
	delete herdManager;
	delete boFBusageCS;
	delete boMovingCS;
	delete boAttackCS;
	delete boCollisionCheckCS;
}

void BillboardObjectManager::CalculateBOAngle() const
{
	boFBusageCS->CalculateBOAngle();
}

void BillboardObjectManager::Move(float dt) const
{
	boMovingCS->Move(dt);
}

void BillboardObjectManager::Attack(float dt) const
{
	boAttackCS->AttackComputation(dt);
}

void BillboardObjectManager::CollisionCheck(float dt) const
{
	boCollisionCheckCS->CollisionCheck(dt);
}

void BillboardObjectManager::ResetAnimationState() const
{
	boAnimChangeCS->ResetAnimation();
}

void BillboardObjectManager::Render(const glm::mat4& projMat, const glm::mat4& viewMat) const
{
	herdManager->Render(projMat, viewMat);
}

void BillboardObjectManager::ChangeAnimationOfHerds() const
{
	Buffer* animationIndexBuffer = boMovingCS->csBuffers->GetBuffer(ToInt(MoveCS::animationIndices));
	const Buffer* frameIndexBuffer = boFBusageCS->csBuffers->GetBuffer(ToInt(AngleCS::frameBufferUsageIndex));

	//also, check dead objects
	Buffer* boIsDeadBuffer = boAttackCS->csBuffers->GetBuffer(ToInt(AttackCS::isDead));

	frameIndexBuffer->GetData(boFBusageCS->boFBusageDatas);
	animationIndexBuffer->GetData(boMovingCS->animationIndexBuffer);
	boIsDeadBuffer->GetData(boAttackCS->isDead);

	herdManager->ChangeAnimationIndicesOfHerd(boFBusageCS->boFBusageDatas, boMovingCS->animationIndexBuffer,
		boAttackCS->isDead);

	animationIndexBuffer->WriteData(boMovingCS->animationIndexBuffer);
}

void BillboardObjectManager::CheckHerdReachedDestination() const
{
	Buffer* herdReachedBuffer = boMovingCS->csBuffers->GetBuffer(ToInt(MoveCS::herdReachedDestination));
	herdReachedBuffer->GetData(boMovingCS->herdReachedBuffer);

	const int herdCount = herdManager->GetHerdCount();
	bool someReached = false;
	std::vector<int> herdReached;
	for(int i = 0; i < herdCount; ++i)
	{
		int reached = boMovingCS->herdReachedBuffer[i];
		Herd* herd = herdManager->GetHerd(i);

		if(reached == 1)
		{
			herd->herdDirection = glm::vec4(0.f, 0.f, 0.f, 0.f);
			someReached = true;
		}
		herdReached.push_back(0);
	}

	if(someReached)
	{
		herdReachedBuffer->WriteData(herdReached.data());
	}
}

void BillboardObjectManager::ResetAttackingCountBuffer() const
{
	boMovingCS->ResetHerdAttackingCountBuffer();
}

void BillboardObjectManager::ResetCollisionCheckBuffer() const
{
	boCollisionCheckCS->ResetCollisionCheckBuffer();
}
