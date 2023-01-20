/*
 * Author		: Ryan Kim.
 * Date			: 2022-10-07
 * Description	: Class for storing all informations of animating object.
 * Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#include "AnimationModelDatas.h"

#include <assimp/scene.h>
#include <GL/glew.h>

#include "AnimatingFunctions.h"
#include "Buffer.hpp"

#include "BoneStorageManager.h"
#include "ComputeShaderBufferManager.h"
#include "MeshDatas.h"

#include "glm/vec2.hpp"

AnimationModelDatas::AnimationModelDatas(unsigned vao_, MeshDatas* reusableMeshDatas)
{
	vao = vao_;

	if (reusableMeshDatas != nullptr)
		needNewMesh = false;

	if (needNewMesh)
		meshDatas = new MeshDatas();
	else
		meshDatas = reusableMeshDatas;
	
}

AnimationModelDatas::~AnimationModelDatas()
{
	//delete meshDatas;
	delete storage;

	delete ssboBonesBuffer;
	delete ssboWeightsBuffer;
	delete ssboIndexStartsBuffer;
	delete ssboIndexEndsBuffer;
	delete ssboTransformsBuffer;

	//delete computeNodeTransformInBuffer;
	//delete computeNodeTransformOutBuffer;
	//delete computeTotalOrderBuffer;
	//delete computeOrderStartBuffer;
	//delete computeOrderEndBuffer;
	//delete computeBoolAnimationBuffer;
	//delete computeTotalScalingKeysBuffer;
	//delete computeTotalTranslationKeysBuffer;
	//delete computeTotalRotationKeysBuffer;
	//delete computeKeyFactorsStartIndexBuffer;
	//delete computeKeyFactorsEndIndexBuffer;
	//delete computeKeyFactorsTimeStampBuffer;
	//delete computeOffsetMatrixesBuffer;
	//delete computeNodeContainOffsetMatrix;
	//delete computeBoneIndexes;
	//delete computeOutFinalTransforms;
	delete csBuffers;
}

void AnimationModelDatas::ReserveVectorSpace()
{
	if(needNewMesh)
		meshDatas->ReserveSpace(numVertices, numIndices);
	bones.resize(numVertices);
}

void AnimationModelDatas::ReserveSpace(const aiScene* scene)
{
	meshes.resize(scene->mNumMeshes);
	//materials.resize(scene->mNumMaterials);

	const unsigned size = static_cast<unsigned>(meshes.size());

	for (unsigned i = 0; i < size; ++i)
	{
		BasicMeshEntry& mesh = meshes[i];

		mesh.MaterialIndex = scene->mMeshes[i]->mMaterialIndex;
		mesh.NumIndices = scene->mMeshes[i]->mNumFaces * 3;
		mesh.BaseVertex = numVertices;
		mesh.BaseIndex = numIndices;

		numVertices += scene->mMeshes[i]->mNumVertices;
		numIndices += meshes[i].NumIndices;
	}
	ReserveVectorSpace();
}

void AnimationModelDatas::PopulateBuffers()
{
	glBindVertexArray(vao);

	if(needNewMesh)
		meshDatas->PopulateBuffer();

	meshDatas->FeedLayout();

	storage = new BoneStorageManager(bones);

	ssboBonesBuffer = new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(int) * static_cast<int>(storage->bones.size()), GL_STATIC_DRAW, storage->bones.data(), 0);
	ssboBonesBuffer->UnBind();
	ssboWeightsBuffer = new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(float) * static_cast<int>(storage->weights.size()), GL_STATIC_DRAW, storage->weights.data(), 1);
	ssboWeightsBuffer->UnBind();
	ssboIndexStartsBuffer = new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(int) * static_cast<int>(storage->indexStarts.size()), GL_STATIC_DRAW, storage->indexStarts.data(), 2);
	ssboIndexStartsBuffer->UnBind();
	ssboIndexEndsBuffer = new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(int) * static_cast<int>(storage->indexEnds.size()), GL_STATIC_DRAW, storage->indexEnds.data(), 3);
	ssboIndexEndsBuffer->UnBind();

	glBindVertexArray(0);
}

void AnimationModelDatas::PopulateTransforms(std::vector<glm::mat4>& transforms)
{
	const int transformsSize = static_cast<int>(transforms.size());

	glBindVertexArray(vao);

	ssboTransformsBuffer = new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(glm::mat4) * transformsSize, GL_STATIC_DRAW, transforms.data(), 4);

	glBindVertexArray(0);
}

void AnimationModelDatas::PopulateInterpolationShaderBuffer(const aiScene* scene, AnimationModel* model)
{
	std::vector<int> nodeContainAnimation;
	std::vector<std::vector<glm::vec4>> scalingFactor;
	std::vector<std::vector<glm::vec4>> translationFactor;
	std::vector<std::vector<glm::vec4>> rotationFactor;
	std::vector<std::vector<float>> factorTimeStamps;
	std::vector<glm::vec4> scalingIn1DArray;
	std::vector<glm::vec4> translationIn1DArray;
	std::vector<glm::vec4> rotationIn1DArray;
	std::vector<int> keyFactorStartIndex;
	std::vector<int> keyFactorEndIndex;
	std::vector<float> keyFactorTimeStampIn1DArray;
	std::vector<int> orderIn1DArray;
	std::vector<int> orderStartIndex;
	std::vector<int> orderEndIndex;
	std::vector<glm::mat4> offsetMatrixes;
	std::vector<int> containOffsetMatrixes;
	std::vector<int> boneIndexes;

	AnimatingFunctions::AnimationMatrix::GetNodeTransforms(nodeTransforms, scene,
		transformOrder, nodeContainAnimation, scalingFactor, translationFactor, rotationFactor,
		factorTimeStamps, offsetMatrixes, containOffsetMatrixes, boneIndexes, model);

	int index = 0;
	int factorVecSize = static_cast<int>(scalingFactor.size());

	for (int i = 0; i < factorVecSize; ++i)
	{
		std::vector<glm::vec4> scalingKeys = scalingFactor[i];
		std::vector<glm::vec4> translationKeys = translationFactor[i];
		std::vector<glm::vec4> rotationKeys = rotationFactor[i];
		std::vector<float> timeStamps = factorTimeStamps[i];
		int individualVecSize = static_cast<int>(scalingKeys.size());

		keyFactorStartIndex.push_back(index);
		for (int j = 0; j < individualVecSize; ++j)
		{
			glm::vec4 scalingKey = scalingKeys[j];
			glm::vec4 translationKey = translationKeys[j];
			glm::vec4 rotationKey = rotationKeys[j];
			float timeStamp = timeStamps[j];

			scalingIn1DArray.push_back(scalingKey);
			translationIn1DArray.push_back(translationKey);
			rotationIn1DArray.push_back(rotationKey);
			keyFactorTimeStampIn1DArray.push_back(timeStamp);
		}
		index += individualVecSize;

		keyFactorEndIndex.push_back(index);
	}

	const int totalOrderSize = static_cast<int>(transformOrder.size());
	int startIndex = 0;
	for (int i = 0; i < totalOrderSize; ++i)
	{
		std::vector<int> order = transformOrder[i];

		const int orderSize = static_cast<int>(order.size());

		orderStartIndex.push_back(startIndex);
		for (int j = 0; j < orderSize; ++j)
		{
			orderIn1DArray.push_back(order[j]);
		}
		startIndex += orderSize;
		orderEndIndex.push_back(startIndex);
	}
	int size = static_cast<int>(boneInfos.size());
	gBonesBuffer = new Buffer(GL_SHADER_STORAGE_BUFFER, 
		sizeof(glm::mat4) * size, GL_DYNAMIC_DRAW, nullptr, 4);


	csBuffers = new ComputeShaderBufferManager();

	csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(glm::mat4) * static_cast<int>(nodeTransforms.size()), GL_DYNAMIC_DRAW,
		nodeTransforms.data(), 0));

	csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(glm::mat4) * static_cast<int>(nodeTransforms.size()), GL_DYNAMIC_DRAW,
		nullptr, 1));

	csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(int) * static_cast<int>(orderIn1DArray.size()), GL_DYNAMIC_DRAW,
		orderIn1DArray.data(), 2));

	csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(int) * static_cast<int>(orderStartIndex.size()), GL_DYNAMIC_DRAW,
		orderStartIndex.data(), 3));

	csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(int) * static_cast<int>(orderEndIndex.size()), GL_DYNAMIC_DRAW,
		orderEndIndex.data(), 4));

	csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(int) * static_cast<int>(nodeContainAnimation.size()), GL_DYNAMIC_DRAW,
		nodeContainAnimation.data(), 5));

	csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * static_cast<int>(scalingIn1DArray.size()), GL_DYNAMIC_DRAW,
		scalingIn1DArray.data(), 6));

	csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * static_cast<int>(translationIn1DArray.size()), GL_DYNAMIC_DRAW,
		translationIn1DArray.data(), 7));

	csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * static_cast<int>(rotationIn1DArray.size()), GL_DYNAMIC_DRAW,
		rotationIn1DArray.data(), 8));

	csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(int) * static_cast<int>(keyFactorStartIndex.size()), GL_DYNAMIC_DRAW,
		keyFactorStartIndex.data(), 9));

	csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(int) * static_cast<int>(keyFactorEndIndex.size()), GL_DYNAMIC_DRAW,
		keyFactorEndIndex.data(), 10));

	csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(float) * static_cast<int>(keyFactorTimeStampIn1DArray.size()), GL_DYNAMIC_DRAW,
		keyFactorTimeStampIn1DArray.data(), 11));

	csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(glm::mat4) * static_cast<int>(offsetMatrixes.size()), GL_DYNAMIC_DRAW,
		offsetMatrixes.data(), 12));

	csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(int) * static_cast<int>(containOffsetMatrixes.size()), GL_DYNAMIC_DRAW,
		containOffsetMatrixes.data(), 13));

	csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(int) * static_cast<int>(boneIndexes.size()), GL_DYNAMIC_DRAW,
		boneIndexes.data(), 14));

	csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(glm::mat4) * static_cast<int>(boneInfos.size()), GL_DYNAMIC_DRAW,
		nullptr, 15));
}

void AnimationModelDatas::Bind()
{
	meshDatas->Bind();
	ssboBonesBuffer->Bind();
	ssboWeightsBuffer->Bind();
	ssboIndexStartsBuffer->Bind();
	ssboIndexEndsBuffer->Bind();
}

void AnimationModelDatas::BindInterpolationBuffer()
{
	//computeNodeTransformInBuffer->BindStorage();
	//computeNodeTransformOutBuffer->BindStorage();
	//computeTotalOrderBuffer->BindStorage();
	//computeOrderStartBuffer->BindStorage();
	//computeOrderEndBuffer->BindStorage();
	//computeBoolAnimationBuffer->BindStorage();
	//computeTotalScalingKeysBuffer->BindStorage();
	//computeTotalTranslationKeysBuffer->BindStorage();
	//computeTotalRotationKeysBuffer->BindStorage();
	//computeKeyFactorsStartIndexBuffer->BindStorage();
	//computeKeyFactorsEndIndexBuffer->BindStorage();
	//computeKeyFactorsTimeStampBuffer->BindStorage();
	//computeOffsetMatrixesBuffer->BindStorage();
	//computeNodeContainOffsetMatrix->BindStorage();
	//computeBoneIndexes->BindStorage();
	//computeOutFinalTransforms->BindStorage();
	csBuffers->BindBuffers();
}

