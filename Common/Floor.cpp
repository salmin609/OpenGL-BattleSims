/*
 *	File for GAM400 - OpenGL MAs project.
 *	Term			- 2022F
 *	Author			- Ryan Kim
 *	Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#include "Floor.hpp"

#include <GL/glew.h>

#include "Buffer.hpp"
#include <vector>
#include <glm/gtc/matrix_transform.hpp>


#include "Shader.h"

Floor::Floor(Shader* shader_)
{
	shader = shader_;
	std::vector<float>vertices = {
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	positionBuffer = new Buffer(GL_ARRAY_BUFFER,
		static_cast<int>(vertices.size()) * sizeof(float),
		GL_STATIC_DRAW,
		vertices.data());
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);

	glBindVertexArray(0);
}

Floor::~Floor()
{
	delete positionBuffer;
	glDeleteVertexArrays(1, &vao);
}

void Floor::Draw(const glm::mat4& projViewMat) const
{
	shader->Use();
	glBindVertexArray(vao);

	glm::mat4 projViewModelMat = projViewMat * GetModelMatrix();
	shader->SendUniformMatGLM("projViewModelMat", &projViewModelMat);

	glDrawArrays(GL_TRIANGLES, 0, 36);


	glBindVertexArray(0);
}

glm::mat4 Floor::GetModelMatrix() const
{
	glm::mat4 modelMat = glm::mat4(1.f);

	modelMat = glm::translate(modelMat, pos);
	modelMat = glm::scale(modelMat, scale);
	//modelMat = glm::rotate(modelMat, 1.5708f, rot);

	return modelMat;

}
