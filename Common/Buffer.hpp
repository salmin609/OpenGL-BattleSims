/*
 * Author		: Ryan Kim.
 * Date			: 2022-10-07
 * Description	: For handle OpenGL buffer
 * Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */
#pragma once
#include <vector>
#include <GL/glew.h>

class Buffer
{
public:
	Buffer(GLenum type, unsigned size, GLenum usage, void* data, int storageIndex_ = 0);
	void Bind(unsigned uniformBufferSlot = 0);
	void BindStorage(int index);
	void BindStorage() const;
	void BindStorageBuffer(int storageIndex, unsigned size);
	void UnBind();

	template <typename T>
	void WriteData(void* data);

	void GetData(void* data) const;
	
	unsigned GetId();
	

	template <typename T>
	T* GetData();

	void UnMap();

	~Buffer();

	int GetSize();


private:
	unsigned bufferId{};
	GLenum type;
	int storageIndex;
	int size;
	
};

template <typename T>
void Buffer::WriteData(void* data)
{
	if (type == GL_SHADER_STORAGE_BUFFER)
	{
		BindStorage();
	}
	else if (type == GL_ARRAY_BUFFER)
	{
		Bind();
	}

	T* writeVal = static_cast<T*>(glMapBufferRange(type, 0, size,
		GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));

	memcpy(writeVal, data, size);

	glUnmapBuffer(type);
}

inline void Buffer::GetData(void* data) const
{
	BindStorage();

	glGetBufferSubData(type, 0, size, data);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, storageIndex, 0);
}

template <typename T>
T* Buffer::GetData()
{
	if(type == GL_SHADER_STORAGE_BUFFER)
	{
		BindStorage();
	}

	const void* dataPtr = glMapBufferRange(type, 0, size, GL_MAP_READ_BIT);
	assert(dataPtr);
	T* checkPtr = new T[size];
	memcpy(checkPtr, dataPtr, size);
	glUnmapBuffer(type);

	return checkPtr;
}

inline Buffer::Buffer(GLenum type, unsigned sizeVal, GLenum usage, void* data, int storageIndex_) : type(type)
{
	storageIndex = storageIndex_;
	size = static_cast<int>(sizeVal);
	
	glGenBuffers(1, &bufferId);
	glBindBuffer(type, bufferId);
	glBufferData(type, size, data, usage);

	if (type == GL_SHADER_STORAGE_BUFFER)
		glBindBufferBase(type, storageIndex, bufferId);
}

inline void Buffer::Bind(unsigned uniformBufferSlot)
{
	switch(type)
	{
	case GL_UNIFORM_BUFFER:
		glBindBufferBase(GL_UNIFORM_BUFFER, uniformBufferSlot, bufferId);
		break;
	case GL_ARRAY_BUFFER:
	case GL_SHADER_STORAGE_BUFFER:
		glBindBufferBase(type, storageIndex, bufferId);
	case GL_ELEMENT_ARRAY_BUFFER:
		glBindBuffer(type, bufferId);
	default: ;
	}
}

inline void Buffer::BindStorage(int index)
{
	storageIndex = index;
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, bufferId);
}

inline void Buffer::BindStorage() const
{
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, storageIndex, bufferId);
}

inline void Buffer::BindStorageBuffer(int storageIndexVal, unsigned sizeVal)
{
	glBindBufferRange(GL_SHADER_STORAGE_BUFFER, storageIndexVal, bufferId, 0, sizeVal);
}

inline void Buffer::UnBind()
{
	glBindBuffer(type, 0);
}

inline unsigned Buffer::GetId()
{
	return bufferId;
}

inline void Buffer::UnMap()
{
	glUnmapBuffer(type);
}

inline Buffer::~Buffer()
{
	glDeleteBuffers(1, &bufferId);
}

inline int Buffer::GetSize()
{
	return size;
}
