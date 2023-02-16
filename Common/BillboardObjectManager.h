#pragma once
#include "glm/mat4x4.hpp"

class BillboardAnimationChangeCS;
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
		Shader* boAttackShader_,
		Shader* boChangeAnimationShader_,
		Shader* lineShader_);
	~BillboardObjectManager();

	void CalculateBOAngle() const;
	void Move(float dt) const;
	void Attack(float dt) const;
	void ResetAnimationState() const;
	void Render(const glm::mat4& projMat, const glm::mat4& viewMat) const;
	void ChangeAnimationOfHerds() const;

	HerdManager* herdManager;
	BillboardMovingCS* boMovingCS;
	BillboardAttackCS* boAttackCS;
	BillboardAnimationChangeCS* boAnimChangeCS;
private:
	Shader* boShader;
	Shader* boFBusageComputeShader;
	BillboardManager* boManager;
	Camera* currentCam;
	BillboardFrameBufferUsageCS* boFBusageCS;
	
};
