#pragma once
#include <vector>
#include "glm/mat4x4.hpp"
#include <glm/vec4.hpp>

class Line;
class Shader;
class BillBoardObject;
class Buffer;

class Herd
{
public:
	Herd(int num_, glm::vec4 herdDirection_, int side_, Shader* lineShader_
		, int herdWidth_, float speed_);
	~Herd();
	void Render(const glm::mat4& projMat, const glm::mat4& viewMat,
		glm::vec4* posDatas, int startIndex);
	void DrawLine(const glm::mat4& projMat, const glm::mat4& viewMat,
		glm::vec4* posDatas, int startIndex);

	int herdOffset{};
	std::vector<BillBoardObject*> bos;
	int count;
	glm::vec4 herdDirection;
	float speed;
	int side;
	Line* outline;
	bool selected;
	int herdWidth;
private:

};
