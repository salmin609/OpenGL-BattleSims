/*
 * Author		: Ryan Kim.
 * Date			: 2022-10-07
 * Description	: Functions for read / parse / load animating object
 * Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#pragma once

#include <string>
#include <vector>
#include <string>
#include <assimp/mesh.h>
#include "AnimationStructure.hpp"


struct aiBone;
class Material;
struct aiMaterial;
struct aiNodeAnim;
struct aiAnimation;
struct aiNode;
struct aiScene;
struct AnimationModel;
struct Texture;

namespace AnimatingFunctions
{
	glm::mat4 mat4_cast(const aiMatrix4x4& value);
	glm::mat4 mat4_cast(const aiMatrix3x3& value);

	namespace MeshInitializing
	{
		void InitAllMeshes(AnimationModel* model);
		void InitSingleMesh(aiMesh* mesh, unsigned meshIndex, AnimationModel* model);
	}
	namespace MaterialInitializing
	{
		struct MaterialInfos
		{
			std::string path;
			Texture* texture;
		};
		//static std::map<std::string, Texture*> diffuseTextures;
		void InitMaterials(const std::string filename, AnimationModel* model);
		void LoadTextures(const aiMaterial* material, const std::string filename, int index, AnimationModel* model);
		void LoadDiffuseTexture(const aiMaterial* material, const std::string filename, int index, AnimationModel* model);
		//void LoadSpecularTexture(const aiMaterial* material, const std::string filename, int index, AnimationModel* model);
		//void LoadColors(const aiMaterial* material, int index, AnimationModel* model);

		bool TextureExist(const std::string& filename, int& index);
	}
	namespace BoneLoading
	{
		void InitAllBones(AnimationModel* model);
		void LoadMeshBones(aiMesh* mesh, unsigned meshIndex, AnimationModel* model);
		void LoadSingleBone(aiBone* bone, unsigned meshIndex, AnimationModel* model);
		int GetBoneID(const aiBone* bone, AnimationModel* model);
	}
	namespace AnimationMatrix
	{
		void PrintMatrix(glm::mat4 a, std::string name);
		void GetBoneTransforms(std::vector<glm::mat4>& transforms, float timeInSeconds, const aiScene* scene, AnimationModel* model, unsigned animationIndex);
		void ReadNodeHierarchy(const aiNode* node, const glm::mat4& parentTransform, float animationTimeTicks, const aiScene* scene, AnimationModel* model, int animationIndex,
			int& nodeIndex);
		aiNodeAnim* FindNodeAnimation(const aiAnimation* pAnimation, const std::string& nodeName);

		void GetNodeTransforms(std::vector<glm::mat4>& nodeTransforms, const aiScene* scene,
			std::vector<std::vector<int>>& transformOrder, std::vector<int>& nodeContainAnimation,
			std::vector<std::vector<glm::vec4>>& scalingFactor, std::vector<std::vector<glm::vec4>>& translationFactor,
			std::vector<std::vector<glm::vec4>>& rotationFactor,
			std::vector<std::vector<float>>& factorTimeStamps,
			std::vector<glm::mat4>& offsetMatrixes,
			std::vector<int>& containOffsetMatrixes,
			std::vector<int>& boneIndexes, AnimationModel* model);

		void ReadNodeHierarchyNodeTransforms(const aiNode* node, const aiScene* scene, std::vector<glm::mat4>& nodeTransforms,
			int& transformIndex, std::vector<std::vector<int>>& transformOrder, std::vector<int> prevOrder
			, std::vector<int>& nodeContainAnimation, std::vector<std::vector<glm::vec4>>& scalingFactor, std::vector<std::vector<glm::vec4>>& translationFactor,
			std::vector<std::vector<glm::vec4>>& rotationFactor,
			std::vector<std::vector<float>>& factorTimeStamps,
			std::vector<glm::mat4>& offsetMatrixes,
			std::vector<int>& containOffsetMatrixes,
			std::vector<int>& boneIndexes, AnimationModel* model);

		std::vector<glm::mat4> CheckNodeTransforms(const aiScene* scene, std::vector<glm::mat4> finalTransforms, AnimationModel* model);
		void CheckNodeHierarchyTransforms(const aiNode* node, const aiScene* scene, std::vector<glm::mat4> finalTransforms, AnimationModel* model,
			int& transformIndex, const glm::mat4& parentTransform);
	}
}
