#pragma once
#include <vector>
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

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
	//void PopulateObjsPos();
	void PopulateObjs(int num, int obj, glm::vec3 pos, float offset);
	void Populate();
	void CheckFrameBufferUsage();
	void SetBosFrameBufferIndex();
	void Render(const glm::mat4& projMat, const glm::mat4& viewMat);
	void SetShaderUniforms();
	void SetPositionOffsetBuffers(std::vector<glm::vec3> directions);
	std::vector<glm::vec3> GetHerdPos(int num, glm::vec3 pos, float offset);
	void PopulateBuffers();
	std::vector<Texture*> GetTextures(const std::vector<BillboardAnimatingDatas*>& boDatas);

	float zNear = 0.1f;
	float zFar = 1000.f;
	void* boFBusageDatas{};
	unsigned gArrayTexture{};
	int totalPositionBufferCount = 0;

	//xyz = boDirection, w = count
	glm::vec4* herdBoDirAndOffset{};
private:
	unsigned vao{};
	Shader* boShader;
	Shader* boFBusageComputeShader;
	std::vector<std::vector<BillBoardObject*>> bos;
	BillboardManager* boManager;
	Camera* currentCam;
	int posOffset = 0;
	int totalRenderingAmount = 0;
	int herdCount = 0;
	std::vector<int> herdOffset;

	std::vector<glm::vec4> posDatas;

	BufferManager* csBuffers;
};
