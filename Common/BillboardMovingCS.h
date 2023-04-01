/*
 * Author		: Ryan Kim.
 * Date			: 2023-03-17
 * Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

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
	int* animationIndexBuffer;
	int* herdReachedBuffer;

private:
	void SetShaderUniforms() override;
	void PopulateBuffers() override;
	HerdManager* herdManager;
	BillboardObjectManager* boObjManager;
};
