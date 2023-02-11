#pragma once
#include <vector>

#include "ModelKinds.hpp"

class AnimationModel;

class AnimationState
{
public:
	AnimationState();
	~AnimationState();
	void AddAnimation(State animState, AnimationModel* model);
	AnimationModel* RequestAnimation(int state);

	std::vector<AnimationModel*> idleAnimations;
	std::vector<AnimationModel*> attackAnimations;
	std::vector<AnimationModel*> painAnimations;

	AnimationModel* runAnimation;
	AnimationModel* deathAnimation;
private:
	
};
