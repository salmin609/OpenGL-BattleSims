/*
 * Author		: Ryan Kim.
 * Date			: 2022-10-07
 * Description	: class for mesh information.
 * Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#pragma once
#include <vector>
#include "glm/vec3.hpp"


struct aiScene;
class Buffer;

class MeshDatas
{
public:
	MeshDatas();
	~MeshDatas();
	void ReserveSpace(int numVertices, int numIndices);
	void PopulateBuffer();
	void FeedLayout();
	void Bind();
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texCoords;
	std::vector<unsigned> indices;

	Buffer* posBuffer;
	Buffer* texBuffer;
	Buffer* normalBuffer;
	Buffer* indexBuffer;
private:
	


};
