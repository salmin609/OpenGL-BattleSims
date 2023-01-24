#pragma once
#include <glm/vec4.hpp>

class Buffer;

class Herd
{
public:
	Herd();
	~Herd();
	void SetPosBuffer(Buffer* posBuffer_);
	void SetBoDirAndOffset(glm::vec4 data_);
	glm::vec4& GetHerdBoDirAndOffset();

private:
	glm::vec4 herdBoDirAndOffset{};
	Buffer* posBuffer;

};
