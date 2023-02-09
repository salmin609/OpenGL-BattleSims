/*
 * Author		: Ryan Kim.
 * Date			: 2022-10-07
 * Description	: Class for animating object.
 * Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */


#pragma once
#include <chrono>
#include <string>
#include <assimp/Importer.hpp>

#include "AnimatingFunctions.h"
#include "AnimationStructure.hpp"
#include "AnimationModelDatas.h"

struct aiNode;
class Camera;
class Shader;

typedef Assimp::Importer Importer;

class AnimationModel
{
public:
	AnimationModel(Shader* shaderVal, std::string _filePath, Shader* interpolationShader,
		MeshDatas* reusableMeshDatas);
	~AnimationModel();
	
	void Select();
	void CheckBuffers();
	void InitMaterial();
	void Draw(const glm::mat4& objMat, const glm::mat4& projViewMat,
		glm::mat4* transforms);
	aiNode* GetRootNode() const;
	const aiScene* GetScene() const;
	glm::mat4* Interpolate(float animationTimeTicks) const;
	void SetInitialBoneTransformData();
	AnimationModelDatas* datas;
	std::chrono::system_clock::time_point startTime;
	glm::mat4x4* boneTransformsData;
	glm::mat4x4* initialBoneTransformsData;

private:
	Importer* importer;
	const aiScene* scene;
	aiNode* rootNode{};
	Shader* shader;

	unsigned vao{};
	int numVertices, numIndices;
	std::string filePath;
	Shader* interpolationComputeShader;
};
