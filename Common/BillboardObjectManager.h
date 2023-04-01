/*
 * Author		: Ryan Kim.
 * Date			: 2023-03-17
 * Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#pragma once
#include "glm/mat4x4.hpp"

class BillboardBufferResetCS;
class BillboardRangeAttackTimerCS;
class BillboardCollisionCheckCS;
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
		Shader* lineShader_,
		Shader* bbCollisionCheck_,
		Shader* bbRangeAttackTimer_,
		Shader* bbResetBuffer_);
	~BillboardObjectManager();

	void CalculateBOAngle() const;
	void Move(float dt) const;
	void Attack(float dt) const;
	void CollisionCheck(float dt) const;
	void ResetAnimationState() const;
	void Render(const glm::mat4& projMat, const glm::mat4& viewMat) const;
	void ChangeAnimationOfHerds() const;
	void CheckHerdReachedDestination() const;
	void CheckRangeAttackTimer(float dt) const;
	void PopulateBuffer();
	void RunSimulation(float dt) const;
	void ResetBuffer() const;

	HerdManager* herdManager;
	BillboardMovingCS* boMovingCS;
	BillboardAttackCS* boAttackCS;
	BillboardAnimationChangeCS* boAnimChangeCS;
	BillboardCollisionCheckCS* boCollisionCheckCS;
	BillboardRangeAttackTimerCS* boRangeAttackTimerCS;
	BillboardBufferResetCS* boResetBufferCS;

	BufferManager* csBuffers{};
	
private:
	Shader* boShader;
	Shader* boFBusageComputeShader;
	BillboardManager* boManager;
	Camera* currentCam;
	BillboardFrameBufferUsageCS* boFBusageCS;
	
};
