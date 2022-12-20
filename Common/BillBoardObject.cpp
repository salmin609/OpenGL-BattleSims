/*
 *	File for GAM400 - OpenGL MAs project.
 *	Term			- 2022F
 *	Author			- Ryan Kim
 *	Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#include "BillBoardObject.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "FrameBuffer.h"
#include "FrustumCulling.h"
#include "Shader.h"
#include "Texture.h"


BillBoardObject::BillBoardObject(Shader* shader_, const glm::vec3& pos_, FrameBuffer* fb_)
{
	pos = pos_;
	shader = shader_;
	fb = fb_;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(0);

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

		fb->texture->Bind(0);

		const glm::mat4 modelMat = glm::translate(glm::mat4(1.f), pos);

		shader->SendUniformMatGLM("projMat", projMat);
		shader->SendUniformMatGLM("viewMat", viewMat);
		shader->SendUniformMatGLM("modelMat", modelMat);

		glDrawArrays(GL_POINTS, 0, 1);

		glBindVertexArray(0);
	}
}
