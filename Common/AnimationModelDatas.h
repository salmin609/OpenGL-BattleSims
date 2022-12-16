/*
 * Author		: Ryan Kim.
 * Date			: 2022-10-07
 * Description	: Class for storing all informations of animating object.
 * Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#pragma once
#include <map>
#include <string>
#include <vector>
#include "Material.h"
#include "VertexBoneData.hpp"
#include "AnimationStructure.hpp"

class MeshDatas;
class AnimationModel;
class BoneStorageManager;
struct aiScene;
class Buffer;

class AnimationModelDatas
{
public:
	AnimationModelDatas(unsigned vao_, MeshDatas* reusableMeshDatas = nullptr);
	~AnimationModelDatas();
	void ReserveVectorSpace();
	void ReserveSpace(const aiScene* scene);
	void PopulateBuffers();
	void PopulateTransforms(std::vector<glm::mat4>& transforms);
	void PopulateInterpolationShaderBuffer(const aiScene* scene, AnimationModel* model);
	void Bind();
	void BindInterpolationBuffer();


	std::vector<glm::mat4> nodeTransforms;
	std::vector<std::vector<int>> transformOrder;
	bool needNewMesh = true;

	std::vector<BoneInfo> boneInfos;
	std::vector<VertexBoneData> bones;

	std::map<std::string, uint> boneName2IndexMap;
	std::vector<BasicMeshEntry> meshes;
	std::string materialPath;
	Texture* diffuseTexture = nullptr;

	Buffer* gBonesBuffer;
	MeshDatas* meshDatas;

	Buffer* ssboBonesBuffer;
	Buffer* ssboWeightsBuffer;
	Buffer* ssboIndexStartsBuffer;
	Buffer* ssboIndexEndsBuffer;
	Buffer* ssboTransformsBuffer;

	Buffer* computeNodeTransformInBuffer;
	Buffer* computeNodeTransformOutBuffer;
	Buffer* computeTotalOrderBuffer;
	Buffer* computeOrderStartBuffer;
	Buffer* computeOrderEndBuffer;
	Buffer* computeBoolAnimationBuffer;

	Buffer* computeTotalScalingKeysBuffer;
	Buffer* computeTotalTranslationKeysBuffer;
	Buffer* computeTotalRotationKeysBuffer;
	Buffer* computeKeyFactorsStartIndexBuffer;
	Buffer* computeKeyFactorsEndIndexBuffer;

	Buffer* computeKeyFactorsTimeStampBuffer;

	Buffer* computeOffsetMatrixesBuffer;
	Buffer* computeNodeContainOffsetMatrix;
	Buffer* computeBoneIndexes;
	Buffer* computeOutFinalTransforms;


	int numVertices, numIndices;
	BoneStorageManager* storage;
	unsigned vao;
private:

};
