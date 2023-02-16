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
		//runAnimation = model;
		runAnimations.push_back(model);
		break;
	case State::Death:
		//deathAnimation = model;
		deathAnimations.push_back(model);
		break;
	default: ;
	}
}

AnimationModel* AnimationState::RequestAnimation(int state)
{
	std::vector<AnimationModel*> anis;
	const State newState = static_cast<State>(state);
	switch(newState)
	{
	case State::Idle:
		anis = idleAnimations;
		break;
	case State::Attack:
		anis = attackAnimations;
		break;
	case State::Pain:
		anis = painAnimations;
		break;
	case State::Run:
		//anis = std::vector<AnimationModel*>{runAnimation};
		anis = runAnimations;
		break;
	case State::Death:
		//anis = std::vector<AnimationModel*>{deathAnimation};
		anis = deathAnimations;
		break;
	default: 
		break;
	}

	//if(newState == State::Attack /*|| newState == State::Death*/ ||
	//	newState == State::Pain)
	//{
	//	for (AnimationModel* ani : anis)
	//	{
	//		if (ani->playingStatus == AnimationModel::PlayingStatus::Ready)
	//			return ani;
	//	}

	//	//return idleAnimations[0];
	//}
	//else
	//{
	//	const int randomIndex = rand() % static_cast<int>(anis.size());

	//	return anis[randomIndex];
	//}
	const int randomIndex = rand() % static_cast<int>(anis.size());
	return anis[randomIndex];
	//return nullptr;
}
