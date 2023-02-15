#include "BillboardMovingCS.h"

#include "BillboardAttackCS.h"
#include "BillboardObjectManager.h"
#include "Buffer.hpp"
#include "BufferManager.h"
#include "Herd.h"
#include "HerdManager.h"
#include "Shader.h"
#include "glm/vec4.hpp"
#include "CSBufferNames.h"
#include "ModelKinds.hpp"

BillboardMovingCS::BillboardMovingCS(Shader* boMovingShader_, HerdManager* herdManager_,
                                     BillboardObjectManager* boObjManager_): ComputeShaderClass(boMovingShader_), boObjManager(boObjManager_)
{
	herdManager = herdManager_;
	BillboardMovingCS::PopulateBuffers();
	BillboardMovingCS::SetShaderUniforms();
	animationIndexBuffer = new int[herdManager->totalRenderingAmount];
}

BillboardMovingCS::~BillboardMovingCS()
{
	delete[] animationIndexBuffer;
}

void BillboardMovingCS::SetShaderUniforms()
{
	int& herdCount = herdManager->GetHerdCount();

	for (int i = 0; i < herdCount; ++i)
	{
		Herd* herd = herdManager->GetHerd(i);
		herd->herdOffset = herdManager->herdOffset[i];

		const std::string uName = "herdOffset[" + std::to_string(i) + "]";
		shader->AddUniformValues(uName, ShaderValueType::Int, &herd->herdOffset);

		const std::string uName2 = "herdCounts[" + std::to_string(i) + "]";
		shader->AddUniformValues(uName2, ShaderValueType::Int, &herd->count);
	}

	shader->AddUniformValues("herdCount", ShaderValueType::Int, &herdCount);
}


void BillboardMovingCS::PopulateBuffers()
{
	std::vector<int> ck;
	for(int i = 0; i < herdManager->totalRenderingAmount; ++i)
	{
		ck.push_back(static_cast<int>(State::Idle));
	}

	csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(int) * herdManager->totalRenderingAmount,
		GL_DYNAMIC_DRAW, ck.data(), ToInt(MoveCS::AnimationIndex)));




	const float LO = 12.f;
	const float HI = 25.f;

	std::vector<float> randSpeed;
	for(int i = 0; i < herdManager->totalRenderingAmount; ++i)
	{
		float r3 = LO + static_cast <float> (rand()) / (RAND_MAX / (HI - LO));
		randSpeed.push_back(r3);
	}
	csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(float) * herdManager->totalRenderingAmount,
		GL_DYNAMIC_DRAW, randSpeed.data(), ToInt(MoveCS::randSpeed)));

	csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * herdManager->totalRenderingAmount,
		GL_DYNAMIC_DRAW, nullptr, ToInt(MoveCS::targetEnemyPos)));

	std::vector<int> attackedCount;
	for(int i = 0; i < herdManager->totalRenderingAmount; ++i)
	{
		attackedCount.push_back(0);
	}

	csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(int) * herdManager->totalRenderingAmount,
		GL_DYNAMIC_DRAW, attackedCount.data(), ToInt(MoveCS::attackedCount)));
}

void BillboardMovingCS::Move(float dt) const
{
	shader->Use();
	csBuffers->BindBuffers();
	boObjManager->boAttackCS->csBuffers->GetBuffer(ToInt(AttackCS::time))->
		BindStorage(ToInt(MoveCS::time));
	boObjManager->boAttackCS->csBuffers->GetBuffer(ToInt(AttackCS::isDead))->
		BindStorage(ToInt(MoveCS::isDead));
	//herdManager->BindHerdPositions();
	herdManager->posBuffer->BindStorage(ToInt(MoveCS::objsPoses));
	herdManager->directionBuffer->BindStorage(ToInt(MoveCS::objsDirections));

	shader->SendUniformValues();
	shader->SendUniformFloat("dt", dt);

	glDispatchCompute(herdManager->totalRenderingAmount / 64, 1, 1);
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	glUseProgram(0);
}
