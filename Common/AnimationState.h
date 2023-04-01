/*
 * Author		: Ryan Kim.
 * Date			: 2023-03-17
 * Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#pragma once
#include <vector>

#include "ModelKinds.h"

class AnimationModel;

class AnimationState
{
public:
	AnimationState();
	~AnimationState();
	void AddAnimation(State animState, AnimationModel* model);
	AnimationModel* RequestAnimation(State newState, int randomIndex);

	std::vector<AnimationModel*> idleAnimations;
	std::vector<AnimationModel*> attackAnimations;
	std::vector<AnimationModel*> painAnimations;
	std::vector<AnimationModel*> runAnimations;
	std::vector<AnimationModel*> deathAnimations;
private:
	
};
