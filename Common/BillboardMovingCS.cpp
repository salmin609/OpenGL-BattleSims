#include "BillboardMovingCS.h"


#include "Buffer.hpp"
#include "BufferManager.h"
#include "Herd.h"
#include "HerdManager.h"
#include "Shader.h"
#include "glm/vec4.hpp"

BillboardMovingCS::BillboardMovingCS(Shader* boMovingShader_, HerdManager* herdManager_)
{
	boMovingShader = boMovingShader_;
	herdManager = herdManager_;
	csBuffers = new BufferManager();
	tempDt = 0.1f;
	SetBuffers();
	SetUniforms();
}

BillboardMovingCS::~BillboardMovingCS()
{
	delete csBuffers;
}

void BillboardMovingCS::SetUniforms()
{
	boMovingShader->AddUniformValues("dt",ShaderValueType::Float ,&tempDt);

	int& herdCount = herdManager->GetHerdCount();

	for (int i = 0; i < herdCount; ++i)
	{
		Herd* herd = herdManager->GetHerd(i);

		herd->herdBoDirAndOffset.w = static_cast<float>(herdManager->herdOffset[i]);

		const std::string uName = "herdBoDirectionAndOffsets[" + std::to_string(i) + "]";
		boMovingShader->AddUniformValues(uName, ShaderValueType::Vec4, &herd->herdBoDirAndOffset);
	}

	boMovingShader->AddUniformValues("herdCount", ShaderValueType::Int, &herdCount);

}


void BillboardMovingCS::SetBuffers()
{
	csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * herdManager->totalRenderingAmount,
		GL_DYNAMIC_DRAW, nullptr, 3));
}

void BillboardMovingCS::Move()
{
	boMovingShader->Use();
	csBuffers->BindBuffers();
	herdManager->BindHerdPositions();
	boMovingShader->SendUniformValues();

	glDispatchCompute(herdManager->totalRenderingAmount / 128, 1, 1);
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	glUseProgram(0);

	std::vector<glm::vec4> check = csBuffers->GetBuffer(3)->GetDataVector<glm::vec4>();
	int a = 0;
}
