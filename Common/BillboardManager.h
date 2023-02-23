/*
 *	File for GAM400 - OpenGL MAs project.
 *	Term			- 2022F
 *	Author			- Ryan Kim
 *	Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#pragma once
#include <chrono>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include "AnimationState.h"
#include "glm/mat4x4.hpp"

class AnimationState;
class FrameBuffer;
struct aiAnimation;
class AnimationModel;
class Floor;
class MeshDatas;
class Texture;
class Camera;
class Shader;
class BillboardAnimatingDatas;

enum class CamVectorOrder
{
	Front = 0,

	Back, 
	Left,
	Right,

	LeftFront,
	RightFront,

	LeftBack,
	RightBack,
	End,
};


class BillboardManager
{
public:


	BillboardManager(Shader* boShader_, Shader* boComputeShader_, 
		int windowWidth, int windowHeight,
		const std::vector<std::string>& objPaths);
	~BillboardManager();
	void PopulateBoDatas(const std::vector<std::string>& objPaths);
	void GenBillboard(const glm::mat4& projMat);
	void SaveAnimation(std::vector<AnimationModel*> animations, State state,
		const std::chrono::system_clock::time_point& current,
		const std::vector<std::vector<std::vector<FrameBuffer*>>>& frameBuffers,
		const glm::mat4& projMat, const glm::mat4& modelMat,
		AnimationModel* baseModel,
		int& fbSlotIndex,
		int& usingFrameBufferCount);
	void CheckAnimationPlayingStatus();
	void ChangeStatus(std::vector<AnimationModel*> animations);
	
	Camera* GetBoObjCamera(int camIndex);
	BillboardAnimatingDatas* GetAnimData(int index);

	std::vector<BillboardAnimatingDatas*> boDatas;
	std::map<std::string, MeshDatas*> meshDatas;
	std::unordered_map<std::string, AnimationState*> animModels;

private:
	void GenerateBillboard(const std::chrono::system_clock::time_point& current
		,const glm::mat4& projMat, BillboardAnimatingDatas* datas);
	void SaveAngleTextures(BillboardAnimatingDatas* datas);

	std::vector<Camera*> boCams;
	std::vector<glm::mat4> boCamMats;
	Shader* boShader;
	Shader* boComputeShader;
	int windowW, windowH;

	FrameBuffer* check{};
	bool once = true;

};
