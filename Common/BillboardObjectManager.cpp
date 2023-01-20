#include "BillboardObjectManager.h"

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
	csBuffers = new BufferManager();
	glGenVertexArrays(1, &vao);
	glBindVertexArray(0);

	for (int i = 0; i < static_cast<int>(ObjKind::END); ++i)
	{
		std::vector<BillBoardObject*> bo;
		bos.push_back(bo);
	}

	PopulateObjsPos();
	Populate();

	const int boPosDatasCount = static_cast<int>(posDatas.size());
	posOffsets = new int[boPosDatasCount];

	int offset = 0;
	for(int i = 0; i < boPosDatasCount; ++i)
	{
		posOffsets[i] = offset;
		offset += static_cast<int>(posDatas[i].size());
	}

	boFBusageComputeShader = boFrameBufferUsageComputeShader;
	boFBusageComputeShader->AddUniformValues("camPos", Shader::ShaderValueType::Vec3, &currentCam->Position);
	boFBusageComputeShader->AddUniformValues("camFront", Shader::ShaderValueType::Vec3, &currentCam->Front);
	boFBusageComputeShader->AddUniformValues("camRight", Shader::ShaderValueType::Vec3, &currentCam->Right);
	boFBusageComputeShader->AddUniformValues("camUp", Shader::ShaderValueType::Vec3, &currentCam->Up);
	boFBusageComputeShader->AddUniformValues("aspect", Shader::ShaderValueType::Float, &currentCam->fov);
	boFBusageComputeShader->AddUniformValues("fovY", Shader::ShaderValueType::Float, &currentCam->fovY);
	boFBusageComputeShader->AddUniformValues("zNear", Shader::ShaderValueType::Float, &zNear);
	boFBusageComputeShader->AddUniformValues("zFar", Shader::ShaderValueType::Float, &zFar);
	boFBusageComputeShader->AddUniformValues("bufferSize", Shader::ShaderValueType::Int, &totalPositionBufferCount);

	for(int i = 0; i < boPosDatasCount; ++i)
	{
		const std::string bufferOffsetUniformName = "posOffset" + std::to_string(i);

		boFBusageComputeShader->AddUniformValues(bufferOffsetUniformName, 
			Shader::ShaderValueType::Int, &posOffsets[i]);
	}

	boFBusageDatas = new int[csBuffers->GetBufferSize(5)];
}

BillboardObjectManager::~BillboardObjectManager()
{
	for (const auto& bo : bos)
		for(const auto& b : bo)
			delete b;

	delete[] boFBusageDatas;
	delete[] posOffsets;
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
			pos, data->frameBuffers[timeDiffSlot][animationIndex]));

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

	for (int i = 0; i < static_cast<int>(ObjKind::END); ++i)
		csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * static_cast<int>(posDatas[i].size()),
			GL_DYNAMIC_DRAW, posDatas[i].data(), i));

	csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(int) * totalPositionBufferCount, GL_DYNAMIC_DRAW,
		nullptr, 5));
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

	csBuffers->GetData(5, boFBusageDatas);
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
