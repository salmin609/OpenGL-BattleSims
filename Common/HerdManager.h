/*
 * Author		: Ryan Kim.
 * Date			: 2023-03-17
 * Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#pragma once
#include <vector>

#include "Ray.h"
#include "glm/mat4x4.hpp"

class Shader;
class BillboardManager;
class Buffer;
class Herd;

class HerdManager
{
public:
	HerdManager(BillboardManager* boManager_, Shader* boShader_,
		Shader* lineShader_);
	~HerdManager();
	void Render(const glm::mat4& projMat, const glm::mat4& viewMat);

	void AddHerd(Herd* herd);
	int& GetHerdCount();
	Herd* GetHerd(int index);
	void GetHerdPositions(int num, glm::vec3 pos, float offset, int herdWidth);
	void PopulateBuffers();
	Herd* PopulateHerd(int num, int obj, glm::vec3 pos, float offset, glm::vec4 herdDirection,
		int side, int herdWidth, float speed, float attackRange, int attackType);
	void ChangeAnimationIndicesOfHerd(int* fbAngleIndices, int* animationStateIndices,
		int* isDead);
	void SelectHerd(int herdIndex);
	void ChangeHerdDirection(glm::vec4 direction);
	void CheckPicking(glm::vec3 pos);
	void ForwardSelectedHerdToPos(glm::vec3 pos);

	int totalRenderingAmount;
	std::vector<int> herdOffset;
	int posBufferIndex = 1;
	int boObjIndex = 0;
	std::vector<glm::vec4> positionDatas;
	Buffer* posBuffer{};
	Buffer* directionBuffer{};
	Herd* selectedHerd;
private:
	std::vector<Herd*> herds;
	int herdCount{};
	BillboardManager* boManager;
	Shader* boShader;
	Shader* lineShader;
};
