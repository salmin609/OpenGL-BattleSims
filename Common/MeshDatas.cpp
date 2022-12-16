/*
 * Author		: Ryan Kim.
 * Date			: 2022-10-07
 * Description	: class for mesh information.
 * Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#include "MeshDatas.h"

#include <GL/glew.h>
#include "glm/vec2.hpp"

#include "Buffer.hpp"

MeshDatas::MeshDatas()
{

}

MeshDatas::~MeshDatas()
{
	delete posBuffer;
	delete texBuffer;
	delete normalBuffer;
	delete indexBuffer;
}

void MeshDatas::ReserveSpace(int numVertices, int numIndices)
{
	positions.reserve(numVertices);
	normals.reserve(numVertices);
	texCoords.reserve(numVertices);
	indices.reserve(numIndices);
}

void MeshDatas::PopulateBuffer()
{
	posBuffer = new Buffer(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), GL_STATIC_DRAW,
		positions.data());

	texBuffer = new Buffer(GL_ARRAY_BUFFER, sizeof(glm::vec2) * texCoords.size(), GL_STATIC_DRAW,
		texCoords.data());

	normalBuffer = new Buffer(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), GL_STATIC_DRAW,
		normals.data());

	FeedLayout();

	indexBuffer = new Buffer(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * indices.size(),
		GL_STATIC_DRAW, indices.data());
}

void MeshDatas::FeedLayout()
{
	posBuffer->Bind();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);

	texBuffer->Bind();
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (GLvoid*)0);

	normalBuffer->Bind();
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);

	indexBuffer = new Buffer(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * indices.size(),
		GL_STATIC_DRAW, indices.data());
}

void MeshDatas::Bind()
{
	posBuffer->Bind();
	texBuffer->Bind();
	normalBuffer->Bind();
	indexBuffer->Bind();
}
