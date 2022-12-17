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
	diffTimeAnimCount = 1;
	//model = new AnimationModel(objShader, objPath, interpolationShader, reusableMeshData);
	//model = model_;

	//frameBuffer = new FrameBuffer(windowW, windowH);
	
	//obj = new SingleAnimationObject(model, objPos, glm::vec3(0.f, -5.f, 0.f), glm::vec3(30.f, 30.f, 30.f),
	//	false, 0.f);

	obj = mObj_;

	const int animationsCount = static_cast<int>(obj->animationModels.size());

	std::vector<FrameBuffer*> fbs;

	for(int i = 0; i < diffTimeAnimCount; ++i)
	{
		for (int j = 0; j < animationsCount; ++j)
		{
			fbs.push_back(new FrameBuffer(windowW, windowH));
		}
		frameBuffers.push_back(fbs);
		fbs.clear();
	}
	

	cam = cam_;
}

BillboardAnimatingDatas::~BillboardAnimatingDatas()
{
	//delete model;
	const int animationsCount = static_cast<int>(obj->animationModels.size());


	for (int i = 0; i < diffTimeAnimCount; ++i)
	{
		for(int j = 0; j < animationsCount; ++j)
		{
			delete frameBuffers[i][j];
		}
	}
	delete obj;

}
