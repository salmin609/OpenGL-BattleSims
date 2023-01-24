#include "BillboardObjectManager.h"

#include <iostream>

#include "BillboardAnimatingDatas.h"
#include "BillboardManager.h"
#include "BillBoardObject.h"
#include "Buffer.hpp"
#include "Camera.hpp"
#include "BufferManager.h"
#include "FrameBuffer.h"
#include "Herd.h"
#include "ModelKinds.hpp"
#include "MultipleAnimationObject.h"
#include "Shader.h"

BillboardObjectManager::BillboardObjectManager(Shader* boShader_, BillboardManager* boManager_,
												Shader* boFrameBufferUsageComputeShader,
                                               Camera* currentCam_)
{
	boShader = boShader_;
	boManager = boManager_;
	currentCam = currentCam_;
	boFBusageComputeShader = boFrameBufferUsageComputeShader;

	csBuffers = new BufferManager();
	glGenVertexArrays(1, &vao);
	glBindVertexArray(0);

	for (int i = 0; i < static_cast<int>(ObjKind::END); ++i)
	{
		std::vector<BillBoardObject*> bo;
		bos.push_back(bo);
	}

	//TODO : Need to put obj spanwing
	Populate();

	SetShaderUniforms();

	PopulateBuffers();
}

BillboardObjectManager::~BillboardObjectManager()
{
	for (const auto& bo : bos)
		for(const auto& b : bo)
			delete b;

	delete[] static_cast<int*>(boFBusageDatas);
	//delete[] herdBoDirAndOffset;


}


Herd* BillboardObjectManager::PopulateObjs(int num, int obj, glm::vec3 pos, 
	float offset)
{
	BillboardAnimatingDatas* data = boManager->boDatas[obj];

	const int animationCount = static_cast<int>(data->obj->animationModels.size());

	herdCount++;

	Buffer* posBuffer = GetHerdPosBuffer(num, pos, offset);
	glm::vec4* posData = posBuffer->GetData<glm::vec4>();

	for (int i = 0; i < num; ++i)
	{
		const int animationIndex = rand() % animationCount;
		const int timeDiffSlot = rand() % data->diffTimeAnimCount;

		const glm::vec3& position = glm::vec3(posData[i]);

		bos[obj].push_back(new BillBoardObject(boShader,
			position, data->frameBuffers[timeDiffSlot][animationIndex]));

		posDatas.emplace_back(position, 1.f);

	}

	delete[] posData;

	posOffset += num;

	herdOffset.push_back(totalRenderingAmount);
	totalRenderingAmount += num;

	Herd* herd = new Herd();

	herd->SetPosBuffer(posBuffer);


	return herd;
}

void BillboardObjectManager::Populate()
{
	herds.push_back(PopulateObjs(1280, static_cast<int>(ObjKind::SWAT), glm::vec3(0.f, 12.f, -20.f), 20.f));
	herds.push_back(PopulateObjs(1280, static_cast<int>(ObjKind::AMY), glm::vec3(-400.f, 12.f, -20.f), 20.f));
	//PopulateObjs(2560, static_cast<int>(ObjKind::KNIGHT));
	//PopulateObjs(2560, static_cast<int>(ObjKind::MICHELLE));
	//PopulateObjs(2560, static_cast<int>(ObjKind::ADAM));

	SetPositionOffsetBuffers({
		glm::vec3(-1.f, 0.f, 0.f), 
		glm::vec3(1.f, 0.f, 0.f)});
}

void BillboardObjectManager::CheckFrameBufferUsage()
{
	boFBusageComputeShader->Use();

	csBuffers->BindBuffers();

	boFBusageComputeShader->SendUniformValues();

	glDispatchCompute(totalRenderingAmount / 128, 1, 1);
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	glUseProgram(0);

	csBuffers->GetData(1, boFBusageDatas);
	assert(boFBusageDatas != nullptr);
	SetBosFrameBufferIndex();
}

void BillboardObjectManager::SetBosFrameBufferIndex()
{
	const size_t bosSize = bos.size();

	int bufIndex = 0;

	for (size_t i = 0; i < bosSize; ++i)
	{
		std::vector<BillBoardObject*>& bo = bos[i];

		const size_t boSize = bo.size();

		for (size_t j = 0; j < boSize; ++j)
		{
			const int usingFbIndex = static_cast<int*>(boFBusageDatas)[bufIndex];

			if (usingFbIndex >= 0)
				bo[j]->SetFrameBufferIndex(usingFbIndex);
			else
				bo[j]->usingFrameBuffer = nullptr;

			bufIndex++;
		}
	}
}
void BillboardObjectManager::Render(const glm::mat4& projMat, const glm::mat4& viewMat)
{
	for (const auto& bo : bos)
		for(const auto& b : bo)
			b->Render(projMat, viewMat);
}

