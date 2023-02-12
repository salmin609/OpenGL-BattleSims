#pragma once
#include <string>
#include <vector>


enum class State
{
	Idle = 0,
	Attack,
	Pain,
	Run,
	Death
};

enum class ObjKind
{
	SWAT = 0,
	KNIGHT,

	END,
};

enum class AnimationKinds
{
	SWAT_IDLE_0 = 0,
	SWAT_IDLE_1,
	SWAT_IDLE_2,
	SWAT_RUN_0,
	SWAT_ATTACK_0,
	SWAT_ATTACK_1,
	SWAT_ATTACK_2,
	SWAT_ATTACK_3,
	SWAT_DEATH_0,
	SWAT_PAIN_0,

	KNIGHT_IDLE_0,
	KNIGHT_RUN_0,
	KNIGHT_ATTACK_0,
	KNIGHT_ATTACK_1,
	KNIGHT_ATTACK_2,
	KNIGHT_ATTACK_3,
	KNIGHT_DEATH_0,
	KNIGHT_PAIN_0,

	END,
};

inline std::vector<std::string> ObjPaths()
{
	std::vector<std::string> objPaths;
	for (int i = 0; i < static_cast<int>(AnimationKinds::END); ++i)
	{
		std::string path = "../Models/";
		switch (static_cast<AnimationKinds>(i))
		{
		case AnimationKinds::SWAT_IDLE_0:
			path += "Swat_Idle_0";
			break;

		case AnimationKinds::SWAT_IDLE_1:
			path += "Swat_Idle_1";
			break;

		case AnimationKinds::SWAT_IDLE_2:
			path += "Swat_Idle_2";
			break;

		case AnimationKinds::SWAT_RUN_0:
			path += "Swat_Run_0";
			break;

		case AnimationKinds::SWAT_DEATH_0:
			path += "Swat_Death_0";
			break;

		case AnimationKinds::SWAT_ATTACK_0:
			path += "Swat_Attack_0";
			break;

		case AnimationKinds::SWAT_ATTACK_1:
			path += "Swat_Attack_1";
			break;

		case AnimationKinds::SWAT_ATTACK_2:
			path += "Swat_Attack_2";
			break;

		case AnimationKinds::SWAT_ATTACK_3:
			path += "Swat_Attack_3";
			break;

		case AnimationKinds::SWAT_PAIN_0:
			path += "Swat_Pain_0";
			break;


		case AnimationKinds::KNIGHT_IDLE_0:
			path += "Knight_Idle_0";
			break;

		case AnimationKinds::KNIGHT_RUN_0:
			path += "Knight_Run_0";
			break;
		case AnimationKinds::KNIGHT_ATTACK_0:
			path += "Knight_Attack_0";
			break;
		case AnimationKinds::KNIGHT_ATTACK_1:
			path += "Knight_Attack_1";
			break;
		case AnimationKinds::KNIGHT_ATTACK_2:
			path += "Knight_Attack_2";
			break;
		case AnimationKinds::KNIGHT_ATTACK_3:
			path += "Knight_Attack_3";
			break;

		case AnimationKinds::KNIGHT_DEATH_0:
			path += "Knight_Death_0";
			break;

		case AnimationKinds::KNIGHT_PAIN_0:
			path += "Knight_Pain_0";
			break;

		case AnimationKinds::END:
			break;
		default:;
		}
		path += ".dae";
		objPaths.push_back(path);
	}

	return objPaths;
}
