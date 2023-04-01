/*
 * Author		: Ryan Kim.
 * Date			: 2023-03-17
 * Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#pragma once

class BufferManager;
class Shader;

class ComputeShaderClass
{
public:
	ComputeShaderClass(Shader* shader_);
	virtual ~ComputeShaderClass();
	BufferManager* csBuffers{};

protected:
	void SendBuffersAndUniforms(float dt) const;
	void Dispatch(int xGroupNum) const;
	Shader* shader;

private:
	virtual void SetShaderUniforms() = 0;
	virtual void PopulateBuffers() = 0;
};
