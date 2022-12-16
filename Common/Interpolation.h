/*
 * Author		: Ryan Kim.
 * Date			: 2022-10-07
 * Description	: Functions for interpolation.
 * Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#pragma once

#include <assimp/vector3.h>
#include <glm/detail/type_int.hpp>

#include "Quaternion.h"

struct aiNodeAnim;

namespace Interpolation
{
	glm::uint FindScaling(float AnimationTimeTicks, const aiNodeAnim* pNodeAnim);

	void CalcInterpolatingScaling(aiVector3D& out, float animationTimeTicks, const aiNodeAnim* pNodeAnim);
	void CalcInterpolatedRotation(Quaternion& out, float animationTimeTicks, const aiNodeAnim* pNodeAnim);
	void CalcInterpolatedPosition(aiVector3D& out, float animationTimeTicks, const aiNodeAnim* pNodeAnim);
}
