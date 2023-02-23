/*
 *	File for GAM400 - OpenGL MAs project.
 *	Term			- 2022F
 *	Author			- Ryan Kim
 *	Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#pragma once
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"


class Shader;
class Buffer;

class Cube
{
public:
	Cube(Shader* shader_);
	~Cube();
	void Draw(const glm::mat4& projViewMat) const;
	glm::mat4 GetModelMatrix() const;
	glm::vec3 scale, rot, pos;
	glm::vec4 color;
private:
    unsigned vao;
    Buffer* positionBuffer;
	Shader* shader;
};
