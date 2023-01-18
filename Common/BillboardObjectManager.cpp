#include "BillboardObjectManager.h"

#include "BillboardAnimatingDatas.h"
#include "BillboardManager.h"
#include "BillBoardObject.h"
#include "Buffer.hpp"
#include "Camera.hpp"
#include "FrameBuffer.h"
#include "ModelKinds.hpp"
#include "MultipleAnimationObject.h"
#include "Shader.h"

BillboardObjectManager::BillboardObjectManager(Shader* boShader_, BillboardManager* boManager_,
												Shader* boFrameBufferUsageComputeShader,
                                               Camera* currentCam_): boFrameBufferUsageBuffer(nullptr)
{
	boShader = boShader_;
	boManager = boManager_;
	currentCam = currentCam_;
	boFBusageComputeShader = boFrameBufferUsageComputeShader;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(0);

	for(int i = 0; i < static_cast<int>(ObjKind::END); ++i)
	{
		std::vector<BillBoardObject*> bo;
		bos.push_back(bo);
	}

	PopulateObjsPos();
	Populate();

	boFBusageDatas = new int[boFrameBufferUsageBuffer->GetSize()];
}

BillboardObjectManager::~BillboardObjectManager()
{
	for (const auto bo : bos)
		for(const auto b : bo)
			delete b;

	delete[] boFBusageDatas;
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

	std::vector<glm::vec4> poses;

	for (int i = 0; i < num; ++i)
	{
		const int animationIndex = rand() % animationCount;
		const int timeDiffSlot = rand() % data->diffTimeAnimCount;

		const glm::vec3& pos = objsPos[posOffset + i];

		bos[obj].push_back(new BillBoardObject(boShader,
			pos, data->frameBuffers[timeDiffSlot][animationIndex], currentCam));

		poses.emplace_back(pos, 1.f);

		totalPositionBufferCount++;
	}

	posDatas.push_back(poses);

	posOffset += num;

	totalRenderingAmount += num;
}

void BillboardObjectManager::Populate()
{
	PopulateObjs(2560, static_cast<int>(ObjKind::SWAT));
	PopulateObjs(2560, static_cast<int>(ObjKind::AMY));
	PopulateObjs(2560, static_cast<int>(ObjKind::KNIGHT));
	PopulateObjs(2560, static_cast<int>(ObjKind::MICHELLE));
	PopulateObjs(2560, static_cast<int>(ObjKind::ADAM));


	boFrameBufferUsageBuffer = new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(int) * totalPositionBufferCount, GL_DYNAMIC_DRAW,
		nullptr, 5);

	for (int i = 0; i < static_cast<int>(ObjKind::END); ++i)
	{
		boPosBuffer.push_back(new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * static_cast<int>(posDatas[i].size()),
			GL_DYNAMIC_DRAW, posDatas[i].data(), i));
	}
}

void BillboardObjectManager::CheckFrameBufferUsage(Camera* cam, float fov)
{
	boFBusageComputeShader->Use();
	boFrameBufferUsageBuffer->BindStorage();

	for(int i = 0; i < static_cast<int>(ObjKind::END); ++i)
	{
		boPosBuffer[i]->BindStorage();
	}
	boFBusageComputeShader->SendUniformVec3("camPos", &cam->Position);
	boFBusageComputeShader->SendUniformVec3("camFront", &cam->Front);
	boFBusageComputeShader->SendUniformVec3("camRight", &cam->Right);
	boFBusageComputeShader->SendUniformVec3("camUp", &cam->Up);
	boFBusageComputeShader->SendUniformFloat("aspect", &fov);
	boFBusageComputeShader->SendUniformFloat("fovY", glm::radians(cam->Zoom));
	boFBusageComputeShader->SendUniformFloat("zNear", zNear);
	boFBusageComputeShader->SendUniformFloat("zFar", zFar);
	boFBusageComputeShader->SendUniformInt("bufferSize", totalPositionBufferCount);

	int size = 0;
	boFBusageComputeShader->SendUniformInt("posOffset0", size);
	size += static_cast<int>(posDatas[0].size());
	boFBusageComputeShader->SendUniformInt("posOffset1", size);
	size += static_cast<int>(posDatas[1].size());
	boFBusageComputeShader->SendUniformInt("posOffset2", size);
	size += static_cast<int>(posDatas[2].size());
	boFBusageComputeShader->SendUniformInt("posOffset3", size);
	size += static_cast<int>(posDatas[3].size());
	boFBusageComputeShader->SendUniformInt("posOffset4", size);

	glDispatchCompute(totalPositionBufferCount / 128, 1, 1);
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	glUseProgram(0);

	boFrameBufferUsageBuffer->GetData(boFBusageDatas);

	const size_t bosSize = bos.size();

	int bufIndex = 0;

	for(size_t i = 0; i < bosSize; ++i)
	{
		std::vector<BillBoardObject*>& bo = bos[i];

		const size_t boSize = bo.size();

		for(size_t j = 0; j < boSize; ++j)
		{
			const int usingFbIndex = boFBusageDatas[bufIndex];

			if (usingFbIndex >= 0)
				bo[j]->SetFrameBufferIndex(usingFbIndex);
			else
				bo[j]->usingFrameBuffer = nullptr;

			bufIndex++;
		}
	}
}

void BillboardObjectManager::Render(const glm::mat4& projMat, const glm::mat4& viewMat,
	const std::vector<BillboardAnimatingDatas*>& boDatas)
{
	for (const auto& bo : bos)
		for(const auto& b : bo)
			b->Render(projMat, viewMat);
}

void BillboardObjectManager::GenerateArrayTexture(const std::vector<BillboardAnimatingDatas*>& boDatas)
{

}
