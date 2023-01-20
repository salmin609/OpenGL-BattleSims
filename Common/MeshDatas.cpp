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
#include "BufferManager.h"

MeshDatas::MeshDatas()
{
	renderingDatas = new BufferManager();
}

MeshDatas::~MeshDatas()
{
	delete renderingDatas;
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
	renderingDatas->AddBuffer(new Buffer(GL_ARRAY_BUFFER, sizeof(glm::vec3) * static_cast<int>(positions.size()), GL_STATIC_DRAW,
		positions.data()));

	renderingDatas->AddBuffer(new Buffer(GL_ARRAY_BUFFER, sizeof(glm::vec2) * static_cast<int>(texCoords.size()), GL_STATIC_DRAW,
		texCoords.data()));

	renderingDatas->AddBuffer(new Buffer(GL_ARRAY_BUFFER, sizeof(glm::vec3) * static_cast<int>(normals.size()), GL_STATIC_DRAW,
		normals.data()));

	renderingDatas->AddBuffer(new Buffer(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * static_cast<int>(indices.size()),
		GL_STATIC_DRAW, indices.data()));

	FeedLayout();
}

void MeshDatas::FeedLayout()
{
	renderingDatas->BindBuffer(0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);

	renderingDatas->BindBuffer(1);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (GLvoid*)0);

	renderingDatas->BindBuffer(2);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
}

void MeshDatas::Bind()
{
	renderingDatas->BindBuffers();
}
