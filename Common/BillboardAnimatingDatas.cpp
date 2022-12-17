/*
 *	File for GAM400 - OpenGL MAs project.
 *	Term			- 2022F
 *	Author			- Ryan Kim
 *	Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#include "BillboardAnimatingDatas.h"

#include "AnimationModel.h"
#include "Camera.hpp"
#include "FrameBuffer.h"
#include "MultipleAnimationObject.h"
#include "Object.h"
#include "SingleAnimationObject.h"

BillboardAnimatingDatas::BillboardAnimatingDatas(
	Camera* cam_, int windowW, int windowH
	, MultipleAnimationObject* mObj_)
{
	//model = new AnimationModel(objShader, objPath, interpolationShader, reusableMeshData);
	//model = model_;

	frameBuffer = new FrameBuffer(windowW, windowH);
	//obj = new SingleAnimationObject(model, objPos, glm::vec3(0.f, -5.f, 0.f), glm::vec3(30.f, 30.f, 30.f),
	//	false, 0.f);

	obj = mObj_;

	cam = cam_;
}

BillboardAnimatingDatas::~BillboardAnimatingDatas()
{
	//delete model;
	delete frameBuffer;
	delete obj;

}
