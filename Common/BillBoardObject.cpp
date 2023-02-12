/*
 *	File for GAM400 - OpenGL MAs project.
 *	Term			- 2022F
 *	Author			- Ryan Kim
 *	Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#include "BillBoardObject.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "AnimationModel.h"
#include "BillboardManager.h"
#include "FrameBuffer.h"
#include "Shader.h"
#include "Texture.h"

BillBoardObject::BillBoardObject(Shader* shader_,
	std::vector<std::vector<FrameBuffer*>>* fb_,
	AnimationState* animState_)
{
	shader = shader_;
	animFrames = fb_;
	currentAngleSlot = 0;
	animState = animState_;
	currentState = State::Idle;
	fbs = &(*animFrames)[static_cast<int>(currentState)];

	glGenVertexArrays(1, &vao);
	glBindVertexArray(0);

	SetAnimation(static_cast<int>(State::Idle));
}

BillBoardObject::~BillBoardObject()
{
	glDeleteVertexArrays(1, &vao);
}


void BillBoardObject::Render(const glm::mat4& projMat, const glm::mat4& viewMat, const glm::vec4& pos)
{
	if (usingFrameBuffer != nullptr)
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
	if (fbs != nullptr)
	{
		usingFrameBuffer = (*fbs)[index];
		usingFrameBuffer->isOnUsage = true;
		currentAngleSlot = index;
	}
}

bool BillBoardObject::SetAnimation(int index)
{
	//Todo: Should read animState first, tracking that anim available.

	if (usingFrameBuffer != nullptr)
	{
		const State newState = static_cast<State>(index);

		if (newState != currentState)
		{
			AnimationModel* model = animState->RequestAnimation(index);

			if (model != nullptr)
			{
				model->boUsingThisAnimation.push_back(this);
				fbs = &model->fbs;
				currentState = newState;
				return true;
			}
		}
	}
	return false;
}
