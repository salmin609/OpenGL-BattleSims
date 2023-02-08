#include "BillboardFrameBufferUsageCS.h"

#include "Buffer.hpp"
#include "BufferManager.h"
#include "Camera.hpp"
#include "Herd.h"
#include "HerdManager.h"
#include "Shader.h"
#include "CSBufferNames.h"

BillboardFrameBufferUsageCS::BillboardFrameBufferUsageCS(Shader* boFBusageComputeShader_, Camera* currentCam_,
	HerdManager* herdManager_): ComputeShaderClass(boFBusageComputeShader_)
{
	currentCam = currentCam_;
	herdManager = herdManager_;
	boFBusageDatas = nullptr;
	BillboardFrameBufferUsageCS::SetShaderUniforms();
	BillboardFrameBufferUsageCS::PopulateBuffers();
}

BillboardFrameBufferUsageCS::~BillboardFrameBufferUsageCS()
{
	delete[] boFBusageDatas;
}

void BillboardFrameBufferUsageCS::CheckFrameBufferUsage() const
{
	shader->Use();
	csBuffers->BindBuffers();
	herdManager->BindHerdPositions();
	shader->SendUniformValues();

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
		nullptr, ToInt(AngleCS::frameBufferUsageIndex)));

	boFBusageDatas = new int[herdManager->totalRenderingAmount];
}

void BillboardFrameBufferUsageCS::SetShaderUniforms()
{
	shader->AddUniformValues("camPos", ShaderValueType::Vec3, &currentCam->Position);
	shader->AddUniformValues("camFront", ShaderValueType::Vec3, &currentCam->Front);
	shader->AddUniformValues("camRight", ShaderValueType::Vec3, &currentCam->Right);
	shader->AddUniformValues("camUp", ShaderValueType::Vec3, &currentCam->Up);
	shader->AddUniformValues("aspect", ShaderValueType::Float, &currentCam->fov);
	shader->AddUniformValues("fovY", ShaderValueType::Float, &currentCam->fovY);
	shader->AddUniformValues("zNear", ShaderValueType::Float, &currentCam->zNear);
	shader->AddUniformValues("zFar", ShaderValueType::Float, &currentCam->zFar);
	shader->AddUniformValues("bufferSize", ShaderValueType::Int, &herdManager->totalRenderingAmount);

	int& herdCount = herdManager->GetHerdCount();

	for (int i = 0; i < herdCount; ++i)
	{
		Herd* herd = herdManager->GetHerd(i);

		herd->herdBoDirAndOffset.w = static_cast<float>(herdManager->herdOffset[i]);

		const std::string uName = "herdBoDirectionAndOffsets[" + std::to_string(i) + "]";
		shader->AddUniformValues(uName, ShaderValueType::Vec4, &herd->herdBoDirAndOffset);
	}

	shader->AddUniformValues("herdCount", ShaderValueType::Int, &herdCount);
}
