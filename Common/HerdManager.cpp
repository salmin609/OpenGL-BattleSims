/*
 * Author		: Ryan Kim.
 * Date			: 2023-03-17
 * Copyright � 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#include "HerdManager.h"

#include <iostream>

#include "BillboardAnimatingDatas.h"
#include "BillboardManager.h"
#include "BillBoardObject.h"
#include "Buffer.hpp"
#include "CSBufferNames.h"
#include "Herd.h"
#include "MultipleAnimationObject.h"
#include "ModelKinds.h"

HerdManager::HerdManager(BillboardManager* boManager_, Shader* boShader_,
                         Shader* lineShader_)
{
	totalRenderingAmount = 0;
	herdCount = 0;
	boManager = boManager_;
	boShader = boShader_;
	lineShader = lineShader_;
	selectedHerd = nullptr;

	int num = 512;
	int width = 32;
	float speed = 100.f;

	glm::vec3 initialPos = glm::vec3(250.f, 12.f, -300.f);
	glm::vec3 initialPos2 = glm::vec3(-250.f, 12.f, -300.f);

	std::vector<int> objKind1{ static_cast<int>(ObjKind::KNIGHT) , static_cast<int>(ObjKind::SWAT) , static_cast<int>(ObjKind::VIKING), static_cast<int>(ObjKind::ARCHER),
	static_cast<int>(ObjKind::KNIGHT) , static_cast<int>(ObjKind::SWAT) , static_cast<int>(ObjKind::VIKING), static_cast<int>(ObjKind::ARCHER) };
	std::vector<int> objKind2{ static_cast<int>(ObjKind::MUTANT) , static_cast<int>(ObjKind::ZOMBIE) , static_cast<int>(ObjKind::MUTANT), static_cast<int>(ObjKind::ZOMBIE) ,
	static_cast<int>(ObjKind::MUTANT) , static_cast<int>(ObjKind::ZOMBIE) , static_cast<int>(ObjKind::MUTANT), static_cast<int>(ObjKind::ZOMBIE) };

	float posOffsetZ = 500.f;
	float posOffsetZ2 = 500.f;

	for(int i = 0; i < objKind1.size(); ++i)
	{
		AddHerd(PopulateHerd(num, objKind1[i], initialPos, 15.f,
		                     glm::vec4(0.f, 0.f, 0.f, 0.f), 0, width, speed, 30.f, static_cast<int>(ObjAttackType::MELEE)));

		initialPos.z += posOffsetZ;

		if (i == (objKind1.size() / 2) - 1)
		{
			initialPos.x += 500.f;
			initialPos.z = -300.f;
		}
	}

	for (int i = 0; i < objKind2.size(); ++i)
	{
		AddHerd(PopulateHerd(num, objKind2[i], initialPos2, 15.f,
			glm::vec4(0.f, 0.f, 0.f, 0.f), 1, width, speed, 30.f, static_cast<int>(ObjAttackType::MELEE)));

		initialPos2.z += posOffsetZ2;

		if (i == (objKind2.size() / 2) - 1)
		{
			initialPos2.x -= 500.f;
			initialPos2.z = -300.f;
		}
	}
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

		herd->UpdateBoundingBox(positionDatas.data(), startIndex);

		if (herd->selected)
			herd->DrawLine(projMat, viewMat);

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

	const int numDivWidth = num / herdWidth;

	for(int i = 0; i < numDivWidth; ++i)
	{
		for(int j = 0; j < herdWidth; ++j)
		{
			glm::vec4 newPos = startPos;
			positionDatas.push_back(newPos);
			startPos.z += offset;
		}
		startPos.z = ogStartPos.z;
		startPos.x += offset;
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
		}
	}

	directionBuffer = new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * totalRenderingAmount,
		GL_DYNAMIC_DRAW, directions.data(), ToInt(MoveCS::objsDirections));
}

Herd* HerdManager::PopulateHerd(int num, int obj, glm::vec3 pos, float offset, glm::vec4 herdDirection,
	int side, int herdWidth, float speed, float attackRange, int attackType)
{
	BillboardAnimatingDatas* data = boManager->boDatas[obj];
	GetHerdPositions(num, pos, offset, herdWidth);

	Herd* herd = new Herd(num, herdDirection, side, lineShader, herdWidth, speed, attackRange, attackType);

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



void HerdManager::CheckPicking(glm::vec3 pos)
{
	int herdIndex = 0;
	for(Herd* herd : herds)
	{
		std::vector<glm::vec3> bounding = herd->boundingBoxes;
		if((bounding[0].z < pos.z && pos.z < bounding[1].z) &&
			(bounding[0].x < pos.x && pos.x < bounding[2].x) &&
			(bounding[2].z < pos.z && pos.z < bounding[3].z) &&
			(bounding[1].x < pos.x && pos.x < bounding[3].x))
		{
			SelectHerd(herdIndex);
		}
		herdIndex++;
	}
}

void HerdManager::ForwardSelectedHerdToPos(glm::vec3 pos)
{
	if(selectedHerd != nullptr)
	{
		//calc mid pos
		std::vector<glm::vec3> boundingBoxes = selectedHerd->boundingBoxes;

		glm::vec3 midPos;
		midPos.x = (boundingBoxes[0].x + boundingBoxes[2].x) / 2.f;
		midPos.z = (boundingBoxes[0].z + boundingBoxes[1].z) / 2.f;
		midPos.y = boundingBoxes[0].y;

		glm::vec3 temp = pos - midPos;
		temp.y = 0.f;

		float tempX = abs(temp.x);
		float tempZ = abs(temp.z);

		if (tempX > tempZ)
			temp.z = 0.f;
		else 
			temp.x = 0.f;

		selectedHerd->herdDirection = glm::vec4(temp, 0.f);
		selectedHerd->herdDestination = glm::vec4(pos, 1.f);
		selectedHerd->selected = false;
		selectedHerd = nullptr;
	}
}
