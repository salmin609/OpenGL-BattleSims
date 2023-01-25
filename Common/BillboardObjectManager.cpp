#include "BillboardObjectManager.h"
#include <iostream>
#include "BillboardAnimatingDatas.h"
#include "BillboardManager.h"
#include "Buffer.hpp"
#include "Camera.hpp"
#include "BufferManager.h"
#include "Herd.h"
#include "HerdManager.h"
#include "ModelKinds.hpp"
#include "MultipleAnimationObject.h"
#include "Shader.h"

BillboardObjectManager::BillboardObjectManager(Shader* boShader_, BillboardManager* boManager_,
												Shader* boFrameBufferUsageComputeShader,
                                               Camera* currentCam_)
{
	boShader = boShader_;
	boManager = boManager_;
	currentCam = currentCam_;
	boFBusageComputeShader = boFrameBufferUsageComputeShader;

	csBuffers = new BufferManager();
	herdManager = new HerdManager(boManager, boShader);

	Populate();
	SetShaderUniforms();
	PopulateBuffers();
}

BillboardObjectManager::~BillboardObjectManager()
{
	delete[] static_cast<int*>(boFBusageDatas);
	delete herdManager;
	delete csBuffers;
}

void BillboardObjectManager::Populate()
{
	herdManager->AddHerd(herdManager->PopulateHerd(1280, static_cast<int>(ObjKind::SWAT), glm::vec3(0.f, 12.f, -20.f), 20.f,
		glm::vec3(-1.f, 0.f, 0.f)));

	herdManager->AddHerd(herdManager->PopulateHerd(1280, static_cast<int>(ObjKind::AMY), glm::vec3(-400.f, 12.f, -20.f), 20.f,
		glm::vec3(1.f, 0.f, 0.f)));
	
	//herds.push_back(PopulateObjs(1280, static_cast<int>(ObjKind::KNIGHT), glm::vec3(0.f, 12.f, -400.f), 20.f));
	//herds.push_back(PopulateObjs(1280, static_cast<int>(ObjKind::MICHELLE), glm::vec3(0.f, 12.f, 400.f), 20.f));
}

void BillboardObjectManager::CheckFrameBufferUsage()
{
	boFBusageComputeShader->Use();
	csBuffers->BindBuffers();
	herdManager->BindHerdPositions();
	boFBusageComputeShader->SendUniformValues();

	glDispatchCompute(herdManager->totalRenderingAmount / 128, 1, 1);
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	glUseProgram(0);

	csBuffers->GetData(0, boFBusageDatas);
	assert(boFBusageDatas != nullptr);
	herdManager->SetBosFrameBufferIndex(boFBusageDatas);
}

void BillboardObjectManager::Render(const glm::mat4& projMat, const glm::mat4& viewMat)
{
	herdManager->Render(projMat, viewMat);
}

void BillboardObjectManager::SetShaderUniforms()
{
	boFBusageComputeShader->AddUniformValues("camPos", ShaderValueType::Vec3, &currentCam->Position);
	boFBusageComputeShader->AddUniformValues("camFront", ShaderValueType::Vec3, &currentCam->Front);
	boFBusageComputeShader->AddUniformValues("camRight", ShaderValueType::Vec3, &currentCam->Right);
	boFBusageComputeShader->AddUniformValues("camUp", ShaderValueType::Vec3, &currentCam->Up);
	boFBusageComputeShader->AddUniformValues("aspect", ShaderValueType::Float, &currentCam->fov);
	boFBusageComputeShader->AddUniformValues("fovY", ShaderValueType::Float, &currentCam->fovY);
	boFBusageComputeShader->AddUniformValues("zNear", ShaderValueType::Float, &zNear);
	boFBusageComputeShader->AddUniformValues("zFar", ShaderValueType::Float, &zFar);
	boFBusageComputeShader->AddUniformValues("bufferSize", ShaderValueType::Int, &herdManager->totalRenderingAmount);

	int& herdCount = herdManager->GetHerdCount();

	for (int i = 0; i < herdCount; ++i)
	{
		Herd* herd = herdManager->GetHerd(i);

		herd->herdBoDirAndOffset.w = static_cast<float>(herdManager->herdOffset[i]);

		const std::string uName = "herdBoDirectionAndOffsets[" + std::to_string(i) + "]";
		boFBusageComputeShader->AddUniformValues(uName, ShaderValueType::Vec4, &herd->GetHerdBoDirAndOffset());
	}

	boFBusageComputeShader->AddUniformValues("herdCount", ShaderValueType::Int, &herdCount);
}

void BillboardObjectManager::PopulateBuffers()
{
	csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER, 
		sizeof(int) * herdManager->totalRenderingAmount, GL_DYNAMIC_DRAW,
		nullptr, 0));

	boFBusageDatas = new int[herdManager->totalRenderingAmount];
}