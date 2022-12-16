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
#include <vector>
#include "glm/mat4x4.hpp"

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
	void ResetFrameBufferWidthHeight(int w, int h);
	Camera* GetBoObjCamera(int index);
	BillboardAnimatingDatas* GetAnimData(int index);

	std::vector<BillboardAnimatingDatas*> boDatas;
	std::map<std::string, MeshDatas*> meshDatas;

private:
	void GenerateBillboard(const std::chrono::system_clock::time_point& current
		,const glm::mat4& projMat, BillboardAnimatingDatas* datas) const;

	Shader* boShader;
	Shader* boComputeShader;
	int windowW, windowH;

};
