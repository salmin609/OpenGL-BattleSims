#include "BufferManager.h"

#include "Buffer.hpp"

BufferManager::BufferManager()
{

}

BufferManager::~BufferManager()
{
	for (const auto& buffer : buffers)
		delete buffer;
}

void BufferManager::AddBuffer(Buffer* buffer)
{
	buffers.push_back(buffer);
}

void BufferManager::BindBuffers()
{
	for(const auto& buffer : buffers)
		buffer->BindStorage();
}

void BufferManager::BindBuffer(int bufferIndex)
{
	buffers[bufferIndex]->Bind();
}

void BufferManager::BindBuffer(int bufferIndex, int storageIndex)
{
	buffers[bufferIndex]->BindStorage(storageIndex);
}

int BufferManager::GetBufferSize(int index)
{
	return buffers[index]->GetSize();
}

void BufferManager::GetData(int bufferIndex, void* dataPtr)
{
	buffers[bufferIndex]->GetData(dataPtr);
}


void BufferManager::WriteData(int bufferIndex, void* dataPtr)
{
	buffers[bufferIndex]->WriteData(dataPtr);
}