void BillboardObjectManager::SetShaderUniforms()
{
	boFBusageComputeShader->AddUniformValues("camPos", ShaderValueType::Vec3, &currentCam->Position);
	boFBusageComputeShader->AddUniformValues("camFront", ShaderValueType::Vec3, &currentCam->Front);
	boFBusageComputeShader->AddUniformValues("camRight", ShaderValueType::Vec3, &currentCam->Right);
	boFBusageComputeShader->AddUniformValues("camUp", ShaderValueType::Vec3, &currentCam->Up);
	boFBusageComputeShader->AddUniformValues("aspect", ShaderValueType::Float, &currentCam->fov);
	boFBusageComputeShader->AddUniformValues("fovY", ShaderValueType::Float, &currentCam->fovY);
	boFBusageComputeShader->AddUniformValues("zNear", ShaderValueType::Float, &zNear);
	boFBusageComputeShader->AddUniformValues("zFar", ShaderValueType::Float, &zFar);
	boFBusageComputeShader->AddUniformValues("bufferSize", ShaderValueType::Int, &totalRenderingAmount);
}

void BillboardObjectManager::SetPositionOffsetBuffers(std::vector<glm::vec3> directions)
{

	for(int i = 0; i < herdCount; ++i)
	{
		Herd* herd = herds[i];

		herd->SetBoDirAndOffset(glm::vec4(directions[i].x,
			directions[i].y,
			directions[i].z,
			static_cast<float>(herdOffset[i])));

		const std::string uName = "herdBoDirectionAndOffsets[" + std::to_string(i) + "]";
		boFBusageComputeShader->AddUniformValues(uName, ShaderValueType::Vec4, &herds[i]->GetHerdBoDirAndOffset());
	}

	boFBusageComputeShader->AddUniformValues("herdCount",ShaderValueType::Int, &herdCount);
}

Buffer* BillboardObjectManager::GetHerdPosBuffer(int num, glm::vec3 pos,
	float offset)
{
	std::vector<glm::vec4> resultVector;
	
	glm::vec4 startPos;
	startPos.x = pos.x;
	startPos.y = pos.y;
	startPos.z = pos.z;
	startPos.w = 1.f;
	const glm::vec4 ogStartPos = startPos; 

	for(int i = 0 ; i < num ; ++i)
	{
		glm::vec4 newPos = startPos;

		if(i % 16 == 0 && i != 0)
		{
			startPos.x = ogStartPos.x;
			startPos.z += offset;
		}
		else
			startPos.x += offset;


		resultVector.push_back(newPos);
	}

	Buffer* resultBuffer = new Buffer(GL_SHADER_STORAGE_BUFFER, resultVector.size() * sizeof(glm::vec4),
	                                  GL_DYNAMIC_DRAW, resultVector.data(), 0);

	return resultBuffer;
}


void BillboardObjectManager::PopulateBuffers()
{
	csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * static_cast<int>(posDatas.size()),
	GL_DYNAMIC_DRAW, posDatas.data(), 0));

	csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(int) * totalRenderingAmount, GL_DYNAMIC_DRAW,
		nullptr, 1));

	boFBusageDatas = new int[csBuffers->GetBufferSize(1) / sizeof(int)];
}

std::vector<Texture*> BillboardObjectManager::GetTextures(const std::vector<BillboardAnimatingDatas*>& boDatas)
{
	std::vector<Texture*> result;

	const size_t boDatasSize = boDatas.size();

	for(size_t i = 0; i < boDatasSize; ++i)
	{
		BillboardAnimatingDatas* boData = boDatas[i];

		std::vector<std::vector<std::vector<FrameBuffer*>>>& fbs = boData->frameBuffers;
		const size_t fbsSize = fbs.size();

		for(size_t j = 0; j < fbsSize; ++j)
		{
			std::vector<std::vector<FrameBuffer*>>& fbs2 = fbs[j];

			const size_t fbs2Size = fbs2.size();

			for(size_t k = 0; k < fbs2Size; ++k)
			{
				std::vector<FrameBuffer*>& fbs3 = fbs2[k];

				const size_t fbs3Size = fbs3.size();

				for(size_t l = 0; l < fbs3Size; ++l)
				{
					result.push_back(fbs3[l]->texture);
				}
			}
		}
	}

	return result;
}
