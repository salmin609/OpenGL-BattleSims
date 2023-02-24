#include "Herd.h"

#include <glm/gtc/quaternion.hpp>

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
	float yPos = 3.f;

	glm::vec4 point0 = posDatas[startIndex];
	glm::vec4 point1 = posDatas[startIndex + (herdWidth - 1)];
	glm::vec4 point2 = posDatas[startIndex + (count - (herdWidth - 1))];
	glm::vec4 point3 = posDatas[startIndex + (count - 1)];

	point0.y = yPos;
	point1.y = yPos;
	point2.y = yPos;
	point3.y = yPos;

	glm::vec4 midPoint;
	midPoint.x = (point0.x + point1.x) / 2.f;
	midPoint.y = yPos;
	midPoint.z = (point0.z + point2.z) / 2.f;

	glm::vec4 midToPoint0 = glm::normalize(point0 - midPoint);
	glm::vec4 midToPoint1 = glm::normalize(point1 - midPoint);
	glm::vec4 midToPoint2 = glm::normalize(point2 - midPoint);
	glm::vec4 midToPoint3 = glm::normalize(point3 - midPoint);

	float str = 12.f;

	glm::vec4 newPoint0 = point0 + midToPoint0 * str;
	glm::vec4 newPoint1 = point1 + midToPoint1 * str;
	glm::vec4 newPoint2 = point2 + midToPoint2 * str;
	glm::vec4 newPoint3 = point3 + midToPoint3 * str;


	boundingBoxes.emplace_back(newPoint0);
	boundingBoxes.emplace_back(newPoint1);
	boundingBoxes.emplace_back(newPoint2);
	boundingBoxes.emplace_back(newPoint3);
}
