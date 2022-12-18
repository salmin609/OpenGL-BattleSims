/*
 *	File for GAM400 - OpenGL MAs project.
 *	Term			- 2022F
 *	Author			- Ryan Kim
 *	Copyright � 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#pragma once
#include <chrono>
#include <map>
#include <string>
#include <vector>
#include "glm/mat4x4.hpp"

class AnimationModel;
class Floor;
class MeshDatas;
class Texture;
class Camera;
class Shader;
class BillboardAnimatingDatas;

class BillboardManager
{
public:
	BillboardManager(Shader* boShader_, Shader* boComputeShader_, int windowWidth, int windowHeight,
		const std::vector<std::string>& objPaths);
	~BillboardManager();
	void PopulateBoDatas(const std::vector<std::string>& objPaths);
	void GenBillboard(const glm::mat4& projMat);
	void ChangeAnimationIndexByTime();
	Camera* GetBoObjCamera(int index);
	BillboardAnimatingDatas* GetAnimData(int index);


	std::vector<BillboardAnimatingDatas*> boDatas;
	std::map<std::string, MeshDatas*> meshDatas;
	std::vector<std::vector<AnimationModel*>> animModels;

private:
	void GenerateBillboard(const std::chrono::system_clock::time_point& current
		,const glm::mat4& projMat, BillboardAnimatingDatas* datas) const;
	float GetAnimationTimeTicks(const std::chrono::system_clock::time_point& current,
		const std::chrono::system_clock::time_point& modelStartTime,
		int index, float ticksPerSecond, float duration) const;
	Camera* boCamera;

	Shader* boShader;
	Shader* boComputeShader;
	int windowW, windowH;

};
