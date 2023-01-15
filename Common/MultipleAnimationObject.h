/*
 *	File for GAM400 - OpenGL MAs project.
 *	Term			- 2022F
 *	Author			- Ryan Kim
 *  Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#pragma once
#include "Object.h"

class Timer;

class MultipleAnimationObject final : public Object
{
public:
	MultipleAnimationObject(glm::vec3 posVal, glm::vec3 rotVal, glm::vec3 scaleVal);
	~MultipleAnimationObject() override;

	void Draw(const glm::mat4& projViewMat,
		glm::mat4* transforms) override;
	aiAnimation* GetAiAnimation() override;
	std::chrono::system_clock::time_point GetAnimationStartTime() const override;
	void ResetAnimationStartTime() override;
	glm::mat4* Interpolate(float animationTimeTicks) override;
	void AddAnimation(AnimationModel* model);
	void IncrementIndex();
	void SetAnimationIndex(int index);
	void CheckCurrentIndexRange();
	void ChangeCurrentAnimationWithTime();

	std::vector<AnimationModel*> animationModels;
	AnimationModel* currentAnimationModel;
private:
	int currentAnimationIndex = 0;
	Timer* timer;
};
