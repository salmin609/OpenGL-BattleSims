#pragma once
#include <string>
#include <vector>

std::vector<std::string> GetObjStr();

enum class State
{
	Idle = 0,
	Attack,
	Pain,
	Run,
	Death,
	END
};

enum class ObjKind
{
	MUTANT = 0,
	SWAT,
	ARCHER,
	KNIGHT,
	VIKING,
	END,
};

enum class ObjAttackType
{
	MELEE = 101,
	RANGED
};
