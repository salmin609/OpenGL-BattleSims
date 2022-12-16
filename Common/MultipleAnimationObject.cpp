#include "MultipleAnimationObject.h"

#include <assimp/scene.h>

#include "AnimationModel.h"

MultipleAnimationObject::MultipleAnimationObject(AnimationModel* model, glm::vec3 posVal, glm::vec3 rotVal,
                                                 glm::vec3 scaleVal, bool isInstance_, float instancingOffset_) : Object(posVal, rotVal, scaleVal, isInstance_, instancingOffset_)
{
	animationModels.push_back(model);
	animationModel = animationModels[currentAnimationIndex];
}

MultipleAnimationObject::~MultipleAnimationObject()
{

}

void MultipleAnimationObject::Draw(const glm::mat4& projViewMat, float animationT, int transformsOffset,
	std::vector<glm::mat4> transforms)
{
	animationModels[currentAnimationIndex]->Draw(GetModelMatrix(), projViewMat, animationT, transformsOffset,
		instancingOffset, transforms, isInstance);
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
