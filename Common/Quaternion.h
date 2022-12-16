/*
 * Author		: Ryan Kim.
 * Date			: 2022-10-07
 * Description	: Class for basic Quaternion, provide Slerp, in matrix transformation.
 * Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#pragma once
#include <assimp/quaternion.h>
#include <glm/mat4x4.hpp>

class Quaternion
{
public:
	Quaternion()
	{
		x = 0.f; y = 0.f; z = 0.f; w = 0.f;
	}
	Quaternion(float x_, float y_, float z_, float w_);
	Quaternion(const aiQuaternion& val);
	void Interpolate(const Quaternion& pStart, const Quaternion& pEnd, float factor);
	Quaternion& operator=(const aiQuaternion& val);
	void Normalize();
	glm::mat4 GetMatrix();
	~Quaternion();

private:
	float x, y, z, w;


};
