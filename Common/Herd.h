#pragma once
#include <vector>
#include "glm/mat4x4.hpp"
#include <glm/vec4.hpp>

class BillBoardObject;
class Buffer;

class Herd
{
public:
	Herd(int num_);
	~Herd();
	void Render(const glm::mat4& projMat, const glm::mat4& viewMat);

	Buffer* posBuffer;
	//xyz = boDirection, w = count
	//glm::vec4 herdBoDirAndOffset{};
	int herdOffset{};
	std::vector<BillBoardObject*> bos;
	int count;
	glm::vec4* positions;
private:

};
