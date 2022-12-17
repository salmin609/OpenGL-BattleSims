/*
 *	File for GAM400 - OpenGL MAs project.
 *	Term			- 2022F
 *	Author			- Ryan Kim
 *	Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#pragma once
#include <glm/detail/func_geometric.hpp>

class Camera;

struct Plan
{
	glm::vec3 normal = { 0.f, 1.f, 0.f }; // unit vector
	float     distance = 0.f;        // Distance with origin

	Plan() = default;

	Plan(const glm::vec3 & p1, const glm::vec3 & norm)
		: normal(glm::normalize(norm)),
		distance(glm::dot(normal, p1))
	{}

	float getSignedDistanceToPlan(const glm::vec3 & point) const
	{
		return glm::dot(normal, point) - distance;
	}
};

struct Frustum
{
	Plan topFace;
	Plan bottomFace;

	Plan rightFace;
	Plan leftFace;

	Plan farFace;
	Plan nearFace;

	void ResetFrustumPlans(const Camera& cam, float aspect, float fovY,
		float zNear, float zFar);
};

struct SphereBV
{
	glm::vec3 center{ 0.f, 0.f, 0.f };
	float radius{ 0.f };

	SphereBV(const glm::vec3& inCenter, float inRadius)
		: center{ inCenter }, radius{ inRadius }
	{}

	bool isOnOrForwardPlan(const Plan& plan) 
	{
		return plan.getSignedDistanceToPlan(center) > -radius;
	}

	bool isOnFrustum(const Frustum& camFrustum)
	{
		return (isOnOrForwardPlan(camFrustum.leftFace) &&
			isOnOrForwardPlan(camFrustum.rightFace) &&
			isOnOrForwardPlan(camFrustum.farFace) &&
			isOnOrForwardPlan(camFrustum.nearFace) &&
			isOnOrForwardPlan(camFrustum.topFace) &&
			isOnOrForwardPlan(camFrustum.bottomFace));
	};
};
