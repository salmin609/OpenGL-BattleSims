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
	int GetBufferSize(int storageIndex);
	void GetData(int storageIndex, void* dataPtr);
	void WriteData(int bufferIndex, void* dataPtr);
	Buffer* GetBuffer(int storageIndex);

	template <typename T>
	std::vector<T> GetDataVector(int storageIndex);
private:
	std::vector<Buffer*> buffers;
};


