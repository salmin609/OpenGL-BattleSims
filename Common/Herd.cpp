#include "Herd.h"

#include "BillBoardObject.h"
#include "Line.h"

Herd::Herd(int num_, glm::vec4 herdDirection_, int side_, Shader* lineShader_,int herdWidth_, float speed_)
{
	count = num_;
	herdDirection = herdDirection_;
	side = side_;
	selected = false;
	herdWidth = herdWidth_;
	speed = speed_;

	std::vector<glm::vec3> temp;
	for(int i = 0 ; i < 8; ++i)
	{
		temp.emplace_back();
	}

	outline = new Line(lineShader_, temp, glm::vec4(1.f, 0.f, 0.f, 1.f));
}

Herd::~Herd()
{
	//delete posBuffer;

	for (const auto& bo : bos)
		delete bo;

	delete outline;
}

void Herd::Render(const glm::mat4& projMat, const glm::mat4& viewMat,
	glm::vec4* posDatas, int startIndex)
{
	int index = startIndex;
	for(const auto& bo : bos)
		bo->Render(projMat, viewMat, posDatas[index++]);
}

void Herd::DrawLine(const glm::mat4& projMat, const glm::mat4& viewMat)
{
	const glm::mat4 projViewMat = projMat * viewMat;

	std::vector<glm::vec3> outPos;

	outPos.emplace_back(boundingBoxes[0]);
	outPos.emplace_back(boundingBoxes[1]);

	outPos.emplace_back(boundingBoxes[1]);
	outPos.emplace_back(boundingBoxes[3]);

	outPos.emplace_back(boundingBoxes[0]);
	outPos.emplace_back(boundingBoxes[2]);

	outPos.emplace_back(boundingBoxes[2]);
	outPos.emplace_back(boundingBoxes[3]);

	outline->UpdatePosBuffer(outPos);
	
	outline->Draw(projViewMat);
}

void Herd::UpdateBoundingBox(glm::vec4* posDatas, int startIndex)
{
	boundingBoxes.clear();
	boundingBoxes.emplace_back(posDatas[startIndex]);
	boundingBoxes.emplace_back(posDatas[startIndex + (herdWidth - 1)]);
	boundingBoxes.emplace_back(posDatas[startIndex + (count - (herdWidth - 1))]);
	boundingBoxes.emplace_back(posDatas[startIndex + (count - 1)]);
}
