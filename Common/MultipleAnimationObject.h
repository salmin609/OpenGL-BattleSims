#pragma once
#include "Object.h"

class MultipleAnimationObject : public Object
{
public:
	MultipleAnimationObject(glm::vec3 posVal, glm::vec3 rotVal, glm::vec3 scaleVal);

	~MultipleAnimationObject() override;

	void Draw(const glm::mat4& projViewMat, float animationT, int transformsOffset,
		std::vector<glm::mat4> transforms) override;
	aiAnimation* GetAiAnimation() override;
	std::chrono::system_clock::time_point GetAnimationStartTime() const override;
	void ResetAnimationStartTime() override;
	std::vector<glm::mat4> Interpolate(float animationTimeTicks) override;
	void AddAnimation(AnimationModel* model);
	void IncrementIndex();
	void SetAnimationIndex(int index);

	std::vector<AnimationModel*> animationModels;
	AnimationModel* animationModel;
private:
	int currentAnimationIndex = 0;
};
