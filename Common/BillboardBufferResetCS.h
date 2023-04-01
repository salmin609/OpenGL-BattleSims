/*
 * Author		: Ryan Kim.
 * Date			: 2023-03-17
 * Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

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
