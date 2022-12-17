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
#include "Buffer.hpp"
//#include "Graphic.h"


#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals |  aiProcess_JoinIdenticalVertices )

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
		ASSIMP_LOAD_FLAGS);

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
}

AnimationModel::~AnimationModel()
{
	delete importer;
	delete datas;
}


void AnimationModel::Select()
{
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
	const glm::mat4& objMat, const glm::mat4& projViewMat, float animationT, int transformsOffset,
	std::vector<glm::mat4> transforms)
{
	assert(shader != nullptr);

	shader->Use();
	Select();

	glm::mat4 matrix = projViewMat * objMat;
	glm::mat4 modelMat = objMat;
	shader->SendUniformMatGLM("gWVP", &matrix);
	datas->gBonesBuffer->WriteData<glm::mat4>(transforms.data());
	datas->gBonesBuffer->BindStorage(4);
	shader->SendUniformInt("transformIndex", transformsOffset);
	shader->SendUniformFloat("timeTicks", animationT);
	shader->SendUniformMatGLM("model", &modelMat);
	

	if (datas->diffuseTexture != nullptr)
		datas->diffuseTexture->Bind(shader->GetShaderId(), 0, "gDiffuse");

	if (datas->specularTexture != nullptr)
		datas->specularTexture->Bind(shader->GetShaderId(), 1, "gSpecular");

	if (datas->normalTexture != nullptr)
		datas->normalTexture->Bind(shader->GetShaderId(), 2, "gNormal");

	const unsigned meshesSize = datas->meshes.size();
	for (unsigned i = 0; i < meshesSize; ++i)
	{
		glDrawElementsBaseVertex(GL_TRIANGLES, static_cast<GLsizei>(datas->meshes[i].NumIndices),
			GL_UNSIGNED_INT, (void*)(sizeof(unsigned int) * datas->meshes[i].BaseIndex)
			, static_cast<GLsizei>(datas->meshes[i].BaseVertex));

		//glBindTexture(GL_TEXTURE_2D, 0);
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

std::vector<glm::mat4> AnimationModel::Interpolate(float animationTimeTicks)
{
	interpolationComputeShader->Use();
	datas->BindInterpolationBuffer();
	interpolationComputeShader->SendUniformInt("inTransformsSize", static_cast<int>(datas->nodeTransforms.size()));
	interpolationComputeShader->SendUniformFloat("animationTimeTicks", animationTimeTicks);

	glMemoryBarrier(GL_ALL_BARRIER_BITS);
	glDispatchCompute(static_cast<GLuint>(datas->transformOrder.size()), 1, 1);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glUseProgram(0);

	return datas->computeOutFinalTransforms->Check<glm::mat4>();
}

void AnimationModel::PopulateTransforms(std::vector<glm::mat4>& transforms)
{
	datas->PopulateTransforms(transforms);
}


