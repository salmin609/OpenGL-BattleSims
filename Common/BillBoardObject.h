/*
 *	File for GAM400 - OpenGL MAs project.
 *	Term			- 2022F
 *	Author			- Ryan Kim
 *	Copyright � 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
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
	BillBoardObject(Shader* shader_, const glm::vec3& pos_, std::vector<FrameBuffer*> fb_,
		Camera* cam_);
	~BillBoardObject();
	void Render(const glm::mat4& projMat, const glm::mat4& viewMatWithoutRot, Frustum* frustum) const;

	glm::vec3 pos;
	int animationIndex = 0;
private:
	unsigned vao{};
	Shader* shader;
	//FrameBuffer* fb;
	std::vector<FrameBuffer*> fbs;
	SphereBV* spv;
	Camera* cam;
};
