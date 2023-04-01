/*
 * Author		: Ryan Kim.
 * Date			: 2023-03-17
 * Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#pragma once

enum class ShaderValueType
{
	Int = 0,
	Float,
	Vec3,
	Vec4,
	Matrix4x4
};

struct ShaderUniformValue
{
	ShaderValueType type;
	void* data;
};

struct BufferDataType
{
	BufferDataType(int size_, void* data_) : size(size_), data(data_)
	{}
	int size;
	void* data;
};