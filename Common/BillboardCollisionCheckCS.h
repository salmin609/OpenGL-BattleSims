#pragma once

#include <vector>

#include "ComputeShaderClass.h"

class BillboardObjectManager;
class HerdManager;

class BillboardCollisionCheckCS final : public ComputeShaderClass
{
public:
	BillboardCollisionCheckCS(Shader* shader_, HerdManager* herdManager_,
		BillboardObjectManager* boObjManager_);
	~BillboardCollisionCheckCS() override;
	void CollisionCheck(float dt) const;
	void ResetCollisionCheckBuffer();
private:
	HerdManager* herdManager;
	void SetShaderUniforms() override;
	void PopulateBuffers() override;
	BillboardObjectManager* boObjManager;
	std::vector<int> collisionStatusVec;

};
