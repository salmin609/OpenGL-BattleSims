#include "Herd.h"

#include "BillBoardObject.h"
#include "Buffer.hpp"

Herd::Herd()
{

}

Herd::~Herd()
{
	delete posBuffer;

	for (const auto& bo : bos)
		delete bo;
}

void Herd::SetPosBuffer(Buffer* posBuffer_)
{
	posBuffer = posBuffer_;
}

void Herd::SetBoDirAndOffset(glm::vec4 data_)
{
	herdBoDirAndOffset = data_;
}

glm::vec4& Herd::GetHerdBoDirAndOffset()
{
	return herdBoDirAndOffset;
}

void Herd::BindPosBuffer()
{
	posBuffer->BindStorage();
}

void Herd::Render(const glm::mat4& projMat, const glm::mat4& viewMat)
{
	for(const auto& bo : bos)
	{
		bo->Render(projMat, viewMat);
	}
}
