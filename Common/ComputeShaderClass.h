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
	Shader* shader;

private:
	virtual void SetShaderUniforms() = 0;
	virtual void PopulateBuffers() = 0;
};
