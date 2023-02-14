#include "HerdManager.h"
#include "BillboardAnimatingDatas.h"
#include "BillboardManager.h"
#include "BillBoardObject.h"
#include "Buffer.hpp"
#include "Herd.h"
#include "ModelKinds.hpp"
#include "MultipleAnimationObject.h"

HerdManager::HerdManager(BillboardManager* boManager_, Shader* boShader_)
{
	totalRenderingAmount = 0;
	herdCount = 0;
	boManager = boManager_;
	boShader = boShader_;

	AddHerd(PopulateHerd(1280, static_cast<int>(ObjKind::SWAT), glm::vec3(200.f, 12.f, -20.f), 15.f));
	AddHerd(PopulateHerd(1280, static_cast<int>(ObjKind::KNIGHT), glm::vec3(-200.f, 12.f, -20.f), 15.f));
}

HerdManager::~HerdManager()
{
	for (const auto& herd : herds)
		delete herd;
}

void HerdManager::Render(const glm::mat4& projMat, const glm::mat4& viewMat)
{
	for (const auto& herd : herds)
	{
		herd->Render(projMat, viewMat);
	}
}

void HerdManager::AddHerd(Herd* herd)
{
	herds.push_back(herd);
	herdOffset.push_back(totalRenderingAmount);
	totalRenderingAmount += herd->count;
	herdCount = static_cast<int>(herds.size());
}

int& HerdManager::GetHerdCount()
{
	return herdCount;
}

void HerdManager::BindHerdPositions()
{
	for(const auto& herd : herds)
	{
		herd->posBuffer->BindStorage();
	}
}

Herd* HerdManager::GetHerd(int index)
{
	return herds[index];
}

Buffer* HerdManager::GetHerdPositionBuffer(int num, glm::vec3 pos, float offset)
{
	std::vector<glm::vec4> resultVector;

	glm::vec4 startPos = glm::vec4(pos, 1.f);
	const glm::vec4 ogStartPos = startPos;
	//float LO = -2.f;
	//float HI = 2.f;
	for (int i = 0; i < num; ++i)
	{
		//float r3 = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));

		glm::vec4 newPos = startPos;

		startPos.x += offset;
		//startPos.z += r3;
		if (i % 16 == 0 && i != 0)
		{
			startPos.x = ogStartPos.x;
			startPos.z += offset;
		}

		resultVector.push_back(newPos);
	}

	return new Buffer(GL_SHADER_STORAGE_BUFFER,
		static_cast<int>(resultVector.size()) * static_cast<int>(sizeof(glm::vec4)),
		GL_DYNAMIC_DRAW, resultVector.data(), posBufferIndex++);
}

Herd* HerdManager::PopulateHerd(int num, int obj, glm::vec3 pos, float offset)
{
	BillboardAnimatingDatas* data = boManager->boDatas[obj];

	//const int animationCount = static_cast<int>(data->obj->animationModels.size());

	Buffer* posBuffer = GetHerdPositionBuffer(num, pos, offset);

	Herd* herd = new Herd(num);

	herd->posBuffer = posBuffer;
	//herd->herdBoDirAndOffset = glm::vec4{ boDirection.x, boDirection.y, boDirection.z, 1.f };

	for (int i = 0; i < num; ++i)
	{
		//const int animationIndex = rand() % animationCount;
		//const int animationIndex = 0;
		//const int timeDiffSlot = rand() % data->diffTimeAnimCount;
		//Setting different times per animation consumes lots of fps.

		herd->bos.push_back(new BillBoardObject(boShader,
			&data->frameBuffers[0], data->obj->animState, boObjIndex++));

	}

	return herd;
}

void HerdManager::ChangeAnimationIndicesOfHerd(int* fbAngleIndices, int* animationStateIndices)
{
	int bufIndex = 0;

	for(int i = 0; i < herdCount; ++i)
	{
		const Herd* herd = herds[i];

		for(int j = 0; j < herd->count; ++j)
		{
			BillBoardObject* bo = herd->bos[j];

			const int fbAngleIndex = fbAngleIndices[bufIndex];

			if (fbAngleIndex >= 0)
			{
				bo->ChangeFrameBufferAngle(fbAngleIndex);

				const int newState = animationStateIndices[bufIndex];

				bo->SetAnimation(newState, animationStateIndices);
			}
			else
			{
				bo->usingFrameBuffer = nullptr;
			}

			bufIndex++;
		}
	}
}
