#pragma once
#include <vector>
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

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
	void PopulateObjsPos();
	void PopulateObjs(int num, int obj);
	void Populate();
	void CheckFrameBufferUsage(Camera* cam, float fov);
	void Render(const glm::mat4& projMat, const glm::mat4& viewMat,
		const std::vector<BillboardAnimatingDatas*>& boDatas);

	void GenerateArrayTexture(const std::vector<BillboardAnimatingDatas*>& boDatas);

	float zNear = 0.1f;
	float zFar = 1000.f;
	int* boFBusageDatas;
	unsigned gArrayTexture{};
	int totalPositionBufferCount = 0;
private:
	unsigned vao{};
	Shader* boShader;
	Shader* boFBusageComputeShader;

	std::vector<std::vector<BillBoardObject*>> bos;

	//std::vector<BillBoardObject*> bos;
	std::vector<glm::vec3> objsPos;
	glm::vec3 populateLastPosition = glm::vec3(0.f, 12.f, -20.f);
	BillboardManager* boManager;
	Camera* currentCam;
	int posOffset = 0;
	int totalRenderingAmount = 0;
	//Buffer* boPosBuffer{};

	std::vector<Buffer*> boPosBuffer{};

	Buffer* boFrameBufferUsageBuffer;
	//std::vector<glm::vec4> posDatas;
	std::vector<std::vector<glm::vec4>> posDatas;
};
