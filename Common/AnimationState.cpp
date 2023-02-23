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

AnimationModel* AnimationState::RequestAnimation(State newState, int randomIndex)
{
	std::vector<AnimationModel*> anis;

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
		anis = runAnimations;
		break;
	case State::Death:
		anis = deathAnimations;
		break;
	case State::END: 
		break;
	default:  
		break;
	}
	return anis[randomIndex];



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

	//return nullptr;
}
