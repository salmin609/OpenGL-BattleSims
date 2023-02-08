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
	void CheckFrameBufferUsage() const;
private:
	void SetShaderUniforms() override;
	void PopulateBuffers() override;

	BillboardObjectManager* boObjManager;
	Camera* currentCam;
	HerdManager* herdManager;
	int* boFBusageDatas{};
};
