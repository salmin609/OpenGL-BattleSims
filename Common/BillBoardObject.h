/*
 *	File for GAM400 - OpenGL MAs project.
 *	Term			- 2022F
 *	Author			- Ryan Kim
 *	Copyright � 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

struct Frustum;
class FrameBuffer;
class Buffer;
class Shader;

class BillBoardObject
{
public:
	BillBoardObject(Shader* shader_, const glm::vec3& pos_, FrameBuffer* fb_);
	~BillBoardObject();
	void Render(glm::mat4 projMat, glm::mat4 viewMat, Frustum* frustum);

	glm::vec3 pos;
	int animationIndex = 0;
private:
	unsigned vao;
	Shader* shader;
	FrameBuffer* fb;
};