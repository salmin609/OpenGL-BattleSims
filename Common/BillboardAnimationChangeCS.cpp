#include "BillboardAnimationChangeCS.h"

#include "BillboardAttackCS.h"
#include "BillboardCollisionCheckCS.h"
#include "BillboardMovingCS.h"
#include "BillboardObjectManager.h"
#include "Buffer.hpp"
#include "BufferManager.h"
#include "CSBufferNames.h"
#include "Herd.h"
#include "HerdManager.h"
#include "ModelKinds.h"
#include "Shader.h"

BillboardAnimationChangeCS::BillboardAnimationChangeCS(Shader* shader_, HerdManager* herdManager_,
                                                       BillboardObjectManager* boObjManager_): ComputeShaderClass(shader_)
{
	herdManager = herdManager_;
	boObjManager = boObjManager_;

	BillboardAnimationChangeCS::PopulateBuffers();
	BillboardAnimationChangeCS::SetShaderUniforms();
}

BillboardAnimationChangeCS::~BillboardAnimationChangeCS()
{

}

void BillboardAnimationChangeCS::ResetAnimation()
{
	SendBuffersAndUniforms(0.f);

	csBuffers->BindBuffers();
	//boObjManager->boMovingCS->csBuffers->
	//	GetBuffer(ToInt(MoveCS::animationIndices))
	//		->BindStorage(ToInt(AnimationChangeCS::animationIndices));

	//boObjManager->csBuffers->GetBuffer(ToInt(TotalBuffer::isDead))
	//	->BindStorage(ToInt(AnimationChangeCS::isDead));

	//boObjManager->csBuffers->GetBuffer(ToInt(TotalBuffer::objsCollisionStatus))
	//	->BindStorage(ToInt(AnimationChangeCS::objsCollisionStatus));

	//boObjManager->csBuffers->GetBuffer(ToInt(TotalBuffer::herdAttackingCounts))
	//	->BindStorage(ToInt(AnimationChangeCS::herdAttackingCounts));

	//boObjManager->csBuffers->GetBuffer(ToInt(TotalBuffer::animationIndices))
	//	->BindStorage(ToInt(AnimationChangeCS::animationIndices));

	boObjManager->boAttackCS->csBuffers->GetBuffer(ToInt(AttackCS::isDead))
		->BindStorage(ToInt(AnimationChangeCS::isDead));

	boObjManager->boCollisionCheckCS->csBuffers->GetBuffer(ToInt(CollisionCheckCS::objsCollisionStatus))
		->BindStorage(ToInt(AnimationChangeCS::objsCollisionStatus));

	boObjManager->boMovingCS->csBuffers->GetBuffer(ToInt(MoveCS::herdAttackingCounts))
		->BindStorage(ToInt(AnimationChangeCS::herdAttackingCounts));

	//boObjManager->boMovingCS->csBuffers->GetBuffer(ToInt(MoveCS::herdMovable))
	//	->BindStorage(ToInt(AnimationChangeCS::herdMovable));

	Dispatch(herdManager->totalRenderingAmount / 64);
}

void BillboardAnimationChangeCS::SetShaderUniforms()
{
	int& herdCount = herdManager->GetHerdCount();
	shader->AddUniformValues("herdCount", ShaderValueType::Int, &herdCount);

	for (int i = 0; i < herdCount; ++i)
	{
		Herd* herd = herdManager->GetHerd(i);
		herd->herdOffset = herdManager->herdOffset[i];

		const std::string uName8 = "herdAttackTypes[" + std::to_string(i) + "]";
		shader->AddUniformValues(uName8, ShaderValueType::Int, &herd->attackType);

		const std::string uName = "herdOffset[" + std::to_string(i) + "]";
		shader->AddUniformValues(uName, ShaderValueType::Int, &herd->herdOffset);

		const std::string uName2 = "herdCounts[" + std::to_string(i) + "]";
		shader->AddUniformValues(uName2, ShaderValueType::Int, &herd->count);

		const std::string uName4 = "herdDirections[" + std::to_string(i) + "]";
		shader->AddUniformValues(uName4, ShaderValueType::Vec4, &herd->herdDirection);
	}
}

void BillboardAnimationChangeCS::PopulateBuffers()
{
	std::vector<int> ck;
	for (int i = 0; i < herdManager->totalRenderingAmount; ++i)
	{
		ck.push_back(static_cast<int>(State::Idle));
	}

	csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(int) * herdManager->totalRenderingAmount,
		GL_DYNAMIC_DRAW, ck.data(), ToInt(AnimationChangeCS::animationIndices)));


}
