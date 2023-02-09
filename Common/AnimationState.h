#pragma once
#include <vector>

class AnimationModel;

class AnimationState
{
public:
	AnimationState();
	~AnimationState();

	std::vector<AnimationModel*> idleAnimations;
	std::vector<AnimationModel*> attackAnimations;
	std::vector<AnimationModel*> painAnimations;

	AnimationModel* runAnimation;
	AnimationModel* deathAnimation;
private:
	
};
