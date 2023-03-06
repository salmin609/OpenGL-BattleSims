#pragma once
#include <vector>
#include "glm/vec3.hpp"

class Herd;

class HerdSetterDatas
{
public:
	HerdSetterDatas(const std::vector<int>& objKinds_,
		glm::vec3 initialPos_,
		float posOffsetZ_,
		int num_,
		int width_,
		float speed_);
	~HerdSetterDatas();
	std::vector<int> objKinds;
	glm::vec3 initialPos;
	float posOffsetZ;
	int num;
	int width;
	float speed;
private:
	
};

class HerdSetter
{
public:
	HerdSetter(const std::vector<HerdSetterDatas>& hDatas_);
	~HerdSetter();

	
	std::vector<Herd*> GetHerds();
private:
	std::vector<HerdSetterDatas> hDatas;
	std::vector<Herd*> herds;
};