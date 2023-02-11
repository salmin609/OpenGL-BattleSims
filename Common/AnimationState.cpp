#include "AnimationState.h"

#include "AnimationModel.h"

AnimationState::AnimationState()
{

}

AnimationState::~AnimationState()
{

}

void AnimationState::AddAnimation(State animState, AnimationModel* model)
{
	model->thisState = animState;


	switch(animState)
	{
	case State::Idle:
		idleAnimations.push_back(model);
		break;
	case State::Attack:
		attackAnimations.push_back(model);
		break;
	case State::Pain:
		painAnimations.push_back(model);
		break;
	case State::Run:
		runAnimation = model;
		break;
	case State::Death:
		deathAnimation = model;
		break;
	default: ;
	}
}

AnimationModel* AnimationState::RequestAnimation(int state)
{
	std::vector<AnimationModel*> anis;
	switch(state)
	{
	case static_cast<int>(State::Idle):
		anis = idleAnimations;
		break;
	case static_cast<int>(State::Attack):
		anis = attackAnimations;
		break;
	case static_cast<int>(State::Pain):
		anis = painAnimations;
		break;
	case static_cast<int>(State::Run):
		anis = std::vector<AnimationModel*>{runAnimation};
		break;
	case static_cast<int>(State::Death):
		anis = std::vector<AnimationModel*>{deathAnimation};
		break;
	default: 
		break;
	}

	for (AnimationModel* ani : anis)
	{
		if (ani->playingStatus == AnimationModel::PlayingStatus::Ready)
			return ani;
	}
	return nullptr;
}
