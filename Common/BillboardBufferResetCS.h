#pragma once

#include "ComputeShaderClass.h"

class BillboardObjectManager;
class HerdManager;

class BillboardBufferResetCS final : public ComputeShaderClass
{
public:
	BillboardBufferResetCS(Shader* shader_, HerdManager* herdManager_,
		BillboardObjectManager* boObjManager_);
	~BillboardBufferResetCS();
	void ResetBuffer();

private:
	HerdManager* herdManager;
	void SetShaderUniforms() override;
	void PopulateBuffers() override;
	BillboardObjectManager* boObjManager;
};
