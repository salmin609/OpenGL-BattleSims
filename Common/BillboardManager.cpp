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

	//TODO: Need to slight modify
	//front
	boCams.push_back(new Camera(glm::vec3(2281.67f, 48.9464f, 610.049f),
		glm::vec3(0.f, 1.f, 0.f),
		-4.9f, -12.1f));

	//back
	boCams.push_back(new Camera(glm::vec3(2504.97f, 37.6267f, 597.099f),
		glm::vec3(0.f, 1.f, 0.f),
		-180.3f, -6.1f));

	//left
	boCams.push_back(new Camera(glm::vec3(2395.03f, 40.8007f, 469.752f),
		glm::vec3(-0.000248875f, 0.989776f, 0.142629f),
		90.1f, -8.20001f));
	//right
	boCams.push_back(new Camera(glm::vec3(2397.98f, 47.8767f, 731.28f),
		glm::vec3(0.00466186f, 0.981627f, -0.190752f),
		-88.6f, -11.f));

	//leftfront
	boCams.push_back(new Camera(glm::vec3(2340.53f, 45.0763f, 475.073f),
		glm::vec3(0.0694263f, 0.982595f, 0.172299f),
		-293.8f, -10.7f));

	//rightfront
	boCams.push_back(new Camera(glm::vec3(2340.38f, 46.89f, 725.429f),
		glm::vec3(0.0856022f, 0.981293f, -0.172444f),
		-63.6f, -11.1f));

	//leftback
	boCams.push_back(new Camera(glm::vec3(2483.67f, 47.9343f, 706.051f),
		glm::vec3(-0.126298f, 0.981471f, -0.144093f),
		-485.9f, -11.f));

	//rightback
	boCams.push_back(new Camera(glm::vec3(2503.77f, 42.3205f, 519.317f),
		glm::vec3(-0.1453f, 0.983572f, 0.107121f),
		-576.399f, -10.4f));
	

	PopulateBoDatas(objPaths);
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
	glm::vec3 objPos(20.f, 0.f, -80.f);

	std::vector<AnimationModel*> models;
	std::string prevKind;

	for (size_t i = 0; i < objPathsSize; ++i)
	{
		const std::string objPath = objPaths[i];
		std::vector<std::string> vec = split(objPath, "_");
		std::vector<std::string> finalParsed = split(vec[0], "/");
		std::string modelKind = finalParsed[finalParsed.size() - 1];

		if (!models.empty())
		{
			if (prevKind != modelKind)
			{
				animModels.push_back(models);
				models.clear();
			}
		}

		MeshDatas* reusableMeshDatas = nullptr;

		auto found = meshDatas.find(modelKind);

		if (found != meshDatas.end())
			reusableMeshDatas = found->second;

		AnimationModel* newModel = new AnimationModel(boShader, objPath, boComputeShader, reusableMeshDatas);
		//animModels.push_back(newModel);
		models.push_back(newModel);


		if (found == meshDatas.end())
			meshDatas.insert(std::pair<std::string, MeshDatas*>(modelKind, newModel->datas->meshDatas));

		prevKind = modelKind;

	}
	animModels.push_back(models);

	const size_t animModelsSize = animModels.size();

	for (size_t i = 0; i < animModelsSize; ++i)
	{
		MultipleAnimationObject* mObj = new MultipleAnimationObject(objPos, glm::vec3(0.f, -5.f, 0.f), glm::vec3(30.f, 30.f, 30.f));

		std::vector<AnimationModel*> modelVec = animModels[i];

		const size_t modelVecSize = modelVec.size();

		for (size_t j = 0; j < modelVecSize; ++j)
		{
			AnimationModel* animModel = modelVec[j];

			mObj->AddAnimation(animModel);
		}

		boDatas.push_back(new BillboardAnimatingDatas(windowW, windowH, mObj));
	}
}

void BillboardManager::GenBillboard(const glm::mat4& projMat)
{
	size_t boDatasSize = boDatas.size();
	const std::chrono::system_clock::time_point current = std::chrono::system_clock::now();

	for (size_t i = 0; i < boDatasSize; ++i)
	{
		BillboardAnimatingDatas* boData = boDatas[i];

		if (boData->inUse)
			GenerateBillboard(current, projMat, boData);
	}
}


void BillboardManager::ChangeAnimationIndexByTime()
{
	const int boDatasSize = static_cast<int>(boDatas.size());

	for (int i = 0; i < boDatasSize; ++i)
	{
		boDatas[i]->obj->ChangeCurrentAnimationWithTime();
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
	, const glm::mat4& projMat, BillboardAnimatingDatas* datas) const
{
	const int animationsSize = static_cast<int>(datas->obj->animationModels.size());

	glClearColor(0.f, 0.f, 0.f, 0.f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//const glm::mat4 viewMat = boCams[static_cast<int>(CamVectorOrder::Back)]->GetViewMatrix();
	//const glm::mat4 projViewMat = projMat * viewMat;

	for (int i = 0; i < datas->diffTimeAnimCount; ++i)
	{
		for (int j = 0; j < animationsSize; ++j)
		{
			const AnimationModel* model = datas->obj->animationModels[j];
			const aiAnimation* animation = model->GetScene()->mAnimations[0];

			const float animationTimeTicks = GetAnimationTimeTicks(current, model->startTime, animation,
				i);
			const std::vector<glm::mat4> transformMat = model->Interpolate(animationTimeTicks);

			for (int k = 0; k < static_cast<int>(CamVectorOrder::End); ++k)
			{
				const glm::mat4 viewMat = boCams[k]->GetViewMatrix();
				const glm::mat4 projViewMat = projMat * viewMat;

				datas->frameBuffers[i][j][k]->Bind();
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				datas->obj->Draw(projViewMat, transformMat);

				datas->frameBuffers[i][j][k]->UnBind();
			}
		}
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
