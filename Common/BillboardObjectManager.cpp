#include "BillboardObjectManager.h"
#include <iostream>
#include "BillboardFrameBufferUsageCS.h"
#include "BillboardManager.h"
#include "Buffer.hpp"
#include "Camera.hpp"
#include "HerdManager.h"
#include "Shader.h"

BillboardObjectManager::BillboardObjectManager(Shader* boShader_, BillboardManager* boManager_,
												Shader* boFrameBufferUsageComputeShader,
                                               Camera* currentCam_)
{
	boShader = boShader_;
	boManager = boManager_;
	currentCam = currentCam_;
	boFBusageComputeShader = boFrameBufferUsageComputeShader;
	herdManager = new HerdManager(boManager, boShader);
	boFBusageCS = new BillboardFrameBufferUsageCS(boFBusageComputeShader, currentCam, herdManager);
}

BillboardObjectManager::~BillboardObjectManager()
{
	delete herdManager;
	delete boFBusageCS;
}

void BillboardObjectManager::CheckFrameBufferUsage()
{
	boFBusageCS->CheckFrameBufferUsage();
}

void BillboardObjectManager::Render(const glm::mat4& projMat, const glm::mat4& viewMat)
{
	herdManager->Render(projMat, viewMat);
}
