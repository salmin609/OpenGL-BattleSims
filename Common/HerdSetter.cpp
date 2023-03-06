#include "HerdSetter.h"

HerdSetterDatas::HerdSetterDatas(const std::vector<int>& objKinds_,
	glm::vec3 initialPos_,
	float posOffsetZ_,
	int num_,
	int width_,
	float speed_)
{
	objKinds = objKinds_;
	initialPos = initialPos_;
	posOffsetZ = posOffsetZ_;
	num = num_;
	width = width_;
	speed = speed_;
}

HerdSetterDatas::~HerdSetterDatas()
{
}

HerdSetter::HerdSetter(const std::vector<HerdSetterDatas>& hDatas_)
{
	hDatas = hDatas_;


}

HerdSetter::~HerdSetter()
{
}

std::vector<Herd*> HerdSetter::GetHerds()
{
	return herds;
}
