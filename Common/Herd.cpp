#include "Herd.h"

#include "BillBoardObject.h"

Herd::Herd(int num_, glm::vec4 herdDirection_)
{
	count = num_;
	herdDirection = herdDirection_;
}

Herd::~Herd()
{
	//delete posBuffer;

	for (const auto& bo : bos)
		delete bo;
}

void Herd::Render(const glm::mat4& projMat, const glm::mat4& viewMat,
	glm::vec4* posDatas, int startIndex)
{
	int index = startIndex;
	for(const auto& bo : bos)
		bo->Render(projMat, viewMat, posDatas[index++]);
}
