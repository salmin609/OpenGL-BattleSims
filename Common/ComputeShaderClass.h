#pragma once

class BufferManager;
class Shader;

class ComputeShaderClass
{
public:
	ComputeShaderClass(Shader* shader_);
	virtual ~ComputeShaderClass();

protected:
	Shader* shader;
	BufferManager* csBuffers{};

private:
	virtual void SetShaderUniforms() = 0;
	virtual void PopulateBuffers() = 0;
};
