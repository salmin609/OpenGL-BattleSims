#pragma once

#include "ComputeShaderClass.h"

class BillboardObjectManager;
class HerdManager;

class BillboardAttackCS final : public ComputeShaderClass
{
public:
	BillboardAttackCS(Shader* shader_, HerdManager* herdManager_,
		BillboardObjectManager* boObjManager_);
	~BillboardAttackCS() override;
	void AttackComputation(float dt);
	float* timers;
private:
	HerdManager* herdManager;
	void SetShaderUniforms() override;
	void PopulateBuffers() override;
	BillboardObjectManager* boObjManager;
};
