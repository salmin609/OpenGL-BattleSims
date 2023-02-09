#pragma once
#include <string>
#include <vector>

enum class ObjKind
{
	SWAT = 0,
	KNIGHT,

	END,
};

enum class AnimationKinds
{
	SWAT_IDLE_0 = 0,
	SWAT_RUN_0,
	SWAT_ATTACK_0,
	SWAT_DEATH_0,
	SWAT_PAIN_0,

	KNIGHT_IDLE_0,
	KNIGHT_RUN_0,
	KNIGHT_ATTACK_0,
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

		case AnimationKinds::SWAT_RUN_0:
			path += "Swat_Run_0";
			break;

		case AnimationKinds::SWAT_DEATH_0:
			path += "Swat_Death_0";
			break;

		case AnimationKinds::SWAT_ATTACK_0:
			path += "Swat_Attack_0";
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
