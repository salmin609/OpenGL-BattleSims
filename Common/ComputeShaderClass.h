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
