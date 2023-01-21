#include "BillboardObjectManager.h"

#include <iostream>

#include "BillboardAnimatingDatas.h"
#include "BillboardManager.h"
#include "BillBoardObject.h"
#include "Buffer.hpp"
#include "Camera.hpp"
#include "BufferManager.h"
#include "FrameBuffer.h"
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

	//PopulateObjsPos();
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
	delete[] posOffsets;
}


void BillboardObjectManager::PopulateObjs(int num, int obj, glm::vec3 pos, float offset)
{
	BillboardAnimatingDatas* data = boManager->boDatas[obj];

	const int animationCount = static_cast<int>(data->obj->animationModels.size());

	//std::vector<glm::vec4> poses;

	const std::vector<glm::vec3> herdVec = GetHerdPos(num, pos, offset);

	for (int i = 0; i < num; ++i)
	{
		const int animationIndex = rand() % animationCount;
		const int timeDiffSlot = rand() % data->diffTimeAnimCount;

		const glm::vec3& pos = herdVec[i];

		bos[obj].push_back(new BillBoardObject(boShader,
			pos, data->frameBuffers[timeDiffSlot][animationIndex]));

		//poses.emplace_back(pos, 1.f);
		posDatas.emplace_back(pos, 1.f);

		totalPositionBufferCount++;
	}

	//posDatas.push_back(poses);

	posOffset += num;

	totalRenderingAmount += num;
}

void BillboardObjectManager::Populate()
{
	//PopulateObjs(2, static_cast<int>(ObjKind::SWAT), glm::vec3(0.f, 12.f, -20.f), 20.f);
	PopulateObjs(128, static_cast<int>(ObjKind::SWAT), glm::vec3(0.f, 12.f, -20.f), 20.f);
	PopulateObjs(128, static_cast<int>(ObjKind::AMY), glm::vec3(-400.f, 12.f, -20.f), 20.f);
	//PopulateObjs(2560, static_cast<int>(ObjKind::KNIGHT));
	//PopulateObjs(2560, static_cast<int>(ObjKind::MICHELLE));
	//PopulateObjs(2560, static_cast<int>(ObjKind::ADAM));
}

void BillboardObjectManager::CheckFrameBufferUsage()
{
	boFBusageComputeShader->Use();

	csBuffers->BindBuffers();

	boFBusageComputeShader->SendUniformValues();

	glDispatchCompute(totalPositionBufferCount / 128, 1, 1);
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

float Convert(float radian)
{
	return radian * (180.f / 3.14159f);
}

void BillboardObjectManager::Render(const glm::mat4& projMat, const glm::mat4& viewMat)
{
	//glm::vec3 boDirection = glm::vec3(-1.f, 0.f, 0.f);

	/*int result;

	glm::vec3 camPos = currentCam->Position;
	glm::vec3 boPos = bos[0][0]->pos;

	glm::vec3 boToCam = camPos - boPos;
	float cosTheta = dot(boToCam, boDirection) / (length(boToCam) * length(boDirection));
	float angle = acos(cosTheta);
	float angleInDegree = Convert(angle);


	if (angleInDegree >= 0.f && angleInDegree <= 22.5f)
		result = static_cast<int>(CamVectorOrder::Front);
	else if (angleInDegree >= 157.5f && angleInDegree <= 180.f)
		result = static_cast<int>(CamVectorOrder::Back);
	else
	{
		if (bos[0][0]->pos.z > currentCam->Position.z)
		{
			if (angleInDegree >= 22.5f && angleInDegree <= 67.5f)
				result = static_cast<int>(CamVectorOrder::LeftFront);
			else if (angleInDegree >= 112.5f && angleInDegree <= 157.5f)
				result = static_cast<int>(CamVectorOrder::LeftBack);
			else
				result = static_cast<int>(CamVectorOrder::Left);
		}
		else
		{
			if (angleInDegree >= 22.5f && angleInDegree <= 67.5f)
				result = static_cast<int>(CamVectorOrder::RightFront);
			else if (angleInDegree >= 112.5f && angleInDegree <= 157.5f)
				result = static_cast<int>(CamVectorOrder::RightBack);
			else
				result = static_cast<int>(CamVectorOrder::Right);
		}
	}*/



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
	boFBusageComputeShader->AddUniformValues("bufferSize", ShaderValueType::Int, &totalPositionBufferCount);
}

void BillboardObjectManager::SetPositionOffsetBuffers()
{
	const int boPosDatasCount = static_cast<int>(posDatas.size());
	posOffsets = new int[boPosDatasCount];


	int offset = 0;
	for (int i = 0; i < boPosDatasCount; ++i)
	{
		posOffsets[i] = offset;
		//offset += static_cast<int>(posDatas[i].size());
	}


	for (int i = 0; i < boPosDatasCount; ++i)
	{
		const std::string bufferOffsetUniformName = "posOffset" + std::to_string(i);

		boFBusageComputeShader->AddUniformValues(bufferOffsetUniformName,
			ShaderValueType::Int, &posOffsets[i]);
	}
}

std::vector<glm::vec3> BillboardObjectManager::GetHerdPos(int num, glm::vec3 pos, float offset)
{
	std::vector<glm::vec3> result;
	
	glm::vec3 startPos;
	startPos.x = pos.x;
	startPos.y = pos.y;
	startPos.z = pos.z;
	const glm::vec3 ogStartPos = startPos; 

	for(int i = 0 ; i < num ; ++i)
	{
		glm::vec3 newPos = startPos;

		if(i % 16 == 0 && i != 0)
		{
			startPos.x = ogStartPos.x;
			startPos.z += offset;
		}
		else
			startPos.x += offset;


		result.push_back(newPos);
	}

	return result;
}


void BillboardObjectManager::PopulateBuffers()
{

	csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * static_cast<int>(posDatas.size()),
	GL_DYNAMIC_DRAW, posDatas.data(), 0));

	csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(int) * totalPositionBufferCount, GL_DYNAMIC_DRAW,
		nullptr, 1));

	boFBusageDatas = new int[csBuffers->GetBufferSize(1)];
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
