#pragma once
#include <vector>

class Buffer;

class BufferManager
{
public:
	BufferManager();
	~BufferManager();
	void AddBuffer(Buffer* buffer);
	void BindBuffers();
	void BindBuffer(int bufferIndex);
	void BindBuffer(int bufferIndex, int storageIndex);
	int GetBufferSize(int index);
	void GetData(int bufferIndex, void* dataPtr);
	void WriteData(int bufferIndex, void* dataPtr);
	//void GetData(int ssboIndex);
private:
	std::vector<Buffer*> buffers;
};
