/*
 *	File for GAM400 - OpenGL MAs project.
 *	Term			- 2022F
 *	Author			- Ryan Kim
 *	Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#pragma once
#include <map>
#include <string>
#include "glm/vec3.hpp"

class MultipleAnimationObject;
class SingleAnimationObject;
class MeshDatas;
class Texture;
class Shader;
class Camera;
class Object;
class FrameBuffer;
class AnimationModel;

class BillboardAnimatingDatas
{
public:
	BillboardAnimatingDatas(
		std::string objPath, Shader* objShader, Shader* interpolationShader,
		glm::vec3 objPos, Camera* cam_, int windowW, int windowH,
		MeshDatas* reusableMeshData);
	~BillboardAnimatingDatas();

	AnimationModel* model;
	FrameBuffer* frameBuffer;
	//Object* obj;
	//SingleAnimationObject* obj;
	MultipleAnimationObject* obj;

	Camera* cam;
	bool inUse = false;
private:


};
