#include "HerdManager.h"

#include <iostream>

#include "BillboardAnimatingDatas.h"
#include "BillboardManager.h"
#include "BillBoardObject.h"
#include "Buffer.hpp"
#include "CSBufferNames.h"
#include "Herd.h"
#include "ModelKinds.hpp"
#include "MultipleAnimationObject.h"

HerdManager::HerdManager(BillboardManager* boManager_, Shader* boShader_)
{
	totalRenderingAmount = 0;
	herdCount = 0;
	boManager = boManager_;
	boShader = boShader_;

	AddHerd(PopulateHerd(256, static_cast<int>(ObjKind::SWAT), glm::vec3(200.f, 12.f, -20.f), 15.f,
		glm::vec4(-1.f, 0.f, 0.f, 1.f), 0));
	AddHerd(PopulateHerd(256 * 3, static_cast<int>(ObjKind::KNIGHT), glm::vec3(-200.f, 12.f, -20.f), 15.f,
		glm::vec4(1.f, 0.f, 0.f, 1.f), 1));
	AddHerd(PopulateHerd(256, static_cast<int>(ObjKind::SWAT), glm::vec3(200.f, 12.f, 300.f), 15.f,
		glm::vec4(-1.f, 0.f, 0.f, 1.f), 0));
	PopulateBuffers();
}

HerdManager::~HerdManager()
{
	for (const auto& herd : herds)
		delete herd;

	delete posBuffer;
	delete directionBuffer;
}

void HerdManager::Render(const glm::mat4& projMat, const glm::mat4& viewMat)
{
	int startIndex = 0;
	posBuffer->GetData(positionDatas.data());
	for (const auto& herd : herds)
	{
		herd->Render(projMat, viewMat, positionDatas.data(), startIndex);
		startIndex += herd->count;
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

Herd* HerdManager::GetHerd(int index)
{
	return herds[index];
}

void HerdManager::GetHerdPositions(int num, glm::vec3 pos, float offset)
{
	glm::vec4 startPos = glm::vec4(pos, 1.f);
	const glm::vec4 ogStartPos = startPos;
	for (int i = 0; i < num; ++i)
	{
		glm::vec4 newPos = startPos;

		startPos.x += offset;
		if (i % 16 == 0 && i != 0)
		{
			startPos.x = ogStartPos.x;
			startPos.z += offset;
		}

		positionDatas.push_back(newPos);
	}
}

void HerdManager::PopulateBuffers()
{
	posBuffer = new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * totalRenderingAmount,
		GL_DYNAMIC_DRAW, positionDatas.data(), ToInt(MoveCS::objsPoses));

	std::vector<glm::vec4> directions;

	for (const Herd* herd : herds)
	{
		for (int i = 0; i < herd->count; ++i)
		{
			directions.push_back(herd->herdDirection);
		}
	}

	directionBuffer = new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * totalRenderingAmount,
		GL_DYNAMIC_DRAW, directions.data(), ToInt(MoveCS::objsDirections));
}

Herd* HerdManager::PopulateHerd(int num, int obj, glm::vec3 pos, float offset, glm::vec4 herdDirection,
	int side)
{
	BillboardAnimatingDatas* data = boManager->boDatas[obj];
	GetHerdPositions(num, pos, offset);

	Herd* herd = new Herd(num, herdDirection, side);

	for (int i = 0; i < num; ++i)
	{
		herd->bos.push_back(new BillBoardObject(boShader,
			&data->frameBuffers[0], data->obj->animState, boObjIndex++));
	}

	return herd;
}

void HerdManager::ChangeAnimationIndicesOfHerd(int* fbAngleIndices, int* animationStateIndices,
	int* isDead)
{
	int bufIndex = 0;

	for(int i = 0; i < herdCount; ++i)
	{
		const Herd* herd = herds[i];

		for(int j = 0; j < herd->count; ++j)
		{
			BillBoardObject* bo = herd->bos[j];

			const int fbAngleIndex = fbAngleIndices[bufIndex];
			//std::cout << "fb : " << fbAngleIndex << std::endl;
			const int dead = isDead[bufIndex];

			if (fbAngleIndex >= 0 && dead == 0)
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
