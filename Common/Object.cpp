/*
 * Author		: Ryan Kim.
 * Date			: 2022-10-07
 * Description	: Class for store animating object's infos (position, rotation, scaling, animationModel)
 * Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#include "Object.h"

#include <glm/gtc/matrix_transform.hpp>

Object::Object(glm::vec3 posVal, glm::vec3 rotVal, glm::vec3 scaleVal, bool isInstance_, float instancingOffset_)
{
	//animationModel = model;
	pos = posVal;
	rot = rotVal;
	scale = scaleVal;
	instancingOffset = instancingOffset_;
	isInstance = isInstance_;
	//animationIndex = animationIndex_;
}

Object::~Object()
{
}

glm::mat4 Object::GetModelMatrix()
{
	glm::mat4 modelMat = glm::mat4(1.f);

	modelMat = glm::scale(modelMat, scale);

	modelMat = glm::rotate(modelMat, 1.5708f, rot);
	modelMat = glm::translate(modelMat, pos);

	return modelMat;
}


//void Object::Draw(
//	const glm::mat4& projViewMat, float animationT, int transformsOffset,
//	std::vector<glm::mat4> transforms)
//{
//	animationModel->Draw(GetModelMatrix(), projViewMat, animationT, transformsOffset,
//		instancingOffset, transforms, isInstance);
//}
//
//std::chrono::system_clock::time_point Object::GetAnimationStartTime() const
//{
//	return animationModel->startTime;
//}
//
//void Object::ResetAnimationStartTime()
//{
//	animationModel->startTime = std::chrono::system_clock::now();
//}
//
//
