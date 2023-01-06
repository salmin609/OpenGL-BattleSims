/*
 * Author		: Ryan Kim.
 * Date			: 2022-10-07
 * Description	: Class for store animating object's infos (position, rotation, scaling, animationModel)
 * Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#pragma once
#include <chrono>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>


struct aiAnimation;
struct aiScene;
class AnimationModel;

class Object
{
public:
	Object(glm::vec3 posVal, glm::vec3 rotVal, glm::vec3 scaleVal);
	virtual ~Object();
	glm::mat4 GetModelMatrix() const;
	virtual void Draw(
		const glm::mat4& projViewMat,
		std::vector<glm::mat4> transforms) = 0;
	virtual aiAnimation* GetAiAnimation() = 0;
	
	virtual std::chrono::system_clock::time_point GetAnimationStartTime() const = 0;
	virtual void ResetAnimationStartTime() = 0;
	virtual std::vector<glm::mat4> Interpolate(float animationTimeTicks) = 0;
	glm::vec3 pos, rot, scale;
private:
};