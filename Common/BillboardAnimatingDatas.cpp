/*
 *	File for GAM400 - OpenGL MAs project.
 *	Term			- 2022F
 *	Author			- Ryan Kim
 *	Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#include "BillboardAnimatingDatas.h"
#include "AnimationModel.h"
#include "AnimationState.h"
#include "BillboardManager.h"
#include "FrameBuffer.h"
#include "MultipleAnimationObject.h"

BillboardAnimatingDatas::BillboardAnimatingDatas(int windowW, int windowH
	, MultipleAnimationObject* mObj_)
{

	//In order to get various time difference, change this variable value
	diffTimeAnimCount = 1;

	obj = mObj_;
	//const int animationsCount = static_cast<int>(obj->animationModels.size());

	////1 : different times
	////2 : different animations
	////3 : different angles

	//for(int i = 0; i < diffTimeAnimCount; ++i)
	//{
	//	std::vector<std::vector<FrameBuffer*>> fbs1;
	//	for(int j = 0; j < animationsCount; ++j)
	//	{
	//		std::vector<FrameBuffer*> fbs2;
	//		for (int k = 0; k < static_cast<int>(CamVectorOrder::End); ++k)
	//		{
	//			fbs2.push_back(new FrameBuffer(windowW, windowH));
	//		}
	//		fbs1.push_back(fbs2);
	//	}
	//	frameBuffers.push_back(fbs1);
	//}

	for(int i = 0; i < diffTimeAnimCount; ++i)
	{
		AnimationState* animState = obj->animState;
		std::vector<std::vector<FrameBuffer*>> fbs1;

		const int idleAnimationsCount = static_cast<int>(animState->idleAnimations.size());
		const int attackAnimationsCount = static_cast<int>(animState->attackAnimations.size());
		const int painAnimationsCount = static_cast<int>(animState->painAnimations.size());
		const int runAnimationsCount = 1;
		const int deathAnimationsCount = 1;

		for(int j = 0; j < idleAnimationsCount; ++j)
		{
			std::vector<FrameBuffer*> fbs2;

			for(int k = 0; k < static_cast<int>(CamVectorOrder::End); ++k)
			{
				fbs2.push_back(new FrameBuffer(windowW, windowH));
			}

			fbs1.push_back(fbs2);
		}

		for (int j = 0; j < attackAnimationsCount; ++j)
		{
			std::vector<FrameBuffer*> fbs2;

			for (int k = 0; k < static_cast<int>(CamVectorOrder::End); ++k)
			{
				fbs2.push_back(new FrameBuffer(windowW, windowH));
			}

			fbs1.push_back(fbs2);
		}

		for (int j = 0; j < painAnimationsCount; ++j)
		{
			std::vector<FrameBuffer*> fbs2;

			for (int k = 0; k < static_cast<int>(CamVectorOrder::End); ++k)
			{
				fbs2.push_back(new FrameBuffer(windowW, windowH));
			}

			fbs1.push_back(fbs2);
		}

		for (int j = 0; j < runAnimationsCount; ++j)
		{
			std::vector<FrameBuffer*> fbs2;

			for (int k = 0; k < static_cast<int>(CamVectorOrder::End); ++k)
			{
				fbs2.push_back(new FrameBuffer(windowW, windowH));
			}

			fbs1.push_back(fbs2);
		}
		
		for (int j = 0; j < deathAnimationsCount; ++j)
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
	//AnimationState* state = obj->animState;
	//const int frameBufferCount = (state->painAnimations.size() + state->idleAnimations.size() +
	//	state->attackAnimations.size() + 2);

	//const int animationsCount = static_cast<int>(obj->animationModels.size());

	//for (int i = 0; i < diffTimeAnimCount; ++i)
	//{
	//	for(int j = 0; j < animationsCount; ++j)
	//	{
	//		for (int k = 0; k < static_cast<int>(CamVectorOrder::End); ++k)
	//		{
	//			delete frameBuffers[i][j][k];
	//		}
	//	}
	//}

	for(int i = 0; i < diffTimeAnimCount; ++i)
	{
		std::vector<std::vector<FrameBuffer*>> fbs = frameBuffers[i];

		const int fbsSize = static_cast<int>(fbs.size());

		for(int j = 0; j < fbsSize; ++j)
		{
			std::vector<FrameBuffer*> fbs2 = fbs[j];

			const int fbs2Size = static_cast<int>(fbs2.size());

			for(int k = 0; k < fbs2Size; ++k)
			{
				delete frameBuffers[i][j][k];
			}

		}

	}



	delete obj;

}
