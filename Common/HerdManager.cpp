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

	AddHerd(PopulateHerd(1280, static_cast<int>(ObjKind::SWAT), glm::vec3(400.f, 12.f, -20.f), 20.f,
		glm::vec3(-1.f, 0.f, 0.f)));

	AddHerd(PopulateHerd(1280, static_cast<int>(ObjKind::KNIGHT), glm::vec3(-400.f, 12.f, -20.f), 20.f,
		glm::vec3(1.f, 0.f, 0.f)));
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

	glm::vec4 startPos;
	startPos.x = pos.x;
	startPos.y = pos.y;
	startPos.z = pos.z;
	startPos.w = 1.f;
	const glm::vec4 ogStartPos = startPos;

	for (int i = 0; i < num; ++i)
	{
		glm::vec4 newPos = startPos;

		if (i % 16 == 0 && i != 0)
		{
			startPos.x = ogStartPos.x;
			startPos.z += offset;
		}
		else
			startPos.x += offset;


		resultVector.push_back(newPos);
	}

	return new Buffer(GL_SHADER_STORAGE_BUFFER,
		static_cast<int>(resultVector.size()) * static_cast<int>(sizeof(glm::vec4)),
		GL_DYNAMIC_DRAW, resultVector.data(), posBufferIndex++);
}

Herd* HerdManager::PopulateHerd(int num, int obj, glm::vec3 pos, float offset, glm::vec3 boDirection)
{
	const BillboardAnimatingDatas* data = boManager->boDatas[obj];

	const int animationCount = static_cast<int>(data->obj->animationModels.size());

	Buffer* posBuffer = GetHerdPositionBuffer(num, pos, offset);
	const glm::vec4* posData = posBuffer->GetData<glm::vec4>();

	Herd* herd = new Herd(num);

	herd->posBuffer = posBuffer;
	herd->herdBoDirAndOffset = glm::vec4{ boDirection.x, boDirection.y, boDirection.z, 1.f };

	for (int i = 0; i < num; ++i)
	{
		//const int animationIndex = rand() % animationCount;
		//const int animationIndex = 0;
		//const int timeDiffSlot = rand() % data->diffTimeAnimCount;

		const glm::vec3& position = glm::vec3(posData[i]);

		//Setting different times per animation consumes lots of fps.
		herd->bos.push_back(new BillBoardObject(boShader,
			position, data->frameBuffers[0]));
	}

	delete[] posData;
	return herd;
}

void HerdManager::SetBosFrameBufferIndex(void* boFBusageDatas)
{
	int bufIndex = 0;

	for (int i = 0; i < herdCount; ++i)
	{
		const Herd* herd = herds[i];

		for (int j = 0; j < herd->count; ++j)
		{
			BillBoardObject* bo = herd->bos[j];
			const int usingFbIndex = static_cast<int*>(boFBusageDatas)[bufIndex];

			if (usingFbIndex >= 0)
				bo->ChangeFrameBufferAngle(usingFbIndex);
			else
				bo->usingFrameBuffer = nullptr;

			bufIndex++;
		}
	}
}

void HerdManager::SetReachedAnimation(int* data)
{
	int bufIndex = 0;

	for (int i = 0; i < herdCount; ++i)
	{
		const Herd* herd = herds[i];

		for (int j = 0; j < herd->count; ++j)
		{
			BillBoardObject* bo = herd->bos[j];
			const int isReached = data[bufIndex];

			if (isReached > 0)
				bo->SetAnimation(2);

			bufIndex++;
		}
	}
}

void HerdManager::ChangeToAttackAnimation()
{

}
