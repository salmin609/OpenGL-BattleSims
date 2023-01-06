/*
 * Author		: Ryan Kim.
 * Date			: 2022-10-07
 * Description	: Class for store animating object's infos (position, rotation, scaling, animationModel)
 * Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#include "Object.h"

#include <glm/gtc/matrix_transform.hpp>

Object::Object(glm::vec3 posVal, glm::vec3 rotVal, glm::vec3 scaleVal)
{
	pos = posVal;
	rot = rotVal;
	scale = scaleVal;
}

Object::~Object()
{
}

glm::mat4 Object::GetModelMatrix() const
{
	glm::mat4 modelMat = glm::mat4(1.f);

	//modelMat = glm::rotate(modelMat, 1.5708f, rot);
	modelMat = glm::translate(modelMat, pos);
	modelMat = glm::scale(modelMat, scale);

	return modelMat;
}
