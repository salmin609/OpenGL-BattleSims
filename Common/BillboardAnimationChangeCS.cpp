#include "BillboardAnimationChangeCS.h"

#include "BillboardMovingCS.h"
#include "BillboardObjectManager.h"
#include "Buffer.hpp"
#include "BufferManager.h"
#include "CSBufferNames.h"
#include "HerdManager.h"
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
	shader->Use();
	//csBuffers->GetBuffer(ToInt(AnimationChangeCS::needReset))->
	//	WriteData(boObjManager->boMovingCS->animationStateResetBuffer);

	csBuffers->BindBuffers();
	boObjManager->boMovingCS->csBuffers->
		GetBuffer(ToInt(MoveCS::animationIndices))
			->BindStorage(ToInt(AnimationChangeCS::animationIndices));


	glDispatchCompute(herdManager->totalRenderingAmount / 64, 1, 1);
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	glUseProgram(0);


}

void BillboardAnimationChangeCS::SetShaderUniforms()
{
}

void BillboardAnimationChangeCS::PopulateBuffers()
{
	csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(int) * herdManager->totalRenderingAmount,
		GL_DYNAMIC_DRAW, nullptr, ToInt(AnimationChangeCS::needReset)));
}
