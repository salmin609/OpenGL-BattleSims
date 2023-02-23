/*
 *	File for GAM400 - OpenGL MAs project.
 *	Term			- 2022F
 *	Author			- Ryan Kim
 *	Copyright ?2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
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

	////1 : different times
	////2 : different animations
	////3 : different angles

	const int camNums = 3;
	

	for(int i = 0; i < diffTimeAnimCount; ++i)
	{
		AnimationState* animState = obj->animState;
		std::vector<std::vector<FrameBuffer*>> fbs1;

		const int idleAnimationsCount = static_cast<int>(animState->idleAnimations.size());
		const int attackAnimationsCount = static_cast<int>(animState->attackAnimations.size());
		const int painAnimationsCount = static_cast<int>(animState->painAnimations.size());
		const int runAnimationsCount = static_cast<int>(animState->runAnimations.size());
		const int deathAnimationsCount = static_cast<int>(animState->deathAnimations.size());

		for(int j = 0; j < idleAnimationsCount; ++j)
		{
			AnimationModel* idleAnimation = animState->idleAnimations[j];
			std::vector<FrameBuffer*> fbs2;

			for(int k = 0; k < static_cast<int>(CamVectorOrder::End); ++k)
			//for (int k = 0; k < camNums; ++k)
			{
				FrameBuffer* fb = new FrameBuffer(windowW, windowH);
				fbs2.push_back(fb);
			}
			idleAnimation->fbs = fbs2;

			fbs1.push_back(fbs2);
		}

		for (int j = 0; j < attackAnimationsCount; ++j)
		{
			AnimationModel* attackAnimation = animState->attackAnimations[j];
			std::vector<FrameBuffer*> fbs2;

			for (int k = 0; k < static_cast<int>(CamVectorOrder::End); ++k)
			//for (int k = 0; k < camNums; ++k)
			{
				FrameBuffer* fb = new FrameBuffer(windowW, windowH);
				fbs2.push_back(fb);
			}
			attackAnimation->fbs = fbs2;

			fbs1.push_back(fbs2);
		}

		for (int j = 0; j < painAnimationsCount; ++j)
		{
			AnimationModel* painAnimation = animState->painAnimations[j];
			std::vector<FrameBuffer*> fbs2;

			for (int k = 0; k < static_cast<int>(CamVectorOrder::End); ++k)
			//for (int k = 0; k < camNums; ++k)
			{
				FrameBuffer* fb = new FrameBuffer(windowW, windowH);
				fbs2.push_back(fb);
			}
			painAnimation->fbs = fbs2;
			fbs1.push_back(fbs2);
		}

		for (int j = 0; j < runAnimationsCount; ++j)
		{
			std::vector<FrameBuffer*> fbs2;
			AnimationModel* runAnimation = animState->runAnimations[j];

			for (int k = 0; k < static_cast<int>(CamVectorOrder::End); ++k)
			//for (int k = 0; k < camNums; ++k)
			{
				FrameBuffer* fb = new FrameBuffer(windowW, windowH);
				fbs2.push_back(fb);
			}
			runAnimation->fbs = fbs2;
			fbs1.push_back(fbs2);
		}
		
		for (int j = 0; j < deathAnimationsCount; ++j)
		{
			std::vector<FrameBuffer*> fbs2;
			AnimationModel* deathAnimation = animState->deathAnimations[j];

			for (int k = 0; k < static_cast<int>(CamVectorOrder::End); ++k)
			//for (int k = 0; k < camNums; ++k)
			{
				FrameBuffer* fb = new FrameBuffer(windowW, windowH);
				fbs2.push_back(fb);
			}
			deathAnimation->fbs = fbs2;
			fbs1.push_back(fbs2);
		}

		frameBuffers.push_back(fbs1);
	}

}

BillboardAnimatingDatas::~BillboardAnimatingDatas()
{

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
