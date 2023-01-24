#include "Herd.h"

#include "Buffer.hpp"

Herd::Herd()
{

}

Herd::~Herd()
{
	delete posBuffer;
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
