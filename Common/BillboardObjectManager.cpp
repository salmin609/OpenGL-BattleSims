#include "BillboardObjectManager.h"
#include <iostream>
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
												Shader* boMovingShader_)
{
	boShader = boShader_;
	boManager = boManager_;
	currentCam = currentCam_;
	boFBusageComputeShader = boFrameBufferUsageComputeShader;
	herdManager = new HerdManager(boManager, boShader);
	boFBusageCS = new BillboardFrameBufferUsageCS(boFBusageComputeShader, currentCam, herdManager);
	boMovingCS = new BillboardMovingCS(boMovingShader_, herdManager);
}

BillboardObjectManager::~BillboardObjectManager()
{
	delete herdManager;
	delete boFBusageCS;
	delete boMovingCS;
}

void BillboardObjectManager::CheckFrameBufferUsage()
{
	boFBusageCS->CheckFrameBufferUsage();
}

void BillboardObjectManager::Move()
{
	boMovingCS->Move();
}

void BillboardObjectManager::MoveBos()
{
}

void BillboardObjectManager::Render(const glm::mat4& projMat, const glm::mat4& viewMat)
{
	herdManager->Render(projMat, viewMat);
}
