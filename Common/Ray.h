/*
 * Author		: Ryan Kim.
 * Date			: 2023-03-17
 * Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#pragma once

#include <algorithm>
#include <glm/detail/func_geometric.hpp>

#include "glm/vec3.hpp"

struct Ray
{
	Ray(glm::vec3 pos, glm::vec3 d) : origin(pos), dir(d)
	{}
	glm::vec3 origin;
	glm::vec3 dir;
};

struct AABB
{
	AABB(glm::vec3 min, glm::vec3 max) : vMin(min), vMax(max)
	{}
	glm::vec3 vMin;
	glm::vec3 vMax;
};

struct Slab
{
	Slab(glm::vec3 n_) : n(n_), d0(0.f), d1(0.f)
	{}
	glm::vec3 n;
	float d0;
	float d1;
};

class Interval
{
public:
	Interval();
	~Interval();

	bool Intersect(Ray* ray, Slab slab);
	float t0, t1;
	glm::vec3 n0, n1;
};
inline bool Interval::Intersect(Ray* ray, Slab slab)
{
	float d0 = slab.d0;
	float d1 = slab.d1;
	glm::vec3 normal = slab.n;
	float temp = dot(normal, ray->dir);

	if (temp < 0.f - std::numeric_limits<float>::epsilon() ||
		temp > 0.f + std::numeric_limits<float>::epsilon())
	{
		t0 = -(d0 + dot(normal, ray->origin)) / (dot(normal, ray->dir));
		t1 = -(d1 + dot(normal, ray->origin)) / (dot(normal, ray->dir));

		if (t0 > t1)
			std::swap(t0, t1);

		if (slab.d0 > slab.d1)
		{
			n0 = -normal;
			n1 = normal;
		}
		else
		{
			n0 = normal;
			n1 = -normal;
		}

		return true;
	}
	float s0 = dot(normal, ray->origin) + d0;
	float s1 = dot(normal, ray->origin) + d1;

	if (s0 < 0 && s1 < 0 || s0 > 0 && s1 > 0)
	{
		//ray is btw planes.
		t0 = 1.f;
		t1 = 0.f;
		return false;
	}
	t0 = 0.f;
	t1 = 1000.f;
	return false;
}

inline Interval::Interval()
{
	t0 = 0;
	t1 = 1000.f;
}
inline Interval::~Interval()
{
}

inline bool RayAABB(Ray& ray, const AABB& aabb)
{
	Slab s1(glm::vec3(1.0f, 0.0f, 0.0f));
	s1.d0 = -aabb.vMin.x;
	s1.d1 = aabb.vMax.x;

	Slab s2(glm::vec3(0.0f, 1.0f, 0.0f));
	s2.d0 = -aabb.vMin.y;
	s2.d1 = aabb.vMax.y;

	Slab s3(glm::vec3(0.0f, 0.0f, 1.0f));
	s3.d0 = -aabb.vMin.z;
	s3.d1 = aabb.vMax.z;

	Interval i1, i2, i3;

	if (i1.Intersect(&ray, s1) &&
		i2.Intersect(&ray, s2) &&
		i3.Intersect(&ray, s3))
	{
		float minT = std::max(std::max(i1.t0, i2.t0), std::max(i2.t0, i3.t0));
		float maxT = std::min(std::min(i1.t1, i2.t1), std::min(i2.t1, i3.t1));

		if (minT > maxT)
			return false;

		if (minT > maxT)
			std::swap(minT, maxT);

		if (minT > 0.f)
		{
			return true;
		}
		if (maxT > 0.f)
		{
			return true;
		}
	}
	return false;
}