#pragma once
#include <string>
#include <vector>

enum class ObjKind
{
	SWAT = 0,
	AMY,

	END,


	KNIGHT,
	MICHELLE,
	ADAM,

};

enum class AnimationKinds
{
	SWAT_RifleAimIdle = 0,
	SWAT_RifleCrouch,
	SWAT_RifleIdle,
	SWAT_RifleWalk,
	SWAT_Run,
	SWAT_SteppingBackward,
	SWAT_Strafing,
	SWAT_TurnLeft45Degree,
	SWAT_CrawlBackward,
	SWAT_Death,
	SWAT_Reload,
	SWAT_Reload2,
	SWAT_Smash,



	AMY_Excited,
	AMY_Floating,
	AMY_HappyIdle,
	AMY_Jogging,
	AMY_Singing,
	AMY_StandingUp,
	AMY_Surprised,
	AMY_TalkingOnPhone,
	AMY_TwistDance,

	END,


	KNIGHT_Attack1,
	KNIGHT_Block,
	KNIGHT_BlockIdle,
	KNIGHT_CrouchBlockIdle,
	KNIGHT_Death,
	KNIGHT_Idle,
	KNIGHT_Idle2,
	KNIGHT_Impact,
	KNIGHT_PowerUp,
	KNIGHT_Run,
	KNIGHT_Slash,



	MICHELLE_BreakDance,
	MICHELLE_BreakDanceReady,
	MICHELLE_Dancing,
	MICHELLE_DancingTwerk,
	MICHELLE_Flair,
	MICHELLE_HipHopDance1,
	MICHELLE_HipHopDance2,
	MICHELLE_HipHopDance3,
	MICHELLE_HipHopDance4,
	MICHELLE_RumbaDance,
	MICHELLE_Twist,
	MICHELLE_WaveHipHop,



	ADAM_ComboPunch,
	ADAM_ElbowPunch,
	ADAM_Idle,
	ADAM_Idle2,
	ADAM_Jab,
	ADAM_MMAKick,
	ADAM_Victory,


};

