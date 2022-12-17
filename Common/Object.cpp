/*
 * Author		: Ryan Kim.
 * Date			: 2022-10-07
 * Description	: Class for store animating object's infos (position, rotation, scaling, animationModel)
 * Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#include "Object.h"

#include <glm/gtc/matrix_transform.hpp>

Object::Object(glm::vec3 posVal, glm::vec3 rotVal, glm::vec3 scaleVal, float instancingOffset_)
{
	pos = posVal;
	rot = rotVal;
	scale = scaleVal;
	instancingOffset = instancingOffset_;
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
