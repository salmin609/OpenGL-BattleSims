#pragma once
#include <vector>
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
	Herd* GetHerd(int index);
	void GetHerdPositions(int num, glm::vec3 pos, float offset);
	void PopulateBuffers();
	Herd* PopulateHerd(int num, int obj, glm::vec3 pos, float offset, glm::vec4 herdDirection,
		int side);
	void ChangeAnimationIndicesOfHerd(int* fbAngleIndices, int* animationStateIndices,
		int* isDead);
	int totalRenderingAmount;
	std::vector<int> herdOffset;
	int posBufferIndex = 1;
	int boObjIndex = 0;
	std::vector<glm::vec4> positionDatas;
	Buffer* posBuffer{};
	Buffer* directionBuffer{};
private:
	std::vector<Herd*> herds;
	int herdCount{};
	BillboardManager* boManager;
	Shader* boShader;
};
