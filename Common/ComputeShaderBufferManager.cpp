#include "ComputeShaderBufferManager.h"

#include "Buffer.hpp"

ComputeShaderBufferManager::ComputeShaderBufferManager()
{

}

ComputeShaderBufferManager::~ComputeShaderBufferManager()
{
	for (const auto& buffer : buffers)
		delete buffer;
}

void ComputeShaderBufferManager::AddBuffer(Buffer* buffer)
{
	buffers.push_back(buffer);
}

void ComputeShaderBufferManager::BindBuffers()
{
	for(const auto& buffer : buffers)
		buffer->BindStorage();
}

void ComputeShaderBufferManager::BindBuffer(int bufferIndex)
{
	buffers[bufferIndex]->BindStorage();
}

void ComputeShaderBufferManager::BindBuffer(int bufferIndex, int storageIndex)
{
	buffers[bufferIndex]->BindStorage(storageIndex);
}

int ComputeShaderBufferManager::GetBufferSize(int index)
{
	return buffers[index]->GetSize();
}

void ComputeShaderBufferManager::GetData(int ssboIndex, void* dataPtr)
{
	buffers[ssboIndex]->GetData(dataPtr);
}