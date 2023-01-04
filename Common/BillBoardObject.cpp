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


		//TODO: need to figure out angle from current cam and use various cam angles

		const glm::vec3 camToPos = pos - cam->Position;
		const glm::vec3 camLook = cam->Front;

		//float cosTheta = (glm::dot(camToPos, camLook)) / (glm::length(camToPos) * glm::length(camLook));
		//float sinTheta = glm::length(glm::cross(camToPos, camLook)) / (glm::length(camToPos) * glm::length(camLook));
		////float angle = acos(cosTheta);
		//float angle = asin(sinTheta);

		//float angleInDegree = Convert(angle);
		//std::cout << "angle : " << angleInDegree << std::endl;

		float dot = glm::dot(camToPos, camLook);

		if (dot > 0)
			std::cout << "<90deg" << std::endl;
		else if (dot < 0)
			std::cout << ">90deg" << std::endl;
		else
			std::cout << "90deg" << std::endl;

		fbs[0]->texture->Bind(0);



		const glm::mat4 modelMat = glm::translate(glm::mat4(1.f), pos);

		shader->SendUniformMatGLM("projMat", projMat);
		shader->SendUniformMatGLM("viewMat", viewMat);
		shader->SendUniformMatGLM("modelMat", modelMat);

		glDrawArrays(GL_POINTS, 0, 1);

		glBindVertexArray(0);
	}
}
