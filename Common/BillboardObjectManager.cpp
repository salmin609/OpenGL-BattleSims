#include "BillboardObjectManager.h"

#include "BillboardAnimatingDatas.h"
#include "BillboardManager.h"
#include "BillBoardObject.h"
#include "Buffer.hpp"
#include "Camera.hpp"
#include "FrameBuffer.h"
#include "FrustumCulling.h"
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

	PopulateObjsPos();
	Populate();

	boFBusageDatas = new int[boFrameBufferUsageBuffer->GetSize()];
}

BillboardObjectManager::~BillboardObjectManager()
{
	for (const auto bo : bos)
		delete bo;

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

	for (int i = 0; i < num; ++i)
	{
		const int animationIndex = rand() % animationCount;
		const int timeDiffSlot = rand() % data->diffTimeAnimCount;

		const glm::vec3& pos = objsPos[posOffset + i];

		bos.push_back(new BillBoardObject(boShader,
			pos, data->frameBuffers[timeDiffSlot][animationIndex], currentCam));

		posDatas.emplace_back(pos, 1.f);
	}

	posOffset += num;

	totalRenderingAmount += num;
}

void BillboardObjectManager::Populate()
{
	PopulateObjs(2560, 0);
	PopulateObjs(2560, 1);
	PopulateObjs(2560, 2);
	PopulateObjs(2560, 3);
	PopulateObjs(2560, 4);

	const int posDatasSize = static_cast<int>(posDatas.size());

	boFrameBufferUsageBuffer = new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(int) * posDatasSize, GL_DYNAMIC_DRAW,
		nullptr, 0);

	boPosBuffer = new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * posDatasSize, GL_DYNAMIC_DRAW,
		posDatas.data(), 1);
}

void BillboardObjectManager::CheckFrameBufferUsage(Camera* cam, float fov)
{
	boFBusageComputeShader->Use();
	boFrameBufferUsageBuffer->BindStorage();
	boPosBuffer->BindStorage();

	boFBusageComputeShader->SendUniformVec3("camPos", &cam->Position);
	boFBusageComputeShader->SendUniformVec3("camFront", &cam->Front);
	boFBusageComputeShader->SendUniformVec3("camRight", &cam->Right);
	boFBusageComputeShader->SendUniformVec3("camUp", &cam->Up);
	boFBusageComputeShader->SendUniformFloat("aspect", &fov);
	boFBusageComputeShader->SendUniformFloat("fovY", glm::radians(cam->Zoom));
	boFBusageComputeShader->SendUniformFloat("zNear", zNear);
	boFBusageComputeShader->SendUniformFloat("zFar", zFar);
	boFBusageComputeShader->SendUniformInt("bufferSize", static_cast<int>(posDatas.size()));

	glDispatchCompute(static_cast<int>(posDatas.size()) / 128, 1, 1);
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	glUseProgram(0);

	boFrameBufferUsageBuffer->GetData(boFBusageDatas);

	const size_t bosSize = bos.size();

	for(size_t i = 0; i < bosSize; ++i)
	{
		BillBoardObject* bo = bos[i];

		if (boFBusageDatas[i] >= 0)
			bo->SetFrameBufferIndex(boFBusageDatas[i]);
		else
			bo->usingFrameBuffer = nullptr;
	}
}

void BillboardObjectManager::Render(const glm::mat4& projMat, const glm::mat4& viewMat)
{
	for (const auto& bo : bos)
	{
		bo->Render(projMat, viewMat);
	}

	//boShader->Use();
	//glBindVertexArray(vao);
	//boPosBuffer->BindStorage(0);
	
}

void BillboardObjectManager::GenerateArrayTexture(const std::vector<BillboardAnimatingDatas*>& boDatas)
{
	glGenTextures(1, &gArrayTexture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, gArrayTexture);

	glTexStorage3D(GL_TEXTURE_2D_ARRAY,
		1,
		GL_RGBA,
		1024,
		768,
		100);

	const size_t boDatasSize = boDatas.size();

	for(size_t i = 0; i < boDatasSize; ++i)
	{
		const BillboardAnimatingDatas* boData = boDatas[i];
		std::vector<std::vector<std::vector<FrameBuffer*>>> fbs = boData->frameBuffers;




	}



}
