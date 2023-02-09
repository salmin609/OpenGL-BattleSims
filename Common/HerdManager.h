#pragma once
#include <vector>
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

class Shader;
class BillboardManager;
class Buffer;
class Herd;

class HerdManager
{
public:
	HerdManager(BillboardManager* boManager_, Shader* boShader_);
	~HerdManager();
	void Render(const glm::mat4& projMat, const glm::mat4& viewMat);

	void AddHerd(Herd* herd);
	int& GetHerdCount();
	void BindHerdPositions();
	Herd* GetHerd(int index);
	Buffer* GetHerdPositionBuffer(int num, glm::vec3 pos, float offset);
	Herd* PopulateHerd(int num, int obj, glm::vec3 pos, float offset);
	void SetBosFrameBufferIndex(void* boFBusageDatas);
	void SetReachedAnimation(int* data);
	void ChangeToAttackAnimation();
	int totalRenderingAmount;
	std::vector<int> herdOffset;
	int posBufferIndex = 1;

private:
	std::vector<Herd*> herds;
	int herdCount{};
	BillboardManager* boManager;
	Shader* boShader;
};
