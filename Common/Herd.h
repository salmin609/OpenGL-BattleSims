#pragma once
#include <vector>
#include "glm/mat4x4.hpp"
#include <glm/vec4.hpp>

class BillBoardObject;
class Buffer;

class Herd
{
public:
	Herd(int num_, glm::vec4 herdDirection_, int side_);
	~Herd();
	void Render(const glm::mat4& projMat, const glm::mat4& viewMat,
		glm::vec4* posDatas, int startIndex);

	int herdOffset{};
	std::vector<BillBoardObject*> bos;
	int count;
	glm::vec4 herdDirection;
	int side;
private:

};
