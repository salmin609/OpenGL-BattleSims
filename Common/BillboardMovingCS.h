#pragma once

#include "ComputeShaderClass.h"

class HerdManager;
class BufferManager;
class Shader;

//need to change to derived class
class BillboardMovingCS final : public ComputeShaderClass
{
public:
	BillboardMovingCS(Shader* boMovingShader_, HerdManager* herdManager_);
	~BillboardMovingCS() override;
	void Move(float dt) const;
private:
	void SetShaderUniforms() override;
	void PopulateBuffers() override;
	HerdManager* herdManager;
	int* reached;
};
