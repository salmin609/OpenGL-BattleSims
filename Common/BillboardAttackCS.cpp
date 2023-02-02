#include "BillboardAttackCS.h"

#include "BillboardMovingCS.h"
#include "BillboardObjectManager.h"
#include "Buffer.hpp"
#include "BufferManager.h"
#include "HerdManager.h"
#include "Shader.h"

BillboardAttackCS::BillboardAttackCS(Shader* shader_, HerdManager* herdManager_,
	BillboardObjectManager* boObjManager_):
ComputeShaderClass(shader_), herdManager(herdManager_), boObjManager(boObjManager_)
{
	timers = new float[herdManager->totalRenderingAmount];
	
	BillboardAttackCS::PopulateBuffers();
	BillboardAttackCS::SetShaderUniforms();
}

BillboardAttackCS::~BillboardAttackCS()
{
	delete[] timers;
}

void BillboardAttackCS::AttackComputation(float dt)
{
	shader->Use();
	csBuffers->BindBuffers();
	shader->SendUniformFloat("dt", dt);
	boObjManager->boMovingCS->csBuffers->GetBuffer(0)->BindStorage(1);

	glDispatchCompute(herdManager->totalRenderingAmount / 64, 1, 1);
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	glUseProgram(0);

	//csBuffers->GetData(0, timers);
	assert(timers != nullptr);
}

void BillboardAttackCS::SetShaderUniforms()
{
}

void BillboardAttackCS::PopulateBuffers()
{
	for(int i = 0; i < herdManager->totalRenderingAmount; ++i)
	{
		timers[i] = 0.f;
	}
	csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(float) * herdManager->totalRenderingAmount,
		GL_DYNAMIC_DRAW, timers, 0));
}
