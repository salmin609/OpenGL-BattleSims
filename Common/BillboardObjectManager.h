#pragma once
#include <vector>
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

class Herd;
class BufferManager;
class Texture;
class BillboardAnimatingDatas;
class Buffer;
struct Frustum;
class Camera;
class BillboardManager;
class BillBoardObject;
class Shader;

class BillboardObjectManager
{
public:
	BillboardObjectManager(Shader* boShader_, BillboardManager* boManager_,
		Shader* boFrameBufferUsageComputeShader,
		Camera* currentCam_);
	~BillboardObjectManager();

	Herd* PopulateObjs(int num, int obj, glm::vec3 pos, float offset);
	void Populate();
	void CheckFrameBufferUsage();
	void SetBosFrameBufferIndex();
	void Render(const glm::mat4& projMat, const glm::mat4& viewMat);
	void SetShaderUniforms();
	void SetPositionOffsetBuffers(std::vector<glm::vec3> directions);
	Buffer* GetHerdPosBuffer(int num, glm::vec3 pos, float offset);
	void PopulateBuffers();
	std::vector<Texture*> GetTextures(const std::vector<BillboardAnimatingDatas*>& boDatas);

	float zNear = 0.1f;
	float zFar = 1000.f;
	void* boFBusageDatas{};
	unsigned gArrayTexture{};
	int herdCount{};
	//xyz = boDirection, w = count
	//glm::vec4* herdBoDirAndOffset{};

	int posBufferIndex = 1;
	std::vector<glm::vec4> resultVector;
	std::vector<Herd*> herds;

private:
	Shader* boShader;
	Shader* boFBusageComputeShader;
	std::vector<std::vector<BillBoardObject*>> bos;
	BillboardManager* boManager;
	Camera* currentCam;
	int totalRenderingAmount = 0;
	std::vector<int> herdOffset;

	//std::vector<glm::vec4> posDatas;

	BufferManager* csBuffers;
};
