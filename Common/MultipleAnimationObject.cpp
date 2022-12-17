#include "MultipleAnimationObject.h"

#include <assimp/scene.h>

#include "AnimationModel.h"

MultipleAnimationObject::MultipleAnimationObject(glm::vec3 posVal, glm::vec3 rotVal,
                                                 glm::vec3 scaleVal) : Object(posVal, rotVal, scaleVal)
{
	//animationModels.push_back(model);
	//animationModel = animationModels[currentAnimationIndex];
	animationModel = nullptr;
}

MultipleAnimationObject::~MultipleAnimationObject()
{

}

void MultipleAnimationObject::Draw(const glm::mat4& projViewMat, float animationT, int transformsOffset,
	std::vector<glm::mat4> transforms)
{
	animationModels[currentAnimationIndex]->Draw(GetModelMatrix(), projViewMat, animationT, transformsOffset, 
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
	animationModel = animationModels[currentAnimationIndex];
}

void MultipleAnimationObject::IncrementIndex()
{
	currentAnimationIndex++;
	animationModel = animationModels[currentAnimationIndex];
}

void MultipleAnimationObject::SetAnimationIndex(int index)
{
	currentAnimationIndex = index;
	animationModel = animationModels[currentAnimationIndex];
}
