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

	for (auto it = animModels.begin(); it != animModels.end(); ++it)
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

void BillboardManager::SaveAnimation(std::vector<AnimationModel*> animations, 
	AnimationState::State state,
	const std::chrono::system_clock::time_point& current,
	const std::vector<std::vector<std::vector<FrameBuffer*>>>& frameBuffers,
	const glm::mat4& projMat, const glm::mat4& modelMat,
	AnimationModel* baseModel)
{
	const int animationCount = static_cast<int>(animations.size());

	for(int i = 0; i < animationCount; ++i)
	{
		AnimationModel* model = animations[i];
		const float animationTimeTicks = model->GetAnimationTimeTicks(current);

		glm::mat4* transformMat = model->Interpolate(animationTimeTicks);

		for (int k = 0; k < static_cast<int>(CamVectorOrder::End); ++k)
		{
			FrameBuffer* fb = frameBuffers[i]
				[static_cast<int>(state)][k];

			if (fb->isOnUsage)
			{
				const glm::mat4 projViewMat = projMat * boCamMats[k];

				fb->Bind();
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				baseModel->Draw(modelMat, projViewMat,
					transformMat);

				fb->UnBind();
				fb->isOnUsage = false;
			}

		}
	}
}


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
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	AnimationModel* baseModel = datas->obj->animState->idleAnimations[0];

	for (int i = 0; i < datas->diffTimeAnimCount; ++i)
	{
		SaveAnimation(datas->obj->animState->idleAnimations,
			AnimationState::State::Idle,
			current,
			datas->frameBuffers,
			projMat,
			datas->obj->GetModelMatrix(),
			baseModel);

		SaveAnimation(datas->obj->animState->attackAnimations,
			AnimationState::State::Attack,
			current,
			datas->frameBuffers,
			projMat,
			datas->obj->GetModelMatrix(),
			baseModel);

		SaveAnimation(datas->obj->animState->painAnimations,
			AnimationState::State::Pain,
			current,
			datas->frameBuffers,
			projMat,
			datas->obj->GetModelMatrix(),
			baseModel);

		SaveAnimation(std::vector<AnimationModel*>{datas->obj->animState->runAnimation},
			AnimationState::State::Run,
			current,
			datas->frameBuffers,
			projMat,
			datas->obj->GetModelMatrix(),
			baseModel);

		SaveAnimation(std::vector<AnimationModel*>{datas->obj->animState->deathAnimation},
			AnimationState::State::Death,
			current,
			datas->frameBuffers,
			projMat,
			datas->obj->GetModelMatrix(),
			baseModel);
	}
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
