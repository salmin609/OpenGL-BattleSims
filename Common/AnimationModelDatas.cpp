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
#include "BufferManager.h"
#include "DataTypes.h"
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
	delete storage;
	delete animInfoBuffers;
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

	animInfoBuffers = new BufferManager();

	std::vector<BufferDataType> datas{
		BufferDataType{static_cast<int>(sizeof(int) * storage->bones.size()), storage->bones.data()},
		BufferDataType{static_cast<int>(sizeof(float) * storage->weights.size()), storage->weights.data()},
		BufferDataType{static_cast<int>(sizeof(int) * storage->indexStarts.size()), storage->indexStarts.data()},
		BufferDataType{static_cast<int>(sizeof(int) * storage->indexEnds.size()), storage->indexEnds.data()},
		BufferDataType{static_cast<int>(sizeof(glm::mat4) * boneInfos.size()), nullptr},
	};

	const size_t datasSize = datas.size();

	for(size_t i = 0; i < datasSize; ++i)
	{
		BufferDataType data = datas[i];
		animInfoBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER,
			data.size, GL_STATIC_DRAW, data.data, static_cast<int>(i)));
	}

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

	csBuffers = new BufferManager();

	std::vector<BufferDataType> datas{
		BufferDataType{static_cast<int>(sizeof(glm::mat4) * nodeTransforms.size()), nodeTransforms.data()},
		BufferDataType{static_cast<int>(sizeof(glm::mat4) * nodeTransforms.size()), nullptr},
		BufferDataType{static_cast<int>(sizeof(int) * orderIn1DArray.size()), orderIn1DArray.data()},
		BufferDataType{static_cast<int>(sizeof(int) * orderStartIndex.size()), orderStartIndex.data()},
		BufferDataType{static_cast<int>(sizeof(int) * orderEndIndex.size()), orderEndIndex.data()},
		BufferDataType{static_cast<int>(sizeof(int) * nodeContainAnimation.size()), nodeContainAnimation.data()},
		BufferDataType{static_cast<int>(sizeof(glm::vec4) * scalingIn1DArray.size()), scalingIn1DArray.data()},
		BufferDataType{static_cast<int>(sizeof(glm::vec4) * translationIn1DArray.size()), translationIn1DArray.data()},
		BufferDataType{static_cast<int>(sizeof(glm::vec4) * rotationIn1DArray.size()), rotationIn1DArray.data()},
		BufferDataType{static_cast<int>(sizeof(int) * keyFactorStartIndex.size()), keyFactorStartIndex.data()},
		BufferDataType{static_cast<int>(sizeof(int) * keyFactorEndIndex.size()), keyFactorEndIndex.data()},
		BufferDataType{static_cast<int>(sizeof(float) * keyFactorTimeStampIn1DArray.size()), keyFactorTimeStampIn1DArray.data()},
		BufferDataType{static_cast<int>(sizeof(glm::mat4) * offsetMatrixes.size()), offsetMatrixes.data()},
		BufferDataType{static_cast<int>(sizeof(int) * containOffsetMatrixes.size()), containOffsetMatrixes.data()},
		BufferDataType{static_cast<int>(sizeof(int) * boneIndexes.size()), boneIndexes.data()},
		BufferDataType{static_cast<int>(sizeof(glm::mat4) * boneInfos.size()), nullptr}
	};

	const size_t datasSize = datas.size();

	for(size_t i = 0; i < datasSize; ++i)
	{
		BufferDataType dataType = datas[i];
		csBuffers->AddBuffer(new Buffer(GL_SHADER_STORAGE_BUFFER, dataType.size, GL_DYNAMIC_DRAW, dataType.data,
			static_cast<int>(i)));
	}
}

void AnimationModelDatas::Bind()
{
	meshDatas->Bind();
	animInfoBuffers->BindBuffers();
}
