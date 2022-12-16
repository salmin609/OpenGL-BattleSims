/*
 *	File for GAM400 - OpenGL MAs project.
 *	Term			- 2022F
 *	Author			- Ryan Kim
 *	Copyright � 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#include "BillboardManager.h"

#include <assimp/anim.h>
#include <assimp/scene.h>

#include "AnimationModel.h"
#include "BillboardAnimatingDatas.h"
#include "Camera.hpp"
#include "Floor.hpp"
#include "FrameBuffer.h"
#include "Graphic.h"
#include "Object.h"
#include "StringParser.h"
#include "MeshDatas.h"
#include "MultipleAnimationObject.h"
#include "SingleAnimationObject.h"

BillboardManager::BillboardManager(Shader* boShader_, Shader* boComputeShader_, 
                                   int windowWidth, int windowHeight, const std::vector<std::string>& objPaths)
{
	boShader = boShader_;
	boComputeShader = boComputeShader_;

	windowW = windowWidth;
	windowH = windowHeight;
	PopulateBoDatas(objPaths);
}

BillboardManager::~BillboardManager()
{
	for(const auto& meshData : meshDatas)
	{
		delete meshData.second;
	}

	for (const auto boData : boDatas)
		delete boData;
}

void BillboardManager::PopulateBoDatas(const std::vector<std::string>& objPaths)
{
	const size_t objPathsSize = objPaths.size();
	glm::vec3 objPos(20.f, 0.f, -80.f);
	for(size_t i = 0; i < objPathsSize; ++i)
	{
		std::string objPath = objPaths[i];
		std::vector<std::string> vec = split(objPath, "_");
		std::vector<std::string> finalParsed = split(vec[0], "/");

		std::string kindString = finalParsed[finalParsed.size() - 1];

		MeshDatas* reusableMeshDatas = nullptr;

		auto found = meshDatas.find(kindString);

		if(found != meshDatas.end())
			reusableMeshDatas = found->second;

		boDatas.push_back(new BillboardAnimatingDatas(objPath, boShader, boComputeShader,
			objPos,
			new Camera(glm::vec3(2281.67f, 48.9464f, 610.049f),
				glm::vec3(0.20865f, 0.97778f, -0.0202643f),
				-4.9f, -12.1f), windowW, windowH,
			reusableMeshDatas));

		if(found == meshDatas.end())
			meshDatas.insert(std::pair<std::string, MeshDatas*>(kindString, boDatas[i]->model->datas->meshDatas));
	}
}

void BillboardManager::GenBillboard(const glm::mat4& projMat)
{
	size_t boDatasSize = boDatas.size();
	const std::chrono::system_clock::time_point current = std::chrono::system_clock::now();

	for (size_t i = 0; i < boDatasSize; ++i)
	{
		BillboardAnimatingDatas* boData = boDatas[i];

		if(boData->inUse)
			GenerateBillboard(current, projMat, boData);
	}
}

void BillboardManager::ResetFrameBufferWidthHeight(int w, int h)
{
	const size_t bosSize = boDatas.size();

	//Delete frameBuffer
	for(size_t i = 0; i < bosSize; ++i)
	{
		delete boDatas[i]->frameBuffer;
	}

	for (size_t i = 0; i < bosSize; ++i)
	{
		boDatas[i]->frameBuffer = new FrameBuffer(w, h);
	}
}

Camera* BillboardManager::GetBoObjCamera(int index)
{
	return boDatas[index]->cam;
}

BillboardAnimatingDatas* BillboardManager::GetAnimData(int index)
{
	return boDatas[index];
}


void BillboardManager::GenerateBillboard(const std::chrono::system_clock::time_point& current
                                         ,const glm::mat4& projMat, BillboardAnimatingDatas* datas) const
{
	datas->frameBuffer->Bind();

	glClearColor(0.f, 0.f, 0.f, 0.f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	const glm::mat4 viewMat = datas->cam->GetViewMatrix();
	const glm::mat4 projViewMat = projMat * viewMat;

	AnimationModel* model = datas->obj->animationModel;
	const aiAnimation* animation = model->GetScene()->mAnimations[0];
	const long long diff = std::chrono::duration_cast<std::chrono::milliseconds>(current - model->startTime).count();
	const float animationT = static_cast<float>(diff) / 1000.f;

	const float timeInTicks = animationT * static_cast<float>(animation->mTicksPerSecond);
	const float animationTimeTicks = fmod(timeInTicks, static_cast<float>(animation->mDuration));
	datas->obj->Draw(projViewMat, animationT, 0, model->Interpolate(animationTimeTicks));

	datas->frameBuffer->UnBind();
}
