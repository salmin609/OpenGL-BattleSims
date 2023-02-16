/*
 * Author		: Ryan Kim.
 * Date			: 2022-10-07
 * Description	: Functions for read / parse / load animating object
 * Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#include "AnimatingFunctions.h"

#include <iostream>

#include "AnimationModel.h"
#include <assimp/scene.h>
#include <GL/glew.h>
#include <glm/gtx/transform.hpp>

#include "Interpolation.h"
//#include "Material.h"
#include "Texture.h"

#include "Quaternion.h"
#include <glm/gtx/string_cast.hpp>

#include "MeshDatas.h"

namespace AnimatingFunctions
{


	glm::mat4 mat4_cast(const aiMatrix4x4& value)
	{
		glm::mat4 result;
		result[0] = glm::vec4(value[0][0], value[1][0], value[2][0], value[3][0]);
		result[1] = glm::vec4(value[0][1], value[1][1], value[2][1], value[3][1]);
		result[2] = glm::vec4(value[0][2], value[1][2], value[2][2], value[3][2]);
		result[3] = glm::vec4(value[0][3], value[1][3], value[2][3], value[3][3]);

		return result;
	}
	glm::mat4 mat4_cast(const aiMatrix3x3& value)
	{
		glm::mat4 result;
		result[0] = glm::vec4(value[0][0], value[1][0], value[2][0], 0.f);
		result[1] = glm::vec4(value[0][1], value[1][1], value[2][1], 0.f);
		result[2] = glm::vec4(value[0][2], value[1][2], value[2][2], 0.f);
		result[3] = glm::vec4(0.f, 0.f, 0.f, 1.f);

		return result;
	}
	//std::vector<std::string> split(std::string s, std::string delimiter) {
	//	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	//	std::string token;
	//	std::vector<std::string> res;

	//	while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
	//		token = s.substr(pos_start, pos_end - pos_start);
	//		pos_start = pos_end + delim_len;
	//		res.push_back(token);
	//	}

	//	res.push_back(s.substr(pos_start));
	//	return res;
	//}

	

	namespace MeshInitializing
	{
		void InitAllMeshes(AnimationModel* model)
		{
			const unsigned size = static_cast<unsigned>(model->datas->meshes.size());

			for (unsigned i = 0; i < size; ++i)
			{
				aiMesh* pMesh = model->GetScene()->mMeshes[i];
				InitSingleMesh(pMesh, model);
			}
		}
		void InitSingleMesh(aiMesh* mesh, AnimationModel* model)
		{
			const aiVector3D zeroVec(0.0f, 0.0f, 0.0f);

			const unsigned size = mesh->mNumVertices;

			for (unsigned i = 0; i < size; ++i)
			{
				aiVector3D& pos = mesh->mVertices[i];

				model->datas->meshDatas->positions.emplace_back(pos.x, pos.y, pos.z);

				aiVector3D normal, texCoord;

				if (mesh->HasNormals())
					normal = mesh->mNormals[i];
				else
					normal = aiVector3D(0.f, 1.f, 0.f);

				model->datas->meshDatas->normals.emplace_back(normal.x, normal.y, normal.z);

				if (mesh->HasTextureCoords(0))
					texCoord = mesh->mTextureCoords[0][i];
				else
					texCoord = zeroVec;

				model->datas->meshDatas->texCoords.emplace_back(texCoord.x, texCoord.y);
			}

			//BoneLoading::LoadMeshBones(mesh, meshIndex, model);

			for (unsigned i = 0; i < mesh->mNumFaces; ++i)
			{
				const aiFace& face = mesh->mFaces[i];

				model->datas->meshDatas->indices.emplace_back(face.mIndices[0]);
				model->datas->meshDatas->indices.emplace_back(face.mIndices[1]);
				model->datas->meshDatas->indices.emplace_back(face.mIndices[2]);
			}
		}

		
	}
	namespace MaterialInitializing
	{
		std::vector<MaterialInfos> diffuseTextures;
		std::vector<MaterialInfos> specularTextures;
		std::vector<MaterialInfos> normalTextures;

		//std::map<std::string, Texture*> diffuseTextures;
		void InitMaterials(const std::string filename, AnimationModel* model)
		{
			const aiScene* scene = model->GetScene();

			for (unsigned i = 0; i < scene->mNumMaterials; ++i)
			{
				const aiMaterial* mat = scene->mMaterials[i];
				
				LoadTextures(mat, model);
				//LoadColors(mat, i, model);
			}
		}

		void LoadTextures(const aiMaterial* material, AnimationModel* model)
		{
			aiString path;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &path, nullptr, nullptr, nullptr, nullptr, nullptr);
			const std::string diffusePath(path.data);
			int matIndex = 0;

			if(TextureExist(diffusePath, matIndex, diffuseTextures))
			{
				model->datas->diffuseTexture = diffuseTextures[matIndex].texture;
			}
			else
				LoadDiffuseTexture(material, model);

			material->GetTexture(aiTextureType_SPECULAR, 0, &path, nullptr, nullptr, nullptr, nullptr, nullptr);
			const std::string specularPath(path.data);

			if (TextureExist(specularPath, matIndex, specularTextures))
			{
				model->datas->specularTexture = specularTextures[matIndex].texture;
			}
			else
				LoadSpecularTexture(material, model);

			//material->GetTexture(aiTextureType_NORMALS, 0, &path, nullptr, nullptr, nullptr, nullptr, nullptr);
			//const std::string normalPath(path.data);

			//if (TextureExist(normalPath, matIndex, normalTextures))
			//{
			//	model->datas->normalTexture = normalTextures[matIndex].texture;
			//}
			//else
			//	LoadNormalTexture(material, model);
		}

		void LoadDiffuseTexture(const aiMaterial* material, AnimationModel* model)
		{
			const unsigned textureCount = material->GetTextureCount(aiTextureType_DIFFUSE);

			if(textureCount > 0)
			{
				aiString path;

				if(material->GetTexture(aiTextureType_DIFFUSE, 0, &path, nullptr, nullptr, nullptr, nullptr, nullptr) == AI_SUCCESS)
				{
					std::string p(path.data);

					std::string filePath = "../Models/" + p;
					Texture* newDiffuseTexture = new Texture(GL_TEXTURE_2D, filePath);

					if (!newDiffuseTexture->Load())
					{
						std::cout << "Failed to Load diffuse texture" << std::endl;
					}
					else
					{
						std::cout << "Load diffuse texture : " << filePath << std::endl;
						model->datas->materialPath = p;
						model->datas->diffuseTexture = newDiffuseTexture;

						MaterialInfos mInfo{ p, newDiffuseTexture };
						diffuseTextures.push_back(mInfo);
					}
				}
			}
			else
			{
				std::cout << "No Diffuse texture" << std::endl;
			}
		}

		bool TextureExist(const std::string& filename, int& index, const std::vector<MaterialInfos>& textures)
		{
			const size_t texturesSize = textures.size();

			for(size_t i = 0; i < texturesSize; ++i)
			{
				//texture exist
				index = static_cast<int>(i);
				if (textures[i].path == filename)
					return true;
			}
			//texture not exist
			return false;
		}

		void LoadSpecularTexture(const aiMaterial* material, AnimationModel* model)
		{
			const unsigned textureCount = material->GetTextureCount(aiTextureType_SPECULAR);

			if (textureCount > 0)
			{
				aiString path;

				if (material->GetTexture(aiTextureType_SPECULAR, 0, &path, nullptr, nullptr, nullptr, nullptr, nullptr) == AI_SUCCESS)
				{
					std::string p(path.data);

					std::string filePath = "../Models/" + p;
					Texture* newSpecularTexture = new Texture(GL_TEXTURE_2D, filePath);

					if (!newSpecularTexture->Load())
					{
						std::cout << "Failed to Load specular texture" << std::endl;
					}
					else
					{
						std::cout << "Load specular texture : " << filePath << std::endl;
						model->datas->materialPath = p;
						model->datas->specularTexture = newSpecularTexture;

						MaterialInfos mInfo{ p, newSpecularTexture };
						specularTextures.push_back(mInfo);
					}
				}
			}
			else
			{
				std::cout << "No Specular texture" << std::endl;
			}
		}

		void LoadNormalTexture(const aiMaterial* material, AnimationModel* model)
		{
			const unsigned textureCount = material->GetTextureCount(aiTextureType_NORMALS);

			if (textureCount > 0)
			{
				aiString path;

				if (material->GetTexture(aiTextureType_NORMALS, 0, &path, nullptr, nullptr, nullptr, nullptr, nullptr) == AI_SUCCESS)
				{
					std::string p(path.data);

					std::string filePath = "../Models/" + p;
					Texture* newNormalTexture = new Texture(GL_TEXTURE_2D, filePath);

					if (!newNormalTexture->Load())
					{
						std::cout << "Failed to Load normal texture" << std::endl;
					}
					else
					{
						std::cout << "Load normal texture : " << filePath << std::endl;
						model->datas->materialPath = p;
						model->datas->normalTexture = newNormalTexture;

						MaterialInfos mInfo{ p, newNormalTexture};
						normalTextures.push_back(mInfo);
					}
				}
			}
			else
			{
				std::cout << "No normal texture" << std::endl;
			}
		}

	}
	namespace BoneLoading
	{
		void InitAllBones(AnimationModel* model)
		{
			unsigned size = static_cast<unsigned>(model->datas->meshes.size());

			for (unsigned i = 0; i < size; ++i)
			{
				aiMesh* pMesh = model->GetScene()->mMeshes[i];
				//InitSingleMesh(pMesh, i, model);
				LoadMeshBones(pMesh, i, model);
			}
		}
		void LoadMeshBones(aiMesh* mesh, unsigned meshIndex, AnimationModel* model)
		{
			const unsigned size = mesh->mNumBones;

			for (unsigned i = 0; i < size; ++i)
				LoadSingleBone(mesh->mBones[i], meshIndex, model);
		}

		void LoadSingleBone(aiBone* bone, unsigned meshIndex, AnimationModel* model)
		{
			const int boneId = GetBoneID(bone, model);

			if (boneId == model->datas->boneInfos.size())
			{
				glm::mat4 mat = mat4_cast(bone->mOffsetMatrix);
				BoneInfo boneInfo(mat);
				model->datas->boneInfos.push_back(boneInfo);
			}

			for (unsigned i = 0; i < bone->mNumWeights; ++i)
			{
				aiVertexWeight& vw = bone->mWeights[i];
				unsigned globalVertexId = model->datas->meshes[meshIndex].BaseVertex + bone->mWeights[i].mVertexId;

				model->datas->bones[globalVertexId].AddBoneData(boneId, vw.mWeight);
			}
		}

		int GetBoneID(const aiBone* bone, AnimationModel* model)
		{
			int boneIndex;
			std::string name(bone->mName.C_Str());

			//means not found
			if (model->datas->boneName2IndexMap.find(name) == model->datas->boneName2IndexMap.end())
			{
				boneIndex = static_cast<int>(model->datas->boneName2IndexMap.size());
				model->datas->boneName2IndexMap[name] = boneIndex;
			}
			else
				boneIndex = model->datas->boneName2IndexMap[name];

			return boneIndex;
		}
	}
	namespace AnimationMatrix
	{
		void PrintMatrix(glm::mat4 a, std::string name)
		{
			std::cout << name << std::endl;
			std::cout << glm::to_string(a) << std::endl;
			std::cout << std::endl;
		}

		void GetBoneTransforms(std::vector<glm::mat4>& transforms, float timeInSeconds, const aiScene* scene, AnimationModel* model, unsigned animationIndex)
		{
			transforms.resize(model->datas->boneInfos.size());

			//const aiAnimation* animation = scene->mAnimations[animationIndex];
			const glm::mat4 identityMat = glm::mat4(1.f);

			/*const float ticksPerSecond = static_cast<float>(animation->mTicksPerSecond != 0 ? animation->mTicksPerSecond : 25.f);
			const float timeInTicks = timeInSeconds * ticksPerSecond;
			const float animationTimeTicks = fmod(timeInTicks, static_cast<float>(animation->mDuration));*/

			int nodeIndex = 0;

			ReadNodeHierarchy(scene->mRootNode, identityMat, timeInSeconds, scene, model, animationIndex, nodeIndex);

			const unsigned size = static_cast<unsigned>(model->datas->boneInfos.size());

			for (unsigned i = 0; i < size; ++i)
			{
				transforms[i] = model->datas->boneInfos[i].finalTransform;
				PrintMatrix(transforms[i], "BT " + std::to_string(i));
			}
		}

		void ReadNodeHierarchy(const aiNode* node, const glm::mat4& parentTransform, float animationTimeTicks, const aiScene* scene, AnimationModel* model, int animationIndex,
			int& nodeIndex)
		{
			std::string nodeName(node->mName.data);

			const aiAnimation* pAnimation = scene->mAnimations[animationIndex];
			const aiNodeAnim* pNodeAnim = FindNodeAnimation(pAnimation, nodeName);

			glm::mat4 nodeTransform(AnimatingFunctions::mat4_cast(node->mTransformation));


			if (pNodeAnim)
			{
				aiVector3D scaling;
				Interpolation::CalcInterpolatingScaling(scaling, animationTimeTicks, pNodeAnim);
				glm::mat4 scalingM;
				scalingM = glm::scale(scalingM, glm::vec3(scaling.x, scaling.y, scaling.z));

				Quaternion rotationQ;
				Interpolation::CalcInterpolatedRotation(rotationQ, animationTimeTicks, pNodeAnim);
				const glm::mat4 rotationM = rotationQ.GetMatrix();

				aiVector3D translation;
				Interpolation::CalcInterpolatedPosition(translation, animationTimeTicks, pNodeAnim);
				glm::mat4 translationM;
				translationM = glm::translate(translationM, glm::vec3(translation.x, translation.y, translation.z));

				nodeTransform = translationM * rotationM * scalingM;
			}

			const glm::mat4 globalTransform = parentTransform * nodeTransform;

			//Means find
			if (model->datas->boneName2IndexMap.find(nodeName) != model->datas->boneName2IndexMap.end())
			{
				const uint boneIndex = model->datas->boneName2IndexMap[nodeName];
				model->datas->boneInfos[boneIndex].finalTransform = globalTransform * model->datas->boneInfos[boneIndex].offsetMat;
			}

			if(nodeIndex == 3)
			{
				PrintMatrix(parentTransform, "3ParentTransform");
			}

			nodeIndex++;

			for (uint i = 0; i < node->mNumChildren; ++i)
				ReadNodeHierarchy(node->mChildren[i], globalTransform, animationTimeTicks, scene, model, animationIndex,
					nodeIndex);
		}

		aiNodeAnim* FindNodeAnimation(const aiAnimation* pAnimation, const std::string& nodeName)
		{
			for (uint i = 0; i < pAnimation->mNumChannels; ++i)
			{
				aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

				if (std::string(pNodeAnim->mNodeName.data) == nodeName)
					return pNodeAnim;
			}

			return NULL;
		}

		void GetNodeTransforms(std::vector<glm::mat4>& nodeTransforms, const aiScene* scene,
			std::vector<std::vector<int>>& transformOrder, std::vector<int>& nodeContainAnimation,
			std::vector<std::vector<glm::vec4>>& scalingFactor, std::vector<std::vector<glm::vec4>>& translationFactor,
			std::vector<std::vector<glm::vec4>>& rotationFactor,
			std::vector<std::vector<float>>& factorTimeStamps,
			std::vector<glm::mat4>& offsetMatrixes,
			std::vector<int>& containOffsetMatrixes,
			std::vector<int>& boneIndexes, AnimationModel* model)
		{
			int transformIndex = 0;
			std::vector<int> prevOrder;

			ReadNodeHierarchyNodeTransforms(scene->mRootNode, scene, nodeTransforms, transformIndex,
				transformOrder, prevOrder, nodeContainAnimation, scalingFactor, translationFactor, rotationFactor,
				factorTimeStamps, offsetMatrixes, containOffsetMatrixes, boneIndexes, model);
		}

		void ReadNodeHierarchyNodeTransforms(const aiNode* node, const aiScene* scene, std::vector<glm::mat4>& nodeTransforms,
			int& transformIndex, std::vector<std::vector<int>>& transformOrder, std::vector<int> prevOrder
			, std::vector<int>& nodeContainAnimation,
			std::vector<std::vector<glm::vec4>>& scalingFactor, std::vector<std::vector<glm::vec4>>& translationFactor,
			std::vector<std::vector<glm::vec4>>& rotationFactor,
			std::vector<std::vector<float>>& factorTimeStamps,
			std::vector<glm::mat4>& offsetMatrixes,
			std::vector<int>& containOffsetMatrixes,
			std::vector<int>& boneIndexes, AnimationModel* model)
		{
			std::string nodeName(node->mName.data);

			const aiAnimation* pAnimation = scene->mAnimations[0];
			const aiNodeAnim* pNodeAnim = FindNodeAnimation(pAnimation, nodeName);

			const glm::mat4 nodeTransform(mat4_cast(node->mTransformation));

			nodeTransforms.push_back(nodeTransform);

			prevOrder.push_back(transformIndex);
			transformIndex++;
			transformOrder.push_back(prevOrder);

			if(pNodeAnim)
			{
				nodeContainAnimation.push_back(1);

				std::vector<float> timeStamps;

				std::vector<glm::vec4> scalingVec;
				for(unsigned i = 0; i < pNodeAnim->mNumScalingKeys; ++i)
				{
					aiVector3D scaling = pNodeAnim->mScalingKeys[i].mValue;
					glm::vec4 scalingValue{scaling.x, scaling.y, scaling.z, 0.f};
					scalingVec.push_back(scalingValue);
					timeStamps.push_back(static_cast<float>(pNodeAnim->mScalingKeys[i].mTime));
				}
				scalingFactor.push_back(scalingVec);

				std::vector<glm::vec4> translationVec;
				for (unsigned i = 0; i < pNodeAnim->mNumPositionKeys; ++i)
				{
					aiVector3D translation = pNodeAnim->mPositionKeys[i].mValue;
					glm::vec4 translationValue{ translation.x, translation.y, translation.z , 0.f};
					translationVec.push_back(translationValue);
				}
				translationFactor.push_back(translationVec);

				std::vector<glm::vec4> rotationVec;
				for (unsigned i = 0; i < pNodeAnim->mNumRotationKeys; ++i)
				{
					aiQuaternion quaternion = pNodeAnim->mRotationKeys[i].mValue;
					glm::vec4 quaternionValue{ quaternion.x, quaternion.y, quaternion.z, quaternion.w};
					rotationVec.push_back(quaternionValue);
				}
				rotationFactor.push_back(rotationVec);

				factorTimeStamps.push_back(timeStamps);

			}
			else
			{
				nodeContainAnimation.push_back(0);
				scalingFactor.push_back({ glm::vec4{1.f, 1.f, 1.f, 0.f} });
				translationFactor.push_back({ glm::vec4{0.f, 0.f, 0.f, 0.f} });
				rotationFactor.push_back({ glm::vec4{0.f, 0.f, 0.f, 0.f} });
				factorTimeStamps.push_back({ 0.f });
			}

			if (model->datas->boneName2IndexMap.find(nodeName) != model->datas->boneName2IndexMap.end())
			{
				const uint boneIndex = model->datas->boneName2IndexMap[nodeName];
				boneIndexes.push_back(static_cast<int>(boneIndex));
				offsetMatrixes.push_back(model->datas->boneInfos[boneIndex].offsetMat);
				containOffsetMatrixes.push_back(1);
			}
			else
			{
				boneIndexes.push_back(-1);
				offsetMatrixes.push_back(glm::mat4(1.f));
				containOffsetMatrixes.push_back(0);
			}


			for (uint i = 0; i < node->mNumChildren; ++i)
			{
				ReadNodeHierarchyNodeTransforms(node->mChildren[i], scene, nodeTransforms, transformIndex,
					transformOrder, prevOrder, nodeContainAnimation, scalingFactor, translationFactor, rotationFactor,
					factorTimeStamps, offsetMatrixes, containOffsetMatrixes, boneIndexes, model);
			}
		}

		std::vector<glm::mat4> CheckNodeTransforms(const aiScene* scene,
			std::vector<glm::mat4> finalTransforms, AnimationModel* model)
		{
			std::vector<glm::mat4> result;
			result.resize(model->datas->boneInfos.size());
			int transformIndex = 0;
			const glm::mat4 identityMat = glm::mat4(1.f);

			CheckNodeHierarchyTransforms(scene->mRootNode, scene, finalTransforms, model, transformIndex);
			const unsigned size = static_cast<unsigned>(model->datas->boneInfos.size());

			for (uint i = 0; i < size; ++i)
			{
				result[i] = model->datas->boneInfos[i].finalTransform;
				PrintMatrix(result[i], "NT " + std::to_string(i));
			}

			return result;
		}

		void CheckNodeHierarchyTransforms(const aiNode* node, const aiScene* scene,
			std::vector<glm::mat4> finalTransforms, AnimationModel* model,
			int& transformIndex)
		{
			const std::string nodeName(node->mName.data);
			const glm::mat4 interpolatedTransform = finalTransforms[transformIndex];

			//const glm::mat4 globalTransform = parentTransform * interpolatedTransform;
			const glm::mat4 globalTransform = interpolatedTransform;

			//Means find
			if (model->datas->boneName2IndexMap.find(nodeName) != model->datas->boneName2IndexMap.end())
			{
				const uint boneIndex = model->datas->boneName2IndexMap[nodeName];
				model->datas->boneInfos[boneIndex].finalTransform = globalTransform * model->datas->boneInfos[boneIndex].offsetMat;
			}

			transformIndex++;

			for (uint i = 0; i < node->mNumChildren; ++i)
				CheckNodeHierarchyTransforms(node->mChildren[i], scene, finalTransforms, model, transformIndex);
		}
	}

}




