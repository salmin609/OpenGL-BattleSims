#pragma once
#include "glm/mat4x4.hpp"

class BillboardAttackCS;
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
		Shader* boMovingShader_,
		Shader* boAttackShader_);
	~BillboardObjectManager();

	void CheckFrameBufferUsage();
	void Move(float dt);
	void Attack(float dt);
	void Render(const glm::mat4& projMat, const glm::mat4& viewMat);

	HerdManager* herdManager;
	BillboardMovingCS* boMovingCS;
	BillboardAttackCS* boAttackCS;
private:
	Shader* boShader;
	Shader* boFBusageComputeShader;
	BillboardManager* boManager;
	Camera* currentCam;
	BillboardFrameBufferUsageCS* boFBusageCS;
	
};
