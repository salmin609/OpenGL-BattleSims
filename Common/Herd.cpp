#include "Herd.h"

#include "BillBoardObject.h"
#include "Buffer.hpp"

Herd::Herd()
{
	posBuffer = nullptr;
}

Herd::~Herd()
{
	delete posBuffer;

	for (const auto& bo : bos)
		delete bo;
}

void Herd::Render(const glm::mat4& projMat, const glm::mat4& viewMat)
{
	for(const auto& bo : bos)
	{
		bo->Render(projMat, viewMat);
	}
}
