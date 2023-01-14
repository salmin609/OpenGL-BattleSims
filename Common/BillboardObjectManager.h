#pragma once
#include <vector>
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

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
	void CheckFrameBufferUsage(Frustum* frustum, Camera* cam, float fov);
	void Render(const glm::mat4& projMat, const glm::mat4& viewMat);
	float zNear = 0.1f;
	float zFar = 1000.f;
private:
	unsigned vao;
	Shader* boShader;
	Shader* boFBusageComputeShader;

	std::vector<BillBoardObject*> bos;
	std::vector<glm::vec3> objsPos;
	glm::vec3 populateLastPosition = glm::vec3(0.f, 12.f, -20.f);
	BillboardManager* boManager;
	Camera* currentCam;
	int posOffset = 0;
	int totalRenderingAmount = 0;
	Buffer* boPosBuffer;
	Buffer* boFrameBufferUsageBuffer;
	Buffer* boIndexCheckBuffer;
	std::vector<glm::vec3> posDatas;
};
