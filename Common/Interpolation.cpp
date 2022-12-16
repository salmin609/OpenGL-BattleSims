/*
 * Author		: Ryan Kim.
 * Date			: 2022-10-07
 * Description	: Functions for interpolation.
 * Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#include "Interpolation.h"
#include <assimp/scene.h>
#include <glm/mat4x4.hpp>

#include "Quaternion.h"



namespace Interpolation
{
	/*
	 * Find key index between assimp animation nodes
	 */
	glm::uint FindScaling(float AnimationTimeTicks, const aiNodeAnim* pNodeAnim)
	{
		assert(pNodeAnim->mNumScalingKeys > 0);

		for (glm::uint i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++) {
			float t = (float)pNodeAnim->mScalingKeys[i + 1].mTime;
			if (AnimationTimeTicks < t) {
				return i;
			}
		}

		return 0;
	}

	/*
	 * Scaling Interpolation.
	 * Lerp was used.
	 */
	void CalcInterpolatingScaling(aiVector3D& out, float animationTimeTicks, const aiNodeAnim* pNodeAnim)
	{
		if (pNodeAnim->mNumScalingKeys == 1)
		{
			out = pNodeAnim->mScalingKeys[0].mValue;
			return;
		}
		glm::uint scalingIndex = FindScaling(animationTimeTicks, pNodeAnim);
		glm::uint nextScalingIndex = scalingIndex + 1;
		assert(nextScalingIndex < pNodeAnim->mNumScalingKeys);

		float t1 = (float)pNodeAnim->mScalingKeys[scalingIndex].mTime;
		float t2 = (float)pNodeAnim->mScalingKeys[nextScalingIndex].mTime;
		float dt = t2 - t1;
		float factor = (animationTimeTicks - t1) / dt;
		assert(factor >= 0.0f && factor <= 1.f);

		const aiVector3D& start = pNodeAnim->mScalingKeys[scalingIndex].mValue;
		const aiVector3D& end = pNodeAnim->mScalingKeys[nextScalingIndex].mValue;
		aiVector3D delta = end - start;

		out = start + delta * factor;
	}

	/*
	 * Find key index between assimp animation nodes
	 */
	glm::uint FindRotation(float AnimationTimeTicks, const aiNodeAnim* pNodeAnim)
	{
		assert(pNodeAnim->mNumRotationKeys > 0);

		for (glm::uint i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++) {
			float t = (float)pNodeAnim->mRotationKeys[i + 1].mTime;
			if (AnimationTimeTicks < t) {
				return i;
			}
		}

		return 0;
	}

	/*
	* Rotation Interpolation.
	* SLerp was used.
	*/
	void CalcInterpolatedRotation(Quaternion& out, float animationTimeTicks, const aiNodeAnim* pNodeAnim)
	{
		if (pNodeAnim->mNumRotationKeys == 1)
		{
			out = pNodeAnim->mRotationKeys[0].mValue;
			return;
		}

		const glm::uint rotationIndex = FindRotation(animationTimeTicks, pNodeAnim);
		const glm::uint nextRotationIndex = rotationIndex + 1;

		assert(nextRotationIndex < pNodeAnim->mNumRotationKeys);

		const float t1 = static_cast<float>(pNodeAnim->mRotationKeys[rotationIndex].mTime);
		const float t2 = static_cast<float>(pNodeAnim->mRotationKeys[nextRotationIndex].mTime);
		const float dt = t2 - t1;

		float factor = (animationTimeTicks - t1) / dt;
		assert(factor >= 0.0f && factor <= 1.f);

		const Quaternion startRotation = pNodeAnim->mRotationKeys[rotationIndex].mValue;
		const Quaternion endRotation = pNodeAnim->mRotationKeys[nextRotationIndex].mValue;

		out.Interpolate(startRotation, endRotation, factor);
		out.Normalize();

	}

	/*
	 * Find key index between assimp animation nodes
	 */
	glm::uint FindPosition(float AnimationTimeTicks, const aiNodeAnim* pNodeAnim)
	{
		for (glm::uint i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) {
			float t = (float)pNodeAnim->mPositionKeys[i + 1].mTime;
			if (AnimationTimeTicks < t) {
				return i;
			}
		}

		return 0;
	}

	/*
	* Translation Interpolation.
	* Lerp was used.
	*/
	void CalcInterpolatedPosition(aiVector3D& out, float animationTimeTicks, const aiNodeAnim* pNodeAnim)
	{
		if (pNodeAnim->mNumPositionKeys == 1)
		{
			out = pNodeAnim->mPositionKeys[0].mValue;
			return;
		}

		glm::uint positionIndex = FindPosition(animationTimeTicks, pNodeAnim);
		glm::uint nextPositionIndex = positionIndex + 1;
		assert(nextPositionIndex < pNodeAnim->mNumPositionKeys);

		float t1 = (float)pNodeAnim->mPositionKeys[positionIndex].mTime;
		float t2 = (float)pNodeAnim->mPositionKeys[nextPositionIndex].mTime;
		float dt = t2 - t1;

		float factor = (animationTimeTicks - t1) / dt;
		assert(factor >= 0.0f && factor <= 1.f);

		aiVector3D start = pNodeAnim->mPositionKeys[positionIndex].mValue;
		aiVector3D end = pNodeAnim->mPositionKeys[nextPositionIndex].mValue;
		aiVector3D delta = end - start;
		out = start + delta * factor;
	}
}

