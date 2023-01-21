/*
 *	File for GAM400 - OpenGL MAs project.
 *	Term			- 2022F
 *	Author			- Ryan Kim
 *	Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#pragma once
#include <vector>


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
	BillboardAnimatingDatas(int windowW, int windowH,
	MultipleAnimationObject* mObj_);
	~BillboardAnimatingDatas();

	std::vector<std::vector<std::vector<FrameBuffer*>>> frameBuffers;
	MultipleAnimationObject* obj;
	int diffTimeAnimCount;
private:


};
