#include "Herd.h"

#include "BillBoardObject.h"
#include "Buffer.hpp"

Herd::Herd(int num_)
{
	posBuffer = nullptr;
	count = num_;

	positions = new glm::vec4[count];
}

Herd::~Herd()
{
	delete posBuffer;
	delete[] positions;

	for (const auto& bo : bos)
		delete bo;
}

void Herd::Render(const glm::mat4& projMat, const glm::mat4& viewMat)
{
	posBuffer->GetData(positions);

	int index = 0;
	for(const auto& bo : bos)
	{
		glm::vec4 ck = positions[index++];
		bo->Render(projMat, viewMat, ck);
	}
}
