/*
 * Author		: Ryan Kim.
 * Date			: 2023-03-17
 * Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#pragma once

#include "ComputeShaderClass.h"

class BillboardObjectManager;
class BufferManager;
class HerdManager;
class Camera;
class Shader;

class BillboardFrameBufferUsageCS final : public ComputeShaderClass
{
public:
	BillboardFrameBufferUsageCS(Shader* boFBusageComputeShader_, Camera* currentCam_,
		HerdManager* herdManager_, BillboardObjectManager* boObjManager_);
	~BillboardFrameBufferUsageCS() override;
	void CalculateBOAngle() const;
	int* boFBusageDatas{};

private:
	void SetShaderUniforms() override;
	void PopulateBuffers() override;

	BillboardObjectManager* boObjManager;
	Camera* currentCam;
	HerdManager* herdManager;
};
