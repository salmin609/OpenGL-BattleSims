#pragma once
#include <vector>

class AnimationModel;

class AnimationState
{
public:
	enum class State
	{
		Idle = 0,
		Attack,
		Pain,
		Run,
		Death
	};

	AnimationState();
	~AnimationState();

	std::vector<AnimationModel*> idleAnimations;
	std::vector<AnimationModel*> attackAnimations;
	std::vector<AnimationModel*> painAnimations;

	AnimationModel* runAnimation;
	AnimationModel* deathAnimation;
private:
	
};
