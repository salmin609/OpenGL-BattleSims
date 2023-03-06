#include "BillboardObjectManager.h"
#include <iostream>

#include "BillboardAnimationChangeCS.h"
#include "BillboardAttackCS.h"
#include "BillboardBufferResetCS.h"
#include "BillboardCollisionCheckCS.h"
#include "BillboardFrameBufferUsageCS.h"
#include "BillboardManager.h"
#include "BillboardMovingCS.h"
#include "BillboardRangeAttackTimerCS.h"
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
												Shader* bbCollisionCheck_,
												Shader* bbRangeAttackTimer_,
												Shader* bbResetBuffer_)
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
	boRangeAttackTimerCS = new BillboardRangeAttackTimerCS(bbRangeAttackTimer_, herdManager, this);
	boResetBufferCS = new BillboardBufferResetCS(bbResetBuffer_, herdManager, this);

	PopulateBuffer();
}

BillboardObjectManager::~BillboardObjectManager()
{
	delete herdManager;
	delete boFBusageCS;
	delete boMovingCS;
	delete boAttackCS;
	delete boCollisionCheckCS;
	delete boRangeAttackTimerCS;
	delete csBuffers;
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
	Buffer* animationIndexBuffer = boAnimChangeCS->csBuffers->GetBuffer(ToInt(AnimationChangeCS::animationIndices));
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

void BillboardObjectManager::CheckRangeAttackTimer(float dt) const
{
	boRangeAttackTimerCS->TimeCheck(dt);
}

void BillboardObjectManager::PopulateBuffer()
{
	csBuffers = new BufferManager();
	
	std::vector<int> zeroVecInt;
	std::vector<int> zeroVecFloat;

	std::vector<int> zeroVecInt2;
	std::vector<int> zeroVecFloat2;

	for(int i = 0; i < herdManager->totalRenderingAmount; ++i)
	{
		zeroVecInt.push_back(0);
		zeroVecFloat.push_back(0.f);
	}

	for (int i = 0; i < herdManager->GetHerdCount(); ++i)
	{
		zeroVecInt2.push_back(0);
		zeroVecFloat2.push_back(0.f);
	}

	//csBuffers->AddBuffer(herdManager->posBuffer);
	//csBuffers->AddBuffer(herdManager->directionBuffer);

	//csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(float) * herdManager->totalRenderingAmount,
	//	GL_DYNAMIC_DRAW, zeroVecFloat.data(), ToInt(TotalBuffer::time)));

	//csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(int) * herdManager->totalRenderingAmount,
	//	GL_DYNAMIC_DRAW, zeroVecInt.data(), ToInt(TotalBuffer::isDead)));

	//csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(int) * herdManager->GetHerdCount(),
	//	GL_DYNAMIC_DRAW, zeroVecInt2.data(), ToInt(TotalBuffer::herdReachedDestination)));

	//csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(int) * herdManager->GetHerdCount(),
	//	GL_DYNAMIC_DRAW, zeroVecInt2.data(), ToInt(TotalBuffer::herdAttackingCounts)));

	//csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(int) * herdManager->totalRenderingAmount,
	//	GL_DYNAMIC_DRAW, zeroVecInt.data(), ToInt(TotalBuffer::objsCollisionStatus)));

	//csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(int) * herdManager->totalRenderingAmount,
	//	GL_DYNAMIC_DRAW, zeroVecInt.data(), ToInt(TotalBuffer::animationIndices)));

	//csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(int) * herdManager->totalRenderingAmount,
	//	GL_DYNAMIC_DRAW, zeroVecInt.data(), ToInt(TotalBuffer::attackedCount)));

	//csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(int) * herdManager->totalRenderingAmount, 
	//	GL_DYNAMIC_DRAW, nullptr, ToInt(TotalBuffer::frameBufferUsageIndex)));

	//csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(float) * herdManager->totalRenderingAmount,
	//	GL_DYNAMIC_DRAW, zeroVecFloat.data(), ToInt(TotalBuffer::rangeTimer)));
}

void BillboardObjectManager::RunSimulation(float dt) const
{
	//Decide whether billboard object is in frustum or not.
	//If it, get angle index from that
	CalculateBOAngle();

	//Decide whether ranger unit's attack is ready or not
	//If so, 
	CheckRangeAttackTimer(dt);

	//Check collision status btw objects.
	//Records on objsCollisionStatus[]
	CollisionCheck(dt);

	//Move objs by it's current collision status.
	//If collision_none, move it
	Move(dt);

	//Check if herd reached destination.
	CheckHerdReachedDestination();

	//Change object's animation state by it's collision state.
	//If collision with ally, set to idle.
	//Enemy, set to attack.
	//None, whether idle or run states.
	ResetAnimationState();

	//Check if object is attacking animation, if it, += dt to timer buffer.
	//if timer over some certain number, change to death state.
	Attack(dt);

	//Request to change animation state
	ChangeAnimationOfHerds();

	ResetBuffer();
}

void BillboardObjectManager::ResetBuffer() const
{
	boResetBufferCS->ResetBuffer();
}
