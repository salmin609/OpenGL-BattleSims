/*
 * Author		: Ryan Kim.
 * Date			: 2022-10-07
 * Description	: Class for basic Quaternion, provide Slerp, in matrix transformation.
 * Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#include "Quaternion.h"
#include <cmath>

Quaternion::Quaternion(float x_, float y_, float z_, float w_)
{

}

Quaternion::Quaternion(const aiQuaternion& val)
{
	*this = val;
}

void Quaternion::Interpolate(const Quaternion& pStart, const Quaternion& pEnd, float factor)
{
	float cosom = pStart.x * pEnd.x + pStart.y * pEnd.y + pStart.z * pEnd.z + pStart.w * pEnd.w;

	Quaternion end = pEnd;

	if(cosom < 0.0f)
	{
		cosom = -cosom;
		end.x = -end.x;
		end.y = -end.y;
		end.z = -end.z;
		end.w = -end.w;
	}

	float sclp, sclq;

	if(1.f - cosom > 0.0001f)
	{
		float omega, sinom;
		omega = acos(cosom); // extract theta from dot product's cos theta
		sinom = sin(omega);
		sclp = sin(1.0f - factor) * omega / sinom;
		sclq = sin(factor * omega) / sinom;
	}
	else
	{
		sclp = 1.f - factor;
		sclq = factor;
	}

	x = sclp * pStart.x + sclq * end.x;
	y = sclp * pStart.y + sclq * end.y;
	z = sclp * pStart.z + sclq * end.z;
	w = sclp * pStart.w + sclq * end.w;
}

Quaternion& Quaternion::operator=(const aiQuaternion& val)
{
	x = val.x;
	y = val.y;
	z = val.z;
	w = val.w;

	return *this;
}

void Quaternion::Normalize()
{
	const float mag = sqrt(x * x + y * y + z * z + w * w);
	if (mag > 0.f)
	{
		const float invMag = 1.0f / mag;
		x *= invMag;
		y *= invMag;
		z *= invMag;
		w *= invMag;
	}
}

glm::mat4 Quaternion::GetMatrix()
{
	glm::mat4 resMatrix;
	resMatrix[0][0] = (1.0f) - (2.0f) * (y * y + z * z);
	resMatrix[1][0] = (2.0f) * (x * y - z * w);
	resMatrix[2][0] = (2.0f) * (x * z + y * w);
	resMatrix[0][1] = (2.0f) * (x * y + z * w);
	resMatrix[1][1] = (1.0f) - (2.0f) * (x * x + z * z);
	resMatrix[2][1] = (2.0f) * (y * z - x * w);
	resMatrix[0][2] = (2.0f) * (x * z - y * w);
	resMatrix[1][2] = (2.0f) * (y * z + x * w);
	resMatrix[2][2] = (1.0f) - (2.0f) * (x * x + y * y);

	return resMatrix;
}

Quaternion::~Quaternion()
{

}
