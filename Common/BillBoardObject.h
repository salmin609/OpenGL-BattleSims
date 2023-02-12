/*
 *	File for GAM400 - OpenGL MAs project.
 *	Term			- 2022F
 *	Author			- Ryan Kim
 *	Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#pragma once
#include <vector>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "ModelKinds.hpp"

class AnimationModel;
class AnimationState;
class Camera;
struct SphereBV;
struct Frustum;
class FrameBuffer;
class Buffer;
class Shader;

class BillBoardObject
{
public:
	BillBoardObject(Shader* shader_, 
		std::vector<std::vector<FrameBuffer*>>* fb_,
		AnimationState* animState_);
	~BillBoardObject();
	void Render(const glm::mat4& projMat, 
		const glm::mat4& viewMatWithoutRot, 
		const glm::vec4& pos);
	void ChangeFrameBufferAngle(int index);
	bool SetAnimation(int index);
	
	
	FrameBuffer* usingFrameBuffer = nullptr;
	std::vector<FrameBuffer*>* fbs{};
	std::vector<std::vector<FrameBuffer*>>* animFrames;
	AnimationState* animState;
	State currentState;
private:
	unsigned vao{};
	Shader* shader;
	unsigned currentAngleSlot;
};
