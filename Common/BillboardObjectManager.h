#pragma once
#include "glm/mat4x4.hpp"

class BillboardMovingCS;
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
		Camera* currentCam_,
		Shader* boMovingShader_);
	~BillboardObjectManager();

	void CheckFrameBufferUsage();
	void Move();
	void MoveBos();
	void Render(const glm::mat4& projMat, const glm::mat4& viewMat);

	HerdManager* herdManager;
private:
	Shader* boShader;
	Shader* boFBusageComputeShader;
	BillboardManager* boManager;
	Camera* currentCam;
	BillboardFrameBufferUsageCS* boFBusageCS;
	BillboardMovingCS* boMovingCS;
};
