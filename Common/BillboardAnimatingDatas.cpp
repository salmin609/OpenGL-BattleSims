/*
 *	File for GAM400 - OpenGL MAs project.
 *	Term			- 2022F
 *	Author			- Ryan Kim
 *	Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#include "BillboardAnimatingDatas.h"

#include "AnimationModel.h"
#include "BillboardManager.h"
#include "Camera.hpp"
#include "FrameBuffer.h"
#include "MultipleAnimationObject.h"
#include "Object.h"

BillboardAnimatingDatas::BillboardAnimatingDatas(int windowW, int windowH
	, MultipleAnimationObject* mObj_)
{

	//In order to get various time difference, change this variable value
	diffTimeAnimCount = 1;

	obj = mObj_;
	const int animationsCount = static_cast<int>(obj->animationModels.size());

	//1 : different times
	//2 : different animations
	//3 : different angles

	for(int i = 0; i < diffTimeAnimCount; ++i)
	{
		std::vector<std::vector<FrameBuffer*>> fbs1;
		for(int j = 0; j < animationsCount; ++j)
		{
			std::vector<FrameBuffer*> fbs2;
			for (int k = 0; k < static_cast<int>(CamVectorOrder::End); ++k)
			{
				fbs2.push_back(new FrameBuffer(windowW, windowH));
			}
			fbs1.push_back(fbs2);
		}
		frameBuffers.push_back(fbs1);
	}
}

BillboardAnimatingDatas::~BillboardAnimatingDatas()
{
	//delete model;
	const int animationsCount = static_cast<int>(obj->animationModels.size());


	for (int i = 0; i < diffTimeAnimCount; ++i)
	{
		for(int j = 0; j < animationsCount; ++j)
		{
			for (int k = 0; k < static_cast<int>(CamVectorOrder::End); ++k)
			{
				delete frameBuffers[i][j][k];
			}
		}
	}
	delete obj;

}
