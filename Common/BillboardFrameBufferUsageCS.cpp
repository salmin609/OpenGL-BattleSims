#include "BillboardFrameBufferUsageCS.h"

#include "Buffer.hpp"
#include "BufferManager.h"
#include "Camera.hpp"
#include "Herd.h"
#include "HerdManager.h"
#include "Shader.h"

BillboardFrameBufferUsageCS::BillboardFrameBufferUsageCS(Shader* boFBusageComputeShader_, Camera* currentCam_,
	HerdManager* herdManager_)
{
	boFBusageComputeShader = boFBusageComputeShader_;
	currentCam = currentCam_;
	herdManager = herdManager_;
	boFBusageDatas = nullptr;
	csBuffers = new BufferManager();
	SetShaderUniforms();
	PopulateBuffers();
}

BillboardFrameBufferUsageCS::~BillboardFrameBufferUsageCS()
{
	delete csBuffers;
	delete[] static_cast<int*>(boFBusageDatas);
}

void BillboardFrameBufferUsageCS::CheckFrameBufferUsage()
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

void BillboardFrameBufferUsageCS::PopulateBuffers()
{
	csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER,
		sizeof(int) * herdManager->totalRenderingAmount, GL_DYNAMIC_DRAW,
		nullptr, 0));

	boFBusageDatas = new int[herdManager->totalRenderingAmount];
}

void BillboardFrameBufferUsageCS::SetShaderUniforms()
{
	boFBusageComputeShader->AddUniformValues("camPos", ShaderValueType::Vec3, &currentCam->Position);
	boFBusageComputeShader->AddUniformValues("camFront", ShaderValueType::Vec3, &currentCam->Front);
	boFBusageComputeShader->AddUniformValues("camRight", ShaderValueType::Vec3, &currentCam->Right);
	boFBusageComputeShader->AddUniformValues("camUp", ShaderValueType::Vec3, &currentCam->Up);
	boFBusageComputeShader->AddUniformValues("aspect", ShaderValueType::Float, &currentCam->fov);
	boFBusageComputeShader->AddUniformValues("fovY", ShaderValueType::Float, &currentCam->fovY);
	boFBusageComputeShader->AddUniformValues("zNear", ShaderValueType::Float, &currentCam->zNear);
	boFBusageComputeShader->AddUniformValues("zFar", ShaderValueType::Float, &currentCam->zFar);
	boFBusageComputeShader->AddUniformValues("bufferSize", ShaderValueType::Int, &herdManager->totalRenderingAmount);

	int& herdCount = herdManager->GetHerdCount();

	for (int i = 0; i < herdCount; ++i)
	{
		Herd* herd = herdManager->GetHerd(i);

		herd->herdBoDirAndOffset.w = static_cast<float>(herdManager->herdOffset[i]);

		const std::string uName = "herdBoDirectionAndOffsets[" + std::to_string(i) + "]";
		boFBusageComputeShader->AddUniformValues(uName, ShaderValueType::Vec4, &herd->herdBoDirAndOffset);
	}

	boFBusageComputeShader->AddUniformValues("herdCount", ShaderValueType::Int, &herdCount);
}
