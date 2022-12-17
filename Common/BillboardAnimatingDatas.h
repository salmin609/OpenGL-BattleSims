/*
 *	File for GAM400 - OpenGL MAs project.
 *	Term			- 2022F
 *	Author			- Ryan Kim
 *	Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#pragma once


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
	BillboardAnimatingDatas(Camera* cam_, int windowW, int windowH,
	MultipleAnimationObject* mObj_);
	~BillboardAnimatingDatas();

	FrameBuffer* frameBuffer;
	MultipleAnimationObject* obj;

	Camera* cam;
	bool inUse = false;
private:


};
