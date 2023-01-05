/*
 *	File for GAM400 - OpenGL MAs project.
 *	Term			- 2022F
 *	Author			- Ryan Kim
 *	Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#include "BillBoardObject.h"

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "BillboardManager.h"
#include "Camera.hpp"
#include "FrameBuffer.h"
#include "FrustumCulling.h"
#include "Shader.h"
#include "Texture.h"

float pi = 3.14159;

float Convert(float radian)
{
	return(radian * (180 / pi));
}

BillBoardObject::BillBoardObject(Shader* shader_, const glm::vec3& pos_, 
	std::vector<FrameBuffer*> fb_, Camera* cam_)
{
	pos = pos_;
	shader = shader_;
	//fb = fb_;
	fbs = fb_;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(0);
	cam = cam_;
	direction = glm::vec3(0.f, 0.f, -1.f);

	spv = new SphereBV(pos, 0.1f);
}

BillBoardObject::~BillBoardObject()
{
	glDeleteVertexArrays(1, &vao);
	delete spv;
}

void BillBoardObject::Render(const glm::mat4& projMat, const glm::mat4& viewMat, Frustum* frustum) const
{
	if(spv->isOnFrustum(*frustum))
	{
		shader->Use();
		glBindVertexArray(vao);

		SetFrameBufferAngleTarget();

		const glm::mat4 modelMat = glm::translate(glm::mat4(1.f), pos);

		shader->SendUniformMatGLM("projMat", projMat);
		shader->SendUniformMatGLM("viewMat", viewMat);
		shader->SendUniformMatGLM("modelMat", modelMat);

		glDrawArrays(GL_POINTS, 0, 1);

		glBindVertexArray(0);
	}
}

void BillBoardObject::SetFrameBufferAngleTarget() const
{
	const glm::vec3 camToPos = pos - cam->Position;
	const float cosTheta = (glm::dot(camToPos, direction)) / (glm::length(camToPos) * glm::length(direction));
	const float angle = acos(cosTheta);
	const float angleInDegree = Convert(angle);

	//TODO : Need to figure more flexible angle view
	if (pos.x > cam->Position.x)
	{
		if (angleInDegree >= 0 && angleInDegree <= 45)
			fbs[static_cast<int>(CamVectorOrder::RightFront)]->texture->Bind(0);
		else if (angleInDegree >= 135 && angleInDegree <= 180)
			fbs[static_cast<int>(CamVectorOrder::LeftFront)]->texture->Bind(0);
		else
			fbs[static_cast<int>(CamVectorOrder::Front)]->texture->Bind(0);
	}
	else
	{
		if (angleInDegree >= 0 && angleInDegree <= 45)
			fbs[static_cast<int>(CamVectorOrder::RightBack)]->texture->Bind(0);
		else if (angleInDegree >= 135 && angleInDegree <= 180)
			fbs[static_cast<int>(CamVectorOrder::LeftBack)]->texture->Bind(0);
		else
			fbs[static_cast<int>(CamVectorOrder::Back)]->texture->Bind(0);
	}
}
