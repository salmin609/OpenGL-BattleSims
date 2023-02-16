#pragma once
#include <vector>
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
class Shader;
class Buffer;

class Line
{
public:
	Line(Shader* shader, std::vector<glm::vec3> points_
		, glm::vec4 color_, bool isItFloor = false);
	~Line();
	void Draw(const glm::mat4& projViewMat);
	void UpdatePosBuffer(std::vector<glm::vec3> points_);
	glm::vec4 color;
private:
	std::vector<glm::vec3> points;
	unsigned vao;
	Buffer* linePosBuffer;
	Shader* lineShader;
};
