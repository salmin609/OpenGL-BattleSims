#pragma once

class BufferManager;
class HerdManager;
class Camera;
class Shader;

class BillboardFrameBufferUsageCS
{
public:
	BillboardFrameBufferUsageCS(Shader* boFBusageComputeShader_, Camera* currentCam_,
		HerdManager* herdManager_);
	~BillboardFrameBufferUsageCS();
	void CheckFrameBufferUsage();

private:
	void SetShaderUniforms();
	void PopulateBuffers();

	Shader* boFBusageComputeShader;
	Camera* currentCam;
	HerdManager* herdManager;
	BufferManager* csBuffers;
	void* boFBusageDatas{};
};
