/*
 * Author		: Ryan Kim.
 * Date			: 2022-10-07
 * Description	: Class for animating object.
 * Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */


#include "AnimationModel.h"

#include <assimp/scene.h>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

#include "AnimatingFunctions.h"
#include "Camera.hpp"
#include "Shader.h"
#include "assimp/postprocess.h"

#include "Texture.h"
#include "AnimationModelDatas.h"
#include "BufferManager.h"

 /*
  * Create VAO for object,
  *
  * Create Assimp::Importer to read / Parse files.
  *
  * AnimationModelDatas
  * Store all infos about model (positions, normals, textureCoords, index)
  * Also, I use Shader Storage Buffer Object to store boneIndexes, weights, BoneStartIndex, BoneEndIndex, so it stores index of ssbo.
  *
  * After that, Reserve VectorSpace,
  * Read positions / texCoords / normals hierarchical, Also, Find & load texture files.
  */
AnimationModel::AnimationModel(Shader* shaderVal, std::string _filePath, Shader* interpolationShader,
	MeshDatas* reusableMeshDatas)
{
	assert(shaderVal != nullptr);
	assert(interpolationShader != nullptr);
	interpolationComputeShader = interpolationShader;
	shader = shaderVal;

	numVertices = 0;
	numIndices = 0;

	glGenVertexArrays(1, &vao);

	startTime = std::chrono::system_clock::now();
	filePath = _filePath;
	importer = new Importer();

	scene = importer->ReadFile(filePath.c_str(),
		aiProcess_Triangulate |
		aiProcess_GenSmoothNormals |
		aiProcess_JoinIdenticalVertices);

	animDuration = scene->mAnimations[0]->mDuration;

	assert(scene != nullptr);

	datas = new AnimationModelDatas(vao, reusableMeshDatas);

	datas->ReserveSpace(scene);

	if (datas->needNewMesh)
	{
		std::cout << "Initialize Mesh " << _filePath << std::endl;
		AnimatingFunctions::MeshInitializing::InitAllMeshes(this);
	}


	AnimatingFunctions::BoneLoading::InitAllBones(this);
	datas->PopulateBuffers();
	InitMaterial();

	datas->PopulateInterpolationShaderBuffer(scene, this);
	boneTransformsData = new glm::mat4x4[static_cast<int>(datas->boneInfos.size())];
	initialBoneTransformsData = new glm::mat4x4[static_cast<int>(datas->boneInfos.size())];

	SetInitialBoneTransformData();
}

AnimationModel::~AnimationModel()
{
	delete importer;
	delete datas;
	delete[] boneTransformsData;
	delete[] initialBoneTransformsData;
}


void AnimationModel::Select()
{
	shader->Use();

	glBindVertexArray(vao);
	datas->Bind();
}

void AnimationModel::CheckBuffers()
{
}

void AnimationModel::InitMaterial()
{
	AnimatingFunctions::MaterialInitializing::InitMaterials(filePath, this);
}

/*
 * Store all animation transformation datas in transforms vector,
 * read hierarchical transformation from all nodes,
 * and pass infos to shader uniform.
 *
 */
void AnimationModel::Draw(
	const glm::mat4& objMat, const glm::mat4& projViewMat,
	glm::mat4* transforms)
{
	assert(shader != nullptr);
	Select();

	glm::mat4 matrix = projViewMat * objMat;
	glm::mat4 modelMat = objMat;
	shader->SendUniformMatGLM("gWVP", &matrix);
	shader->SendUniformMatGLM("model", &modelMat);
	datas->animInfoBuffers->WriteData(4, transforms);

	if (datas->diffuseTexture != nullptr)
		datas->diffuseTexture->Bind(shader->GetShaderId(), 0, "gDiffuse");

	if (datas->specularTexture != nullptr)
		datas->specularTexture->Bind(shader->GetShaderId(), 1, "gSpecular");

	const auto meshesSize = static_cast<unsigned>(datas->meshes.size());
	for (unsigned i = 0; i < meshesSize; ++i)
	{
		glDrawElementsBaseVertex(GL_TRIANGLES, static_cast<GLsizei>(datas->meshes[i].NumIndices),
			GL_UNSIGNED_INT, reinterpret_cast<void*>(sizeof(unsigned int) * datas->meshes[i].BaseIndex)
			, static_cast<GLsizei>(datas->meshes[i].BaseVertex));
	}

	glBindVertexArray(0);

}

aiNode* AnimationModel::GetRootNode() const
{
	return rootNode;
}

const aiScene* AnimationModel::GetScene() const
{
	return scene;
}

glm::mat4* AnimationModel::Interpolate(float animationTimeTicks) const
{
	assert(interpolationComputeShader != nullptr);

	interpolationComputeShader->Use();
	datas->csBuffers->BindBuffers();
	interpolationComputeShader->SendUniformInt("inTransformsSize", static_cast<int>(datas->nodeTransforms.size()));
	interpolationComputeShader->SendUniformFloat("animationTimeTicks", animationTimeTicks);

	glDispatchCompute(static_cast<GLuint>(datas->transformOrder.size()), 1, 1);
	glMemoryBarrier(GL_ALL_BARRIER_BITS);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glUseProgram(0);
	datas->csBuffers->GetData(15, boneTransformsData);
	return boneTransformsData;
}

void AnimationModel::SetInitialBoneTransformData()
{
	assert(interpolationComputeShader != nullptr);

	interpolationComputeShader->Use();
	datas->csBuffers->BindBuffers();
	interpolationComputeShader->SendUniformInt("inTransformsSize", static_cast<int>(datas->nodeTransforms.size()));
	interpolationComputeShader->SendUniformFloat("animationTimeTicks", 0.f);

	glDispatchCompute(static_cast<GLuint>(datas->transformOrder.size()), 1, 1);
	glMemoryBarrier(GL_ALL_BARRIER_BITS);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glUseProgram(0);
	datas->csBuffers->GetData(15, initialBoneTransformsData);
}

float AnimationModel::GetAnimationTimeTicks(const std::chrono::system_clock::time_point& current) const
{
	const aiAnimation* animation = scene->mAnimations[0];
	const long long diff =
		std::chrono::duration_cast<std::chrono::milliseconds>(current - startTime).count();
	float animationT = static_cast<float>(diff) / 1000.f;

	const float timeInTicks = animationT * static_cast<float>(animation->mTicksPerSecond);
	const float animationTimeTicks = fmod(timeInTicks, static_cast<float>(animation->mDuration));

	return animationTimeTicks;
}
