#include "BillboardMovingCS.h"
#include "Buffer.hpp"
#include "BufferManager.h"
#include "Herd.h"
#include "HerdManager.h"
#include "Shader.h"
#include "glm/vec4.hpp"

BillboardMovingCS::BillboardMovingCS(Shader* boMovingShader_, HerdManager* herdManager_): ComputeShaderClass(boMovingShader_)
{
	herdManager = herdManager_;
	BillboardMovingCS::PopulateBuffers();
	BillboardMovingCS::SetShaderUniforms();
	reached = new int[herdManager->totalRenderingAmount];
}

BillboardMovingCS::~BillboardMovingCS()
{
	delete[] reached;
}

void BillboardMovingCS::SetShaderUniforms()
{
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


void BillboardMovingCS::PopulateBuffers()
{
	csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(int) * herdManager->totalRenderingAmount,
		GL_DYNAMIC_DRAW, nullptr, 0));

	//csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * herdManager->GetHerd(0)->count,
	//	GL_DYNAMIC_DRAW, nullptr, 3));

	//csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * herdManager->GetHerd(1)->count,
	//	GL_DYNAMIC_DRAW, nullptr, 4));
}

void BillboardMovingCS::Move(float dt) const
{
	shader->Use();
	csBuffers->BindBuffers();
	herdManager->BindHerdPositions();
	shader->SendUniformValues();

	shader->SendUniformFloat("dt", dt);

	glDispatchCompute(herdManager->totalRenderingAmount / 64, 1, 1);
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	glUseProgram(0);

	csBuffers->GetData(0, reached);
	assert(reached != nullptr);

	herdManager->SetReachedAnimation(reached);
}
