/*
 *	File for GAM400 - OpenGL MAs project.
 *	Term			- 2022F
 *	Author			- Ryan Kim
 *  Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#pragma once
#include "Object.h"


struct aiAnimation;

class SingleAnimationObject : public Object
{
public:
	SingleAnimationObject(AnimationModel* model, 
		glm::vec3 posVal, glm::vec3 rotVal, glm::vec3 scaleVal);
	~SingleAnimationObject() override;
	void Draw(
		const glm::mat4& projViewMat,
		glm::mat4* transforms) override;

	std::chrono::system_clock::time_point GetAnimationStartTime() const override;
	void ResetAnimationStartTime() override;
	aiAnimation* GetAiAnimation() override;
	glm::mat4* Interpolate(float animationTimeTicks) override;
	AnimationModel* animationModel;

private:

};
