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
	MUTANT = 0,
	SWAT,
	KNIGHT,

	END,
};

enum class AnimationKinds
{
	SWAT_IDLE_0 = 0,
	SWAT_IDLE_1,
	SWAT_IDLE_2,
	SWAT_RUN_0,
	SWAT_RUN_1,
	SWAT_ATTACK_0,
	SWAT_ATTACK_1,
	SWAT_ATTACK_2,
	SWAT_ATTACK_3,
	SWAT_DEATH_0,
	SWAT_PAIN_0,

	KNIGHT_IDLE_0,
	KNIGHT_IDLE_1,
	KNIGHT_IDLE_2,
	KNIGHT_RUN_0,
	KNIGHT_RUN_1,
	KNIGHT_RUN_2,
	KNIGHT_ATTACK_0,
	KNIGHT_ATTACK_1,
	KNIGHT_ATTACK_2,
	KNIGHT_ATTACK_3,
	KNIGHT_DEATH_0,
	KNIGHT_DEATH_1,
	KNIGHT_DEATH_2,
	KNIGHT_PAIN_0,

	MUTANT_IDLE_0,
	MUTANT_IDLE_1,
	MUTANT_IDLE_2,
	MUTANT_RUN_0,
	MUTANT_RUN_1,
	MUTANT_ATTACK_0,
	MUTANT_ATTACK_1,
	MUTANT_DEATH_0,
	MUTANT_DEATH_1,
	MUTANT_PAIN_0,

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
		case AnimationKinds::KNIGHT_IDLE_1:
			path += "Knight_Idle_1";
			break;
		case AnimationKinds::KNIGHT_IDLE_2: 
			path += "Knight_Idle_2";
			break;
		case AnimationKinds::KNIGHT_RUN_1:
			path += "Knight_Run_1";
			break;
		case AnimationKinds::KNIGHT_RUN_2:
			path += "Knight_Run_2";
			break;
		case AnimationKinds::KNIGHT_DEATH_1: 
			path += "Knight_Death_1";
			break;
		case AnimationKinds::KNIGHT_DEATH_2:
			path += "Knight_Death_2";
			break;
		case AnimationKinds::SWAT_RUN_1:
			path += "Swat_Run_1";
			break;
		case AnimationKinds::MUTANT_IDLE_0:
			path += "Mutant_Idle_0";
			break;
		case AnimationKinds::MUTANT_IDLE_1:
			path += "Mutant_Idle_1";
			break;
		case AnimationKinds::MUTANT_IDLE_2: 
			path += "Mutant_Idle_2";
			break;
		case AnimationKinds::MUTANT_RUN_0: 
			path += "Mutant_Run_0";
			break;
		case AnimationKinds::MUTANT_RUN_1: 
			path += "Mutant_Run_1";
			break;
		case AnimationKinds::MUTANT_ATTACK_0: 
			path += "Mutant_Attack_0";
			break;
		case AnimationKinds::MUTANT_ATTACK_1: 
			path += "Mutant_Attack_1";
			break;
		case AnimationKinds::MUTANT_DEATH_0: 
			path += "Mutant_Death_0";
			break;
		case AnimationKinds::MUTANT_DEATH_1: 
			path += "Mutant_Death_1";
			break;
		case AnimationKinds::MUTANT_PAIN_0: 
			path += "Mutant_Pain_0";
			break;
		default:;
		}
		path += ".dae";
		objPaths.push_back(path);
	}

	return objPaths;
}
