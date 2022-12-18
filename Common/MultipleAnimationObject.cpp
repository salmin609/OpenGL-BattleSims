/*
 *	File for GAM400 - OpenGL MAs project.
 *	Term			- 2022F
 *	Author			- Ryan Kim
 *  Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#include "MultipleAnimationObject.h"

#include <assimp/scene.h>

#include "AnimationModel.h"
#include "Timer.hpp"

MultipleAnimationObject::MultipleAnimationObject(glm::vec3 posVal, glm::vec3 rotVal,
                                                 glm::vec3 scaleVal) : Object(posVal, rotVal, scaleVal)
{
	currentAnimationModel = nullptr;
	timer = new Timer();
}

MultipleAnimationObject::~MultipleAnimationObject()
{
	delete timer;
}

void MultipleAnimationObject::Draw(const glm::mat4& projViewMat,
	std::vector<glm::mat4> transforms)
{
	animationModels[currentAnimationIndex]->Draw(GetModelMatrix(), projViewMat, 
		transforms);
}

aiAnimation* MultipleAnimationObject::GetAiAnimation()
{
	return animationModels[currentAnimationIndex]->GetScene()->mAnimations[0];
}

std::chrono::system_clock::time_point MultipleAnimationObject::GetAnimationStartTime() const
{
	return animationModels[currentAnimationIndex]->startTime;
}

void MultipleAnimationObject::ResetAnimationStartTime()
{
	animationModels[currentAnimationIndex]->startTime = std::chrono::system_clock::now();
}

std::vector<glm::mat4> MultipleAnimationObject::Interpolate(float animationTimeTicks)
{
	return animationModels[currentAnimationIndex]->Interpolate(animationTimeTicks);
}

void MultipleAnimationObject::AddAnimation(AnimationModel* model)
{
	animationModels.push_back(model);
	currentAnimationModel = animationModels[currentAnimationIndex];
}

void MultipleAnimationObject::IncrementIndex()
{
	currentAnimationIndex++;

	CheckCurrentIndexRange();

	currentAnimationModel = animationModels[currentAnimationIndex];
}

void MultipleAnimationObject::SetAnimationIndex(int index)
{
	currentAnimationIndex = index;

	CheckCurrentIndexRange();

	currentAnimationModel = animationModels[currentAnimationIndex];
}

void MultipleAnimationObject::CheckCurrentIndexRange()
{
	if (currentAnimationIndex < 0)
		currentAnimationIndex = static_cast<int>(animationModels.size()) - 1;

	else if (currentAnimationIndex >= static_cast<int>(animationModels.size()))
		currentAnimationIndex = 0;
}

void MultipleAnimationObject::ChangeCurrentAnimationWithTime()
{
	float animationDuration = static_cast<float>(
		currentAnimationModel->GetScene()->mAnimations[0]->mDuration);

	if (animationDuration < 2.f)
		animationDuration = 2.f;


	if(timer->CheckTime(animationDuration))
		IncrementIndex();
}
