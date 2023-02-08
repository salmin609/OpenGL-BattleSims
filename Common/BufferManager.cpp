#include "BufferManager.h"

#include <cassert>

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
	Buffer* buffer = GetBuffer(storageIndex);

	if (buffer != nullptr)
		return buffer->GetSize();

	/*for(const auto& buffer : buffers)
	{
		if (buffer->GetStorageIndex() == storageIndex)
			return buffer->GetSize();
	}*/
	//StorageIndex does not exist.
	assert(0);
	return -1;
}

void BufferManager::GetData(int storageIndex, void* dataPtr)
{
	Buffer* buffer = GetBuffer(storageIndex);

	if(buffer != nullptr)
	{
		buffer->GetData(dataPtr);
		return;
	}

	/*for (const auto& buffer : buffers)
	{
		if (buffer->GetStorageIndex() == storageIndex)
		{
			buffer->GetData(dataPtr);
			return;
		}
	}*/

	//Storage Index does not exist, somethings wrong.
	assert(0);
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

template <typename T>
std::vector<T> BufferManager::GetDataVector(int storageIndex)
{
	//for (const auto& buffer : buffers)
	//{
	//	if (buffer->GetStorageIndex() == storageIndex)
	//	{
	//		return buffer->GetDataVector<T>();
	//	}
	//}

	Buffer* buffer = GetBuffer(storageIndex);

	if (buffer != nullptr)
		return buffer->GetDataVector<T>();
	
	//assert(0);
	
	return std::vector<T>{};
}
