/*
 *	File for GAM400 - OpenGL MAs project.
 *	Term			- 2022F
 *	Author			- Ryan Kim
 *	Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#include "BillBoardObject.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "BillboardManager.h"
#include "FrameBuffer.h"
#include "Shader.h"
#include "Texture.h"

BillBoardObject::BillBoardObject(Shader* shader_,
	std::vector<std::vector<FrameBuffer*>>* fb_)
{
	shader = shader_;
	animFrames = fb_;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(0);

	SetAnimation(1);
}

BillBoardObject::~BillBoardObject()
{
	glDeleteVertexArrays(1, &vao);
}


void BillBoardObject::Render(const glm::mat4& projMat, const glm::mat4& viewMat,const glm::vec4& pos)
{
	if(usingFrameBuffer != nullptr)
	{
		shader->Use();
		glBindVertexArray(vao);

		usingFrameBuffer->texture->Bind(0);

		shader->SendUniformVec4("boPos", pos);
		shader->SendUniformMatGLM("projMat", projMat);
		shader->SendUniformMatGLM("viewMat", viewMat);

		glDrawArrays(GL_POINTS, 0, 1);

		glBindVertexArray(0);

		usingFrameBuffer = nullptr;
	}
}

void BillBoardObject::ChangeFrameBufferAngle(int index)
{
	usingFrameBuffer = (*fbs)[index];
	usingFrameBuffer->isOnUsage = true;
}

void BillBoardObject::SetAnimation(int index)
{
	fbs = &(*animFrames)[index];
}
