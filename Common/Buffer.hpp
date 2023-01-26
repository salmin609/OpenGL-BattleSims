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
	~Buffer();

	void Bind();
	void BindStorage(int index);
	void BindStorage() const;
	void BindStorageBuffer(int storageIndex, unsigned size);
	void UnBind();
	void WriteData(void* data);
	void GetData(void* data) const;
	unsigned GetId();
	void UnMap();
	int GetSize();
	int GetStorageIndex();

	template <typename T>
	std::vector<T> GetDataVector();


	template <typename T>
	T* GetData();

	template <typename T>
	int GetCount();

private:
	unsigned bufferId{};
	GLenum type;
	int baseStorageIndex;
	int size;
	int count;

};


inline void Buffer::WriteData(void* data)
{
	if (type == GL_SHADER_STORAGE_BUFFER)
	{
		BindStorage();
	}
	else if (type == GL_ARRAY_BUFFER)
	{
		Bind();
	}

	void* writeVal = glMapBufferRange(type, 0, size,
		GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

	memcpy(writeVal, data, size);

	glUnmapBuffer(type);
}

inline void Buffer::GetData(void* data) const
{
	BindStorage();

	glGetBufferSubData(type, 0, size, data);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, baseStorageIndex, 0);
}

template <typename T>
std::vector<T> Buffer::GetDataVector()
{
	if (type == GL_SHADER_STORAGE_BUFFER)
	{
		BindStorage();
	}

	T* checkVal = static_cast<T*>(glMapBufferRange(type, 0, size,
		GL_MAP_READ_BIT));
	std::vector<T> check;
	int sizeCheck = size / sizeof(T);
	check.reserve(size);
	for (int i = 0; i < sizeCheck; ++i)
	{
		check.push_back(checkVal[i]);
	}

	glUnmapBuffer(type);

	return check;
}

template <typename T>
T* Buffer::GetData()
{
	Bind();
	const void* dataPtr = glMapBufferRange(type, 0, size, GL_MAP_READ_BIT);
	T* checkPtr = new T[size];
	memcpy(checkPtr, dataPtr, size);
	glUnmapBuffer(type);

	return checkPtr;
}

inline Buffer::Buffer(GLenum type, unsigned sizeVal, GLenum usage, void* data, int storageIndex_) : type(type)
{
	baseStorageIndex = storageIndex_;
	size = static_cast<int>(sizeVal);

	glGenBuffers(1, &bufferId);
	glBindBuffer(type, bufferId);
	glBufferData(type, size, data, usage);

	if (type == GL_SHADER_STORAGE_BUFFER)
		glBindBufferBase(type, baseStorageIndex, bufferId);
}

inline void Buffer::Bind()
{
	switch (type)
	{
	case GL_UNIFORM_BUFFER:
		//glBindBufferBase(GL_UNIFORM_BUFFER, uniformBufferSlot, bufferId);
		break;
	case GL_ARRAY_BUFFER:
	case GL_ELEMENT_ARRAY_BUFFER:
		glBindBuffer(type, bufferId);
		break;
	case GL_SHADER_STORAGE_BUFFER:
		BindStorage();
		break;
	default:
		break;
	}
}

inline void Buffer::BindStorage(int index)
{
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, bufferId);
}

inline void Buffer::BindStorage() const
{
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, baseStorageIndex, bufferId);
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

template <typename T>
int Buffer::GetCount()
{
	return size / sizeof(T);
}


inline int Buffer::GetStorageIndex()
{
	return baseStorageIndex;
}
