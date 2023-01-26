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

int BufferManager::GetBufferSize(int storageIndex)
{
	for(const auto& buffer : buffers)
	{
		if (buffer->GetStorageIndex() == storageIndex)
			return buffer->GetSize();
	}
	//StorageIndex does not exist.
	return -1;
}

void BufferManager::GetData(int storageIndex, void* dataPtr)
{
	for (const auto& buffer : buffers)
	{
		if (buffer->GetStorageIndex() == storageIndex)
		{
			buffer->GetData(dataPtr);
			return;
		}
	}

	dataPtr = nullptr;
}


void BufferManager::WriteData(int bufferIndex, void* dataPtr)
{
	buffers[bufferIndex]->WriteData(dataPtr);
}

Buffer* BufferManager::GetBuffer(int storageIndex)
{
	for (const auto& buffer : buffers)
	{
		if (buffer->GetStorageIndex() == storageIndex)
		{
			return buffer;
		}
	}
	return nullptr;
}