inline std::vector<std::string> ObjPaths()
{
	std::vector<std::string> objPaths;
	for (int i = 0; i < static_cast<int>(AnimationKinds::END); ++i)
	{
		std::string path = "../Models/";
		switch (static_cast<AnimationKinds>(i))
		{
		case AnimationKinds::SWAT_RifleAimIdle:
			path += "Swat_RifleAimingIdle";
			break;
		case AnimationKinds::SWAT_RifleCrouch:
			path += "Swat_RifleCrouchWalk";
			break;
		case AnimationKinds::SWAT_RifleIdle:
			path += "Swat_RifleIdle";
			break;
		case AnimationKinds::SWAT_RifleWalk:
			path += "Swat_RifleWalk";
			break;
		case AnimationKinds::SWAT_Run:
			path += "Swat_Run";
			break;
		case AnimationKinds::SWAT_SteppingBackward:
			path += "Swat_SteppingBackward";
			break;
		case AnimationKinds::SWAT_Strafing:
			path += "Swat_Strafing";
			break;
		case AnimationKinds::SWAT_TurnLeft45Degree:
			path += "Swat_TurnLeft45Degree";
			break;
		case AnimationKinds::SWAT_CrawlBackward:
			path += "Swat_CrawlBackward";
			break;
		case AnimationKinds::SWAT_Death:
			path += "Swat_Death";
			break;
		case AnimationKinds::SWAT_Reload:
			path += "Swat_Reload";
			break;
		case AnimationKinds::SWAT_Reload2:
			path += "Swat_Reload2";
			break;
		case AnimationKinds::SWAT_Smash:
			path += "Swat_Smash";
			break;

		case AnimationKinds::AMY_Excited:
			path += "Amy_Excited";
			break;
		case AnimationKinds::AMY_Floating:
			path += "Amy_Floating";
			break;
		case AnimationKinds::AMY_HappyIdle:
			path += "Amy_HappyIdle";
			break;
		case AnimationKinds::AMY_Jogging:
			path += "Amy_Jogging";
			break;
		case AnimationKinds::AMY_Singing:
			path += "Amy_Singing";
			break;
		case AnimationKinds::AMY_StandingUp:
			path += "Amy_StandingUp";
			break;
		case AnimationKinds::AMY_Surprised:
			path += "Amy_Surprised";
			break;
		case AnimationKinds::AMY_TalkingOnPhone:
			path += "Amy_TalkingOnPhone";
			break;
		case AnimationKinds::AMY_TwistDance:
			path += "Amy_TwistDance";
			break;
		case AnimationKinds::KNIGHT_Attack1:
			path += "Knight_Attack1";
			break;
		case AnimationKinds::KNIGHT_Block:
			path += "Knight_Block";
			break;
		case AnimationKinds::KNIGHT_BlockIdle:
			path += "Knight_BlockIdle";
			break;
		case AnimationKinds::KNIGHT_CrouchBlockIdle:
			path += "Knight_CrouchBlockIdle";
			break;
		case AnimationKinds::KNIGHT_Death:
			path += "Knight_Death";
			break;
		case AnimationKinds::KNIGHT_Idle:
			path += "Knight_Idle";
			break;
		case AnimationKinds::KNIGHT_Idle2:
			path += "Knight_Idle2";
			break;
		case AnimationKinds::KNIGHT_Impact:
			path += "Knight_Impact";
			break;
		case AnimationKinds::KNIGHT_PowerUp:
			path += "Knight_PowerUp";
			break;
		case AnimationKinds::KNIGHT_Run:
			path += "Knight_Run";
			break;
		case AnimationKinds::KNIGHT_Slash:
			path += "Knight_Slash";
			break;

		case AnimationKinds::MICHELLE_BreakDance:
			path += "Michelle_BreakDance";
			break;
		case AnimationKinds::MICHELLE_BreakDanceReady:
			path += "Michelle_BreakDanceReady";
			break;
		case AnimationKinds::MICHELLE_Dancing:
			path += "Michelle_Dancing";
			break;
		case AnimationKinds::MICHELLE_DancingTwerk:
			path += "Michelle_DancingTwerk";
			break;
		case AnimationKinds::MICHELLE_Flair:
			path += "Michelle_Flair";
			break;
		case AnimationKinds::MICHELLE_HipHopDance1:
			path += "Michelle_HipHopDance1";
			break;
		case AnimationKinds::MICHELLE_HipHopDance2:
			path += "Michelle_HipHopDance2";
			break;
		case AnimationKinds::MICHELLE_HipHopDance3:
			path += "Michelle_HipHopDance3";
			break;
		case AnimationKinds::MICHELLE_HipHopDance4:
			path += "Michelle_HipHopDance4";
			break;
		case AnimationKinds::MICHELLE_RumbaDance:
			path += "Michelle_RumbaDance";
			break;
		case AnimationKinds::MICHELLE_Twist:
			path += "Michelle_Twist";
			break;
		case AnimationKinds::MICHELLE_WaveHipHop:
			path += "Michelle_WaveHipHop";
			break;

		case AnimationKinds::ADAM_ComboPunch:
			path += "Adam_ComboPunch";
			break;
		case AnimationKinds::ADAM_ElbowPunch:
			path += "Adam_ElbowPunch";
			break;
		case AnimationKinds::ADAM_Idle:
			path += "Adam_Idle1";
			break;
		case AnimationKinds::ADAM_Idle2:
			path += "Adam_Idle2";
			break;
		case AnimationKinds::ADAM_Jab:
			path += "Adam_Jab";
			break;
		case AnimationKinds::ADAM_MMAKick:
			path += "Adam_MMAKick";
			break;
		case AnimationKinds::ADAM_Victory:
			path += "Adam_Victory";
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
