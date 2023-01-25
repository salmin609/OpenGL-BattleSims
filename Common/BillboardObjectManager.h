#pragma once
#include "glm/mat4x4.hpp"

class BillboardFrameBufferUsageCS;
class HerdManager;
class Herd;
class BufferManager;
class Texture;
class BillboardAnimatingDatas;
class Buffer;
struct Frustum;
class Camera;
class BillboardManager;
class BillBoardObject;
class Shader;

class BillboardObjectManager
{
public:
	BillboardObjectManager(Shader* boShader_, BillboardManager* boManager_,
		Shader* boFrameBufferUsageComputeShader,
		Camera* currentCam_);
	~BillboardObjectManager();

	//void Populate();
	void CheckFrameBufferUsage();
	void Render(const glm::mat4& projMat, const glm::mat4& viewMat);
	//void SetShaderUniforms();
	//void PopulateBuffers();

	//void* boFBusageDatas{};

	HerdManager* herdManager;
private:
	Shader* boShader;
	Shader* boFBusageComputeShader;
	BillboardManager* boManager;
	//BufferManager* csBuffers;
	Camera* currentCam;
	BillboardFrameBufferUsageCS* boFBusageCS;
};
