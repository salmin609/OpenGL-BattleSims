#pragma once
#include <vector>

class Buffer;

class ComputeShaderBufferManager
{
public:
	ComputeShaderBufferManager();
	~ComputeShaderBufferManager();
	void AddBuffer(Buffer* buffer);
	void BindBuffers();
	void BindBuffer(int bufferIndex);
	void BindBuffer(int bufferIndex, int storageIndex);
	int GetBufferSize(int index);
	void GetData(int ssboIndex, void* dataPtr);
	void GetData(int ssboIndex);
private:
	std::vector<Buffer*> buffers;
};
