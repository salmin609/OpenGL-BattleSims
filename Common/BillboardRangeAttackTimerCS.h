#pragma once

#include "ComputeShaderClass.h"

class BillboardObjectManager;
class HerdManager;
class BufferManager;
class Shader;

//need to change to derived class
class BillboardRangeAttackTimerCS final : public ComputeShaderClass
{
public:
	BillboardRangeAttackTimerCS(Shader* boRangeAttackTimer_, HerdManager* herdManager_,
		BillboardObjectManager* boObjManager_);
	~BillboardRangeAttackTimerCS() override;
	void TimeCheck(float dt) const;
	

private:
	void SetShaderUniforms() override;
	void PopulateBuffers() override;
	HerdManager* herdManager;
	BillboardObjectManager* boObjManager;
};
