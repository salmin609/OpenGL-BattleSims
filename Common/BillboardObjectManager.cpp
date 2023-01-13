#include "BillboardObjectManager.h"

#include "BillboardAnimatingDatas.h"
#include "BillboardManager.h"
#include "BillBoardObject.h"
#include "MultipleAnimationObject.h"

BillboardObjectManager::BillboardObjectManager(Shader* boShader_, BillboardManager* boManager_,
	Camera* currentCam_)
{
	boShader = boShader_;
	boManager = boManager_;
	currentCam = currentCam_;
	PopulateObjsPos();
}

BillboardObjectManager::~BillboardObjectManager()
{
	for (const auto bo : bos)
		delete bo;
}

void BillboardObjectManager::PopulateObjsPos()
{
	glm::vec3 pos = populateLastPosition;

	for (int i = 0; i < 200000; ++i)
	{
		if (i % 200 == 0)
		{
			pos.x = 0.f;
			pos.z += 20.f;
		}

		pos.x += 20.f;

		objsPos.push_back(pos);
	}
}

void BillboardObjectManager::PopulateObjs(int num, int obj)
{
	BillboardAnimatingDatas* data = boManager->boDatas[obj];

	const int animationCount = static_cast<int>(data->obj->animationModels.size());
	data->inUse = true;

	for (int i = 0; i < num; ++i)
	{
		const int animationIndex = rand() % animationCount;
		const int timeDiffSlot = rand() % data->diffTimeAnimCount;

		const glm::vec3& pos = objsPos[posOffset + i];

		bos.push_back(new BillBoardObject(boShader,
			pos, data->frameBuffers[timeDiffSlot][animationIndex], currentCam));
	}

	posOffset += num;

	totalRenderingAmount += num;
}

void BillboardObjectManager::Populate()
{
	PopulateObjs(1000, 0);
	PopulateObjs(1000, 1);
	PopulateObjs(1000, 2);
}

void BillboardObjectManager::CheckFrameBufferUsage(Frustum* frustum)
{
	for (const auto& bo : bos)
		bo->CheckFrameBufferUsage(frustum);
}

void BillboardObjectManager::Render(const glm::mat4& projMat, const glm::mat4& viewMat, Frustum* frustum)
{
	for (const auto& bo : bos)
	{
		bo->Render(projMat, viewMat, frustum);
	}
}
