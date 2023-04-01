/*
 * Author		: Ryan Kim.
 * Date			: 2023-03-17
 * Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#pragma once

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
private:
	HerdManager* herdManager;
	void SetShaderUniforms() override;
	void PopulateBuffers() override;
	BillboardObjectManager* boObjManager;

};
