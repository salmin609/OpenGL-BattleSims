#include "BillboardObjectManager.h"
#include <iostream>

#include "BillboardAnimationChangeCS.h"
#include "BillboardAttackCS.h"
#include "BillboardFrameBufferUsageCS.h"
#include "BillboardManager.h"
#include "BillboardMovingCS.h"
#include "Buffer.hpp"
#include "Camera.hpp"
#include "HerdManager.h"
#include "Shader.h"

BillboardObjectManager::BillboardObjectManager(Shader* boShader_, BillboardManager* boManager_,
												Shader* boFrameBufferUsageComputeShader,
                                               Camera* currentCam_,
												Shader* boMovingShader_,
												Shader* boAttackShader_,
												Shader* boChangeAnimationShader_)
{
	boShader = boShader_;
	boManager = boManager_;
	currentCam = currentCam_;
	boFBusageComputeShader = boFrameBufferUsageComputeShader;
	herdManager = new HerdManager(boManager, boShader);
	boFBusageCS = new BillboardFrameBufferUsageCS(boFBusageComputeShader, currentCam, herdManager, this);
	boMovingCS = new BillboardMovingCS(boMovingShader_, herdManager, this);
	boAttackCS = new BillboardAttackCS(boAttackShader_, herdManager, this);
	boAnimChangeCS = new BillboardAnimationChangeCS(boChangeAnimationShader_, herdManager, this);
}

BillboardObjectManager::~BillboardObjectManager()
{
	delete herdManager;
	delete boFBusageCS;
	delete boMovingCS;
	delete boAttackCS;
}

void BillboardObjectManager::CheckFrameBufferUsage()
{
	boFBusageCS->CheckFrameBufferUsage();
}

void BillboardObjectManager::Move(float dt)
{
	boMovingCS->Move(dt);
}

void BillboardObjectManager::Attack(float dt)
{
	boAttackCS->AttackComputation(dt);
}

void BillboardObjectManager::ResetAnimationState()
{
	boAnimChangeCS->ResetAnimation();
}

void BillboardObjectManager::Render(const glm::mat4& projMat, const glm::mat4& viewMat)
{
	herdManager->Render(projMat, viewMat);
}
