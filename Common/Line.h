#pragma once
#include <vector>
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
class Shader;
class Buffer;

class Line
{
public:
	Line(Shader* shader);
	~Line();
	void Draw(const glm::mat4& projViewMat);

private:
	std::vector<glm::vec3> points;
	unsigned vao;
	Buffer* linePosBuffer;
	Shader* lineShader;

};
