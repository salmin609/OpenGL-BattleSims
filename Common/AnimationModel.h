/*
 * Author		: Ryan Kim.
 * Date			: 2022-10-07
 * Description	: Class for animating object.
 * Copyright � 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */


#pragma once
#include <chrono>
#include <string>
#include <assimp/Importer.hpp>

#include "AnimatingFunctions.h"
#include "AnimationStructure.hpp"
#include "AnimationModelDatas.h"
#include <map>

struct aiNode;
class Camera;
class Shader;

typedef Assimp::Importer Importer;

class AnimationModel
{
public:
	enum class TextureInfos
	{
		NONE = 0,
		TEXTURED
	};

	AnimationModel(Shader* shaderVal, std::string _filePath, Shader* interpolationShader,
		MeshDatas* reusableMeshDatas);
	~AnimationModel();
	
	void Select();
	void CheckBuffers();
	void InitMaterial();
	void Draw(const glm::mat4& objMat, const glm::mat4& projViewMat,
	          float animationT, int transformsOffset, float instancingOffset,
	          std::vector<glm::mat4> transforms, bool isInstance);
	aiNode* GetRootNode();
	const aiScene* GetScene();
	std::vector<glm::mat4> Interpolate(float animationTimeTicks);
	void PopulateTransforms(std::vector<glm::mat4>& transforms);
	
	AnimationModelDatas* datas;
	std::chrono::system_clock::time_point startTime;

	TextureInfos isTextured = TextureInfos::NONE;

private:
	Importer* importer;
	const aiScene* scene;
	aiNode* rootNode;
	Shader* shader;

	unsigned vao;
	int numVertices, numIndices;
	std::string filePath;
	Shader* interpolationComputeShader;
};
