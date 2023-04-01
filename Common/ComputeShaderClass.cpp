/*
 * Author		: Ryan Kim.
 * Date			: 2023-03-17
 * Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#include "ComputeShaderClass.h"

#include "BufferManager.h"
#include "Shader.h"

ComputeShaderClass::ComputeShaderClass(Shader* shader_) : shader(shader_)
{
	csBuffers = new BufferManager();
}

ComputeShaderClass::~ComputeShaderClass()
{
	delete csBuffers;
}

void ComputeShaderClass::SendBuffersAndUniforms(float dt) const
{
	shader->Use();
	csBuffers->BindBuffers();
	shader->SendUniformFloat("dt", dt);
	shader->SendUniformValues();
}

void ComputeShaderClass::Dispatch(int xGroupNum) const
{
	glDispatchCompute(xGroupNum, 1, 1);
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	glUseProgram(0);
}
