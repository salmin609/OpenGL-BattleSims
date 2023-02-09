/*
 *	File for GAM400 - OpenGL MAs project.
 *	Term			- 2022F
 *	Author			- Ryan Kim
 *	Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#include "BillboardManager.h"

#include <assimp/anim.h>
#include <assimp/scene.h>

#include "AnimationModel.h"
#include "AnimationState.h"
#include "BillboardAnimatingDatas.h"
#include "Camera.hpp"
#include "Floor.hpp"
#include "FrameBuffer.h"
#include "Graphic.h"
#include "StringParser.h"
#include "MeshDatas.h"
#include "MultipleAnimationObject.h"

BillboardManager::BillboardManager(Shader* boShader_, Shader* boComputeShader_,
	int windowWidth, int windowHeight, const std::vector<std::string>& objPaths)
{
	boShader = boShader_;
	boComputeShader = boComputeShader_;

	windowW = windowWidth;
	windowH = windowHeight;

	glm::vec3 camUpVec = glm::vec3(0.f, 1.f, 0.f);
	//TODO: Need to slight modify
	//front
	boCams.push_back(new Camera(glm::vec3(0.f, 45.f, 100.f),
		camUpVec,
		-90.f, -7.f));

	//back
	boCams.push_back(new Camera(glm::vec3(0.f, 45.f, -100.f),
		camUpVec,
		90.f, -7.f));

	//left
	boCams.push_back(new Camera(glm::vec3(-90.f, 45.f, 0.f),
		camUpVec,
		0.f, -7.f));
	//right
	boCams.push_back(new Camera(glm::vec3(90.f, 45.f, 0.f),
		camUpVec,
		180.f, -7.f));

	//leftfront
	boCams.push_back(new Camera(glm::vec3(-75.f, 45.f, 75.f),
		camUpVec,
		-45.f, -7.f));

	//rightfront
	boCams.push_back(new Camera(glm::vec3(75.f, 45.f, 75.f),
		camUpVec,
		225.f, -7.f));

	//leftback
	boCams.push_back(new Camera(glm::vec3(-75.f, 45.f, -75.f),
		camUpVec,
		45.f, -7.f));

	//rightback
	boCams.push_back(new Camera(glm::vec3(75.f, 45.f, -75.f),
		camUpVec,
		135.f, -7.f));

	const int boCamsSize = static_cast<int>(boCams.size());
	for (int i = 0; i < boCamsSize; ++i)
		boCamMats.push_back(boCams[i]->GetViewMatrix());

	PopulateBoDatas(objPaths);

	//check = new FrameBuffer(windowWidth * 3, windowHeight * 3);
}

BillboardManager::~BillboardManager()
{
	for (const auto& meshData : meshDatas)
		delete meshData.second;

	for (const auto boData : boDatas)
		delete boData;

	//delete boCamera;
	for (const auto boCam : boCams)
		delete boCam;
}

void BillboardManager::PopulateBoDatas(const std::vector<std::string>& objPaths)
{
	const size_t objPathsSize = objPaths.size();
	const glm::vec3 objPos(0.f, 10.f, 0.f);

	//std::vector<AnimationModel*> models;

	std::string prevKind;

	for (size_t i = 0; i < objPathsSize; ++i)
	{
		const std::string objPath = objPaths[i];
		std::vector<std::string> vec = split(objPath, "_");
		std::vector<std::string> finalParsed = split(vec[0], "/");
		std::string modelKind = finalParsed[finalParsed.size() - 1];
		std::string animKind = vec[1];

		MeshDatas* reusableMeshDatas = nullptr;

		auto reuse = meshDatas.find(modelKind);
		if (reuse != meshDatas.end())
			reusableMeshDatas = reuse->second;

		AnimationModel* newAnimation = new AnimationModel(boShader, objPath, boComputeShader, reusableMeshDatas);

		//means not exist
		auto state = animModels.find(modelKind);

		if (state == animModels.end())
		{
			AnimationState* animState = new AnimationState();

			animModels.insert(std::pair<std::string, AnimationState*>(modelKind, animState));
			meshDatas.insert(std::pair<std::string, MeshDatas*>(modelKind, newAnimation->datas->meshDatas));

			//idle is first animation of model.
			animState->idleAnimations.push_back(newAnimation);
		}
		//means exist
		else
		{
			AnimationState* animState = state->second;

			if (animKind == "Idle")
				animState->idleAnimations.push_back(newAnimation);
			else if (animKind == "Run")
				animState->runAnimation = newAnimation;
			else if (animKind == "Attack")
				animState->attackAnimations.push_back(newAnimation);
			else if (animKind == "Death")
				animState->deathAnimation = newAnimation;
			else if (animKind == "Pain")
				animState->painAnimations.push_back(newAnimation);
		}
	}
	int a = 0;



	//for (size_t i = 0; i < objPathsSize; ++i)
	//{
	//	const std::string objPath = objPaths[i];
	//	std::vector<std::string> vec = split(objPath, "_");
	//	std::vector<std::string> finalParsed = split(vec[0], "/");
	//	std::string modelKind = finalParsed[finalParsed.size() - 1];

	//	if (!models.empty())
	//	{
	//		if (prevKind != modelKind)
	//		{
	//			//animModels.push_back(models);
	//			models.clear();
	//		}
	//	}

	//	MeshDatas* reusableMeshDatas = nullptr;

	//	auto found = meshDatas.find(modelKind);

	//	if (found != meshDatas.end())
	//		reusableMeshDatas = found->second;

	//	AnimationModel* newModel = new AnimationModel(boShader, objPath, boComputeShader, reusableMeshDatas);
	//	//animModels.push_back(newModel);
	//	models.push_back(newModel);


	//	if (found == meshDatas.end())
	//		meshDatas.insert(std::pair<std::string, MeshDatas*>(modelKind, newModel->datas->meshDatas));

	//	prevKind = modelKind;

	//}
	//animModels.push_back(models);

	const size_t animModelsSize = animModels.size();

	std::map<std::string, AnimationState*>::iterator it = animModels.begin();

	for (it = animModels.begin(); it != animModels.end(); ++it)
	{
		MultipleAnimationObject* mObj = new MultipleAnimationObject(objPos, glm::vec3(0.f, -5.f, 0.f), glm::vec3(30.f, 30.f, 30.f));

		AnimationState* state = it->second;
		mObj->animState = state;

		BillboardAnimatingDatas* boData = new BillboardAnimatingDatas(windowW, windowH, mObj);
		boDatas.push_back(boData);
	}
}

void BillboardManager::GenBillboard(const glm::mat4& projMat)
{
	const size_t boDatasSize = boDatas.size();
	const std::chrono::system_clock::time_point current = std::chrono::system_clock::now();

	for (size_t i = 0; i < boDatasSize; ++i)
	{
		BillboardAnimatingDatas* boData = boDatas[i];

		//if (boData->inUse)
		GenerateBillboard(current, projMat, boData);
	}
}


//void BillboardManager::ChangeAnimationIndexByTime()
//{
//	const int boDatasSize = static_cast<int>(boDatas.size());
//
//	for (int i = 0; i < boDatasSize; ++i)
//	{
//		boDatas[i]->obj->ChangeCurrentAnimationWithTime();
//	}
//}



Camera* BillboardManager::GetBoObjCamera(int camIndex)
{
	return boCams[camIndex];
}

BillboardAnimatingDatas* BillboardManager::GetAnimData(int index)
{
	return boDatas[index];
}


void BillboardManager::GenerateBillboard(const std::chrono::system_clock::time_point& current
	, const glm::mat4& projMat, BillboardAnimatingDatas* datas)
{
	//const int animationsSize = static_cast<int>(datas->obj->animationModels.size());
	//const int animationsSize = 2;

	glClearColor(0.f, 0.f, 0.f, 0.f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	AnimationState* animState = datas->obj->animState;

	for (int i = 0; i < datas->diffTimeAnimCount; ++i)
	{
		const int idleAnimationsCount = static_cast<int>(animState->idleAnimations.size());
		const int attackAnimationsCount = static_cast<int>(animState->attackAnimations.size());
		const int painAnimationsCount = static_cast<int>(animState->painAnimations.size());
		const int runAnimationsCount = 1;
		const int deathAnimationsCount = 1;

		for (int j = 0; j < idleAnimationsCount; ++j)
		{
			const AnimationModel* model = datas->obj->animState->idleAnimations[j];
			const aiAnimation* animation = model->GetScene()->mAnimations[0];
			const float animationTimeTicks = GetAnimationTimeTicks(current, model->startTime, animation, i);
			glm::mat4* transformMat = model->Interpolate(animationTimeTicks);

			for (int k = 0; k < static_cast<int>(CamVectorOrder::End); ++k)
			{
				FrameBuffer* fb = datas->frameBuffers[i][0][k];

				if (fb->isOnUsage)
				{
					const glm::mat4 projViewMat = projMat * boCamMats[k];

					fb->Bind();
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

					datas->obj->Draw(projViewMat, transformMat);

					fb->UnBind();
					fb->isOnUsage = false;
				}
			}
		}

		for (int j = 0; j < attackAnimationsCount; ++j)
		{
			const AnimationModel* model = datas->obj->animState->attackAnimations[j];
			const aiAnimation* animation = model->GetScene()->mAnimations[0];
			const float animationTimeTicks = GetAnimationTimeTicks(current, model->startTime, animation, i);
			glm::mat4* transformMat = model->Interpolate(animationTimeTicks);

			for (int k = 0; k < static_cast<int>(CamVectorOrder::End); ++k)
			{
				FrameBuffer* fb = datas->frameBuffers[i][1][k];

				if (fb->isOnUsage)
				{
					const glm::mat4 projViewMat = projMat * boCamMats[k];

					fb->Bind();
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

					datas->obj->Draw(projViewMat, transformMat);

					fb->UnBind();
					fb->isOnUsage = false;
				}
			}
		}

		for (int j = 0; j < painAnimationsCount; ++j)
		{
			const AnimationModel* model = datas->obj->animState->painAnimations[j];
			const aiAnimation* animation = model->GetScene()->mAnimations[0];
			const float animationTimeTicks = GetAnimationTimeTicks(current, model->startTime, animation, i);
			glm::mat4* transformMat = model->Interpolate(animationTimeTicks);

			for (int k = 0; k < static_cast<int>(CamVectorOrder::End); ++k)
			{
				FrameBuffer* fb = datas->frameBuffers[i][2][k];

				if (fb->isOnUsage)
				{
					const glm::mat4 projViewMat = projMat * boCamMats[k];

					fb->Bind();
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

					datas->obj->Draw(projViewMat, transformMat);

					fb->UnBind();
					fb->isOnUsage = false;
				}
			}
		}

		for (int j = 0; j < runAnimationsCount; ++j)
		{
			const AnimationModel* model = datas->obj->animState->runAnimation;
			const aiAnimation* animation = model->GetScene()->mAnimations[0];
			const float animationTimeTicks = GetAnimationTimeTicks(current, model->startTime, animation, i);
			glm::mat4* transformMat = model->Interpolate(animationTimeTicks);

			for (int k = 0; k < static_cast<int>(CamVectorOrder::End); ++k)
			{
				FrameBuffer* fb = datas->frameBuffers[i][3][k];

				if (fb->isOnUsage)
				{
					const glm::mat4 projViewMat = projMat * boCamMats[k];

					fb->Bind();
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

					datas->obj->Draw(projViewMat, transformMat);

					fb->UnBind();
					fb->isOnUsage = false;
				}
			}
		}

		for (int j = 0; j < deathAnimationsCount; ++j)
		{
			const AnimationModel* model = datas->obj->animState->deathAnimation;
			const aiAnimation* animation = model->GetScene()->mAnimations[0];
			const float animationTimeTicks = GetAnimationTimeTicks(current, model->startTime, animation, i);
			glm::mat4* transformMat = model->Interpolate(animationTimeTicks);

			for (int k = 0; k < static_cast<int>(CamVectorOrder::End); ++k)
			{
				FrameBuffer* fb = datas->frameBuffers[i][4][k];

				if (fb->isOnUsage)
				{
					const glm::mat4 projViewMat = projMat * boCamMats[k];

					fb->Bind();
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

					datas->obj->Draw(projViewMat, transformMat);

					fb->UnBind();
					fb->isOnUsage = false;
				}
			}
		}
	}



	//for (int i = 0; i < datas->diffTimeAnimCount; ++i)
	//{
	//	for (int j = 0; j < animationsSize; ++j)
	//	{
	//		//const AnimationModel* model = datas->obj->animationModels[j];
	//		const AnimationModel* model = datas->obj->animState->idleAnimations[0];

	//		const aiAnimation* animation = model->GetScene()->mAnimations[0];

	//		const float animationTimeTicks = GetAnimationTimeTicks(current, model->startTime, animation,
	//			i);
	//		glm::mat4* transformMat = model->Interpolate(animationTimeTicks);
	//		//glm::mat4* transformMat = model->initialBoneTransformsData;
	//		assert(transformMat != nullptr);

	//		for (int k = 0; k < static_cast<int>(CamVectorOrder::End); ++k)
	//		{
	//			FrameBuffer* fb = datas->frameBuffers[i][j][k];

	//			if(fb->isOnUsage)
	//			{
	//				const glm::mat4 projViewMat = projMat * boCamMats[k];

	//				datas->frameBuffers[i][j][k]->Bind();
	//				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//				datas->obj->Draw(projViewMat, transformMat);

	//				datas->frameBuffers[i][j][k]->UnBind();
	//				fb->isOnUsage = false;
	//			}
	//		}
	//	}
	//}
}

void BillboardManager::SaveAngleTextures(BillboardAnimatingDatas* datas)
{
	if (once)
	{
		glBlitNamedFramebuffer(datas->frameBuffers[0][0][0]->GetFrameBufferId(), check->GetFrameBufferId(),
			0, 0, windowW, windowH,
			0, 0, windowW, windowH,
			GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);

		glBlitNamedFramebuffer(datas->frameBuffers[0][0][1]->GetFrameBufferId(), check->GetFrameBufferId(),
			0, 0, windowW, windowH,
			windowW, 0, 2 * windowW, windowH,
			GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);

		glBlitNamedFramebuffer(datas->frameBuffers[0][0][2]->GetFrameBufferId(), check->GetFrameBufferId(),
			0, 0, windowW, windowH,
			windowW * 2, 0, 3 * windowW, windowH,
			GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);

		glBlitNamedFramebuffer(datas->frameBuffers[0][0][3]->GetFrameBufferId(), check->GetFrameBufferId(),
			0, 0, windowW, windowH,
			0, windowH, windowW, windowH * 2,
			GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);

		glBlitNamedFramebuffer(datas->frameBuffers[0][0][4]->GetFrameBufferId(), check->GetFrameBufferId(),
			0, 0, windowW, windowH,
			windowW, windowH, windowW * 2, windowH * 2,
			GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);

		glBlitNamedFramebuffer(datas->frameBuffers[0][0][5]->GetFrameBufferId(), check->GetFrameBufferId(),
			0, 0, windowW, windowH,
			windowW * 2, windowH, windowW * 3, windowH * 2,
			GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);

		glBlitNamedFramebuffer(datas->frameBuffers[0][0][6]->GetFrameBufferId(), check->GetFrameBufferId(),
			0, 0, windowW, windowH,
			0, windowH * 2, windowW, windowH * 3,
			GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);

		glBlitNamedFramebuffer(datas->frameBuffers[0][0][7]->GetFrameBufferId(), check->GetFrameBufferId(),
			0, 0, windowW, windowH,
			windowW, windowH * 2, windowW * 2, windowH * 3,
			GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);

		//check->SavePNG();
		once = false;
	}
}

float BillboardManager::GetAnimationTimeTicks(const std::chrono::system_clock::time_point& current,
	const std::chrono::system_clock::time_point& startTime,
	const aiAnimation* animation, int index) const
{
	const long long diff =
		std::chrono::duration_cast<std::chrono::milliseconds>(current - startTime).count();
	float animationT = static_cast<float>(diff) / 1000.f;

	//make diff in animation
	animationT += static_cast<float>(index) * 60.f;

	const float timeInTicks = animationT * static_cast<float>(animation->mTicksPerSecond);
	const float animationTimeTicks = fmod(timeInTicks, static_cast<float>(animation->mDuration));

	return animationTimeTicks;
}
