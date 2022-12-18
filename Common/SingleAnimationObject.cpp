/*
 *	File for GAM400 - OpenGL MAs project.
 *	Term			- 2022F
 *	Author			- Ryan Kim
 *  Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#include "SingleAnimationObject.h"

#include <assimp/scene.h>

#include "AnimationModel.h"


SingleAnimationObject::SingleAnimationObject(AnimationModel* model, glm::vec3 posVal, glm::vec3 rotVal,
                                             glm::vec3 scaleVal, bool isInstance_, const float instancingOffset_) : Object(posVal, rotVal, scaleVal)
{
	animationModel = model;
}

SingleAnimationObject::~SingleAnimationObject()
{
}

void SingleAnimationObject::Draw(const glm::mat4& projViewMat, float animationT, int transformsOffset,
	std::vector<glm::mat4> transforms)
{
	animationModel->Draw(GetModelMatrix(), projViewMat, animationT, transformsOffset,
		transforms);
}

std::chrono::system_clock::time_point SingleAnimationObject::GetAnimationStartTime() const
{
	return animationModel->startTime;
}

void SingleAnimationObject::ResetAnimationStartTime()
{
	animationModel->startTime = std::chrono::system_clock::now();
}

aiAnimation* SingleAnimationObject::GetAiAnimation()
{
	return animationModel->GetScene()->mAnimations[0];
}

std::vector<glm::mat4> SingleAnimationObject::Interpolate(float animationTimeTicks)
{
	return animationModel->Interpolate(animationTimeTicks);
}
