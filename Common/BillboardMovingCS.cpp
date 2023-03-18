#include "BillboardMovingCS.h"

#include "BillboardAttackCS.h"
#include "BillboardCollisionCheckCS.h"
#include "BillboardObjectManager.h"
#include "Buffer.hpp"
#include "BufferManager.h"
#include "Herd.h"
#include "HerdManager.h"
#include "Shader.h"
#include "glm/vec4.hpp"
#include "CSBufferNames.h"
#include "ModelKinds.h"

BillboardMovingCS::BillboardMovingCS(Shader* boMovingShader_, HerdManager* herdManager_,
                                     BillboardObjectManager* boObjManager_): ComputeShaderClass(boMovingShader_), boObjManager(boObjManager_)
{
	herdManager = herdManager_;
	BillboardMovingCS::PopulateBuffers();
	BillboardMovingCS::SetShaderUniforms();
	animationIndexBuffer = new int[herdManager->totalRenderingAmount];
	herdReachedBuffer = new int[herdManager->GetHerdCount()];
}

BillboardMovingCS::~BillboardMovingCS()
{
	delete[] animationIndexBuffer;
	delete[] herdReachedBuffer;
}

void BillboardMovingCS::SetShaderUniforms()
{
	int& herdCount = herdManager->GetHerdCount();
	shader->AddUniformValues("herdCount", ShaderValueType::Int, &herdCount);

	for (int i = 0; i < herdCount; ++i)
	{
		Herd* herd = herdManager->GetHerd(i);
		herd->herdOffset = herdManager->herdOffset[i];

		const std::string uName = "herdOffset[" + std::to_string(i) + "]";
		shader->AddUniformValues(uName, ShaderValueType::Int, &herd->herdOffset);

		const std::string uName2 = "herdCounts[" + std::to_string(i) + "]";
		shader->AddUniformValues(uName2, ShaderValueType::Int, &herd->count);

		const std::string uName3 = "herdSides[" + std::to_string(i) + "]";
		shader->AddUniformValues(uName3, ShaderValueType::Int, &herd->side);

		const std::string uName4 = "herdDirections[" + std::to_string(i) + "]";
		shader->AddUniformValues(uName4, ShaderValueType::Vec4, &herd->herdDirection);

		const std::string uName5 = "herdSpeeds[" + std::to_string(i) + "]";
		shader->AddUniformValues(uName5, ShaderValueType::Float, &herd->speed);

		const std::string uName6 = "herdDestinations[" + std::to_string(i) + "]";
		shader->AddUniformValues(uName6, ShaderValueType::Vec4, &herd->herdDestination);

		const std::string uName7 = "herdAttackRanges[" + std::to_string(i) + "]";
		shader->AddUniformValues(uName7, ShaderValueType::Float, &herd->attackRange);

		const std::string uName8 = "herdAttackTypes[" + std::to_string(i) + "]";
		shader->AddUniformValues(uName8, ShaderValueType::Int, &herd->attackType);

		const std::string uName9 = "herdWidths[" + std::to_string(i) + "]";
		shader->AddUniformValues(uName9, ShaderValueType::Int, &herd->herdWidth);
	}
}


void BillboardMovingCS::PopulateBuffers()
{
	const int herdCount = herdManager->GetHerdCount();

	std::vector<int> herdReachedDest;

	for(int i = 0; i < herdCount; ++i)
	{
		herdReachedDest.push_back(0);
	}

	csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(int) * herdCount,
		GL_DYNAMIC_DRAW, herdReachedDest.data(), ToInt(MoveCS::herdReachedDestination)));

	csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(int) * herdCount,
		GL_DYNAMIC_DRAW, herdReachedDest.data(), ToInt(MoveCS::herdAttackingCounts)));

	//std::vector<int> ck2;
	//for (int i = 0; i < herdManager->GetHerdCount(); ++i)
	//{
	//	ck2.push_back(1);
	//}

	//csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(int) * herdManager->GetHerdCount(),
	//	GL_DYNAMIC_DRAW, ck2.data(), ToInt(MoveCS::herdMovable)));
}

void BillboardMovingCS::Move(float dt) const
{
	SendBuffersAndUniforms(dt);

	//boObjManager->csBuffers->GetBuffer(ToInt(TotalBuffer::time))
	//	->BindStorage(ToInt(MoveCS::time));

	//boObjManager->csBuffers->GetBuffer(ToInt(TotalBuffer::isDead))
	//	->BindStorage(ToInt(MoveCS::isDead));

	//boObjManager->csBuffers->GetBuffer(ToInt(TotalBuffer::objsCollisionStatus))
	//	->BindStorage(ToInt(MoveCS::objsCollisionStatus));

	//boObjManager->csBuffers->GetBuffer(ToInt(TotalBuffer::herdReachedDestination))
	//	->BindStorage(ToInt(MoveCS::herdReachedDestination));

	//boObjManager->csBuffers->GetBuffer(ToInt(TotalBuffer::herdAttackingCounts))
	//	->BindStorage(ToInt(MoveCS::herdAttackingCounts));

	boObjManager->boAttackCS->csBuffers->GetBuffer(ToInt(AttackCS::isDead))->
		BindStorage(ToInt(MoveCS::isDead));
	boObjManager->boCollisionCheckCS->csBuffers->GetBuffer(ToInt(CollisionCheckCS::objsCollisionStatus))->
		BindStorage(ToInt(MoveCS::objsCollisionStatus));
	//boObjManager->boAttackCS->csBuffers->GetBuffer(ToInt(AttackCS::herdNumCount))->
	//	BindStorage(ToInt(MoveCS::herdNumCount));

	herdManager->posBuffer->BindStorage(ToInt(MoveCS::objsPoses));
	herdManager->directionBuffer->BindStorage(ToInt(MoveCS::objsDirections));

	Dispatch(herdManager->totalRenderingAmount / 64);
}

