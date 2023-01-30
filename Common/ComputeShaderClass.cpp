#include "ComputeShaderClass.h"

#include "BufferManager.h"

ComputeShaderClass::ComputeShaderClass(Shader* shader_) : shader(shader_)
{
	csBuffers = new BufferManager();
}

ComputeShaderClass::~ComputeShaderClass()
{
	delete csBuffers;
}
