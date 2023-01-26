/*
 *	File for GAM400 - OpenGL MAs project.
 *	Term			- 2022F
 *	Author			- Ryan Kim
 *	Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#pragma once
#include <vector>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class Camera;
struct SphereBV;
struct Frustum;
class FrameBuffer;
class Buffer;
class Shader;

class BillBoardObject
{
public:
	BillBoardObject(Shader* shader_, const glm::vec3& pos_, 
		const std::vector<FrameBuffer*>& fb_);
	~BillBoardObject();
	void Render(const glm::mat4& projMat, 
		const glm::mat4& viewMatWithoutRot, 
		const glm::vec4& pos);
	void SetFrameBufferIndex(int index);

	
	FrameBuffer* usingFrameBuffer = nullptr;
	std::vector<FrameBuffer*> fbs;
private:
	unsigned vao{};
	Shader* shader;
};
