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

HerdManager::HerdManager(BillboardManager* boManager_, Shader* boShader_,
	Shader* lineShader_)
{
	totalRenderingAmount = 0;
	herdCount = 0;
	boManager = boManager_;
	boShader = boShader_;
	lineShader = lineShader_;
	selectedHerd = nullptr;

	AddHerd(PopulateHerd(256, static_cast<int>(ObjKind::KNIGHT), glm::vec3(400.f, 12.f, -300.f), 15.f,
		glm::vec4(0.f, 0.f, 0.f, 0.f), 0, 16, 50.f));
	AddHerd(PopulateHerd(256, static_cast<int>(ObjKind::KNIGHT), glm::vec3(400.f, 12.f, -20.f), 15.f,
		glm::vec4(0.f, 0.f, 0.f, 0.f), 0, 16, 50.f));
	AddHerd(PopulateHerd(256, static_cast<int>(ObjKind::KNIGHT), glm::vec3(400.f, 12.f, 300.f), 15.f,
		glm::vec4(0.f, 0.f, 0.f, 0.f), 0, 16, 50.f));
	AddHerd(PopulateHerd(256, static_cast<int>(ObjKind::KNIGHT), glm::vec3(400.f, 12.f, 600.f), 15.f,
		glm::vec4(0.f, 0.f, 0.f, 0.f), 0, 16, 50.f));
	AddHerd(PopulateHerd(256, static_cast<int>(ObjKind::SWAT), glm::vec3(800.f, 12.f, -300.f), 15.f,
		glm::vec4(0.f, 0.f, 0.f, 0.f), 0, 16, 50.f));
	AddHerd(PopulateHerd(256, static_cast<int>(ObjKind::SWAT), glm::vec3(800.f, 12.f, -20.f), 15.f,
		glm::vec4(0.f, 0.f, 0.f, 0.f), 0, 16, 50.f));
	AddHerd(PopulateHerd(256, static_cast<int>(ObjKind::SWAT), glm::vec3(800.f, 12.f, 300.f), 15.f,
		glm::vec4(0.f, 0.f, 0.f, 0.f), 0, 16, 50.f));
	AddHerd(PopulateHerd(256, static_cast<int>(ObjKind::SWAT), glm::vec3(800.f, 12.f, 600.f), 15.f,
		glm::vec4(0.f, 0.f, 0.f, 0.f), 0, 16, 50.f));

	AddHerd(PopulateHerd(256 * 3, static_cast<int>(ObjKind::MUTANT), glm::vec3(-800.f, 12.f, -20.f), 25.f,
		glm::vec4(0.f, 0.f, 0.f, 0.f), 1, 32, 25.f));
	


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

		if (herd->selected)
			herd->DrawLine(projMat, viewMat, positionDatas.data(), startIndex);

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

void HerdManager::GetHerdPositions(int num, glm::vec3 pos, float offset, int herdWidth)
{
	glm::vec4 startPos = glm::vec4(pos, 1.f);
	const glm::vec4 ogStartPos = startPos;
	for (int i = 0; i < num; ++i)
	{
		glm::vec4 newPos = startPos;

		startPos.x += offset;
		if (i % herdWidth == 0 && i != 0)
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
			if(herd->side == 0)
			{
				directions.emplace_back(-1.f, 0.f, 0.f, 1.f);
			}
			else
			{
				directions.emplace_back(1.f, 0.f, 0.f, 1.f);
			}
			//directions.push_back(herd->herdDirection);
		}
	}

	directionBuffer = new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * totalRenderingAmount,
		GL_DYNAMIC_DRAW, directions.data(), ToInt(MoveCS::objsDirections));
}

Herd* HerdManager::PopulateHerd(int num, int obj, glm::vec3 pos, float offset, glm::vec4 herdDirection,
	int side, int herdWidth, float speed)
{
	BillboardAnimatingDatas* data = boManager->boDatas[obj];
	GetHerdPositions(num, pos, offset, herdWidth);

	Herd* herd = new Herd(num, herdDirection, side, lineShader, herdWidth, speed);

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

void HerdManager::SelectHerd(int herdIndex)
{
	if (selectedHerd != nullptr)
		selectedHerd->selected = false;

	selectedHerd = herds[herdIndex];
	selectedHerd->selected = true;
}

void HerdManager::ChangeHerdDirection(glm::vec4 direction)
{
	if(selectedHerd != nullptr)
	{
		selectedHerd->herdDirection = direction;
		selectedHerd->selected = false;
		selectedHerd = nullptr;
	}
}
