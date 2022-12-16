#pragma once
#include "Object.h"


struct aiAnimation;

class SingleAnimationObject : public Object
{
public:
	SingleAnimationObject(AnimationModel* model, 
		glm::vec3 posVal, glm::vec3 rotVal, glm::vec3 scaleVal, 
		bool isInstance_, float instancingOffset_);
	~SingleAnimationObject() override;
	void Draw(
		const glm::mat4& projViewMat, float animationT, int transformsOffset,
		std::vector<glm::mat4> transforms) override;

	std::chrono::system_clock::time_point GetAnimationStartTime() const override;
	void ResetAnimationStartTime() override;
	aiAnimation* GetAiAnimation() override;
	std::vector<glm::mat4> Interpolate(float animationTimeTicks) override;
	AnimationModel* animationModel;

private:

};
