#pragma once

#include "ComputeShaderClass.h"

class BillboardObjectManager;
class HerdManager;
class BufferManager;
class Shader;

//need to change to derived class
class BillboardMovingCS final : public ComputeShaderClass
{
public:
	BillboardMovingCS(Shader* boMovingShader_, HerdManager* herdManager_,
		BillboardObjectManager* boObjManager_);
	~BillboardMovingCS() override;
	void Move(float dt) const;
private:
	void SetShaderUniforms() override;
	void PopulateBuffers() override;
	HerdManager* herdManager;
	int* animationIndexBuffer;
	BillboardObjectManager* boObjManager;
};
