#include "Line.h"

#include <GL/glew.h>

#include "Buffer.hpp"
#include "Shader.h"

Line::Line(Shader* shader)
{
	lineShader = shader;

	const float coord = 10000.f;
	const float largestZ = coord;
	const float smallestZ = -coord;

	const float largestX = coord;
	const float smallestX = -coord;

	const float offset = 10.f;


	const int nums = static_cast<int>((largestX - smallestX) / offset);
	const float yPos = 0.f;


	//horizon
	for(int i = 0; i < nums; ++i)
	{
		float xPos = smallestX + offset * static_cast<float>(i);
		float zPos = largestZ;

		glm::vec3 pos{ xPos, yPos, zPos };
		points.push_back(pos);

		zPos = smallestZ;
		pos = glm::vec3{ xPos, yPos, zPos };
		points.push_back(pos);
	}

	for (int i = 0; i < nums; ++i)
	{
		float zPos = smallestZ + offset * static_cast<float>(i);
		float xPos = largestX;

		glm::vec3 pos{ xPos, yPos, zPos };
		points.push_back(pos);

		xPos = smallestX;
		pos = glm::vec3{ xPos, yPos, zPos };
		points.push_back(pos);
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	linePosBuffer = new Buffer(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), GL_STATIC_DRAW, points.data());
	linePosBuffer->Bind();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), static_cast<GLvoid*>(0));

	glBindVertexArray(0);

}

Line::~Line()
{
	delete linePosBuffer;
	glDeleteVertexArrays(1, &vao);
}

void Line::Draw(const glm::mat4& projViewMat)
{
	lineShader->Use();
	glBindVertexArray(vao);
	linePosBuffer->Bind();

	lineShader->SendUniformMatGLM("gWVP", projViewMat);

	//const size_t size = line->Coords().size();

	glDrawArrays(GL_LINES, 0, static_cast<int>(points.size()));

	glBindVertexArray(0);
}
