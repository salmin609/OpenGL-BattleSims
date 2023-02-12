#pragma once

enum class MoveCS
{
	AnimationIndex = 0,
	obj1Pos = 1,
	obj2Pos = 2,
	herdDirection1 = 3,
	herdDirection2 = 4,
	time = 5,
	randSpeed = 6,
	targetEnemyPos = 7,
	attackedCount = 8
	
};

inline int ToInt(MoveCS val)
{
	return static_cast<int>(val);
}


enum class AttackCS
{
	time = 0,
	AnimationIndex,
	obj1Pos,
	obj2Pos,
	attackedCount
};

inline int ToInt(AttackCS val)
{
	return static_cast<int>(val);
}

enum class AngleCS
{
	frameBufferUsageIndex = 0,
	obj1Pos,
	obj2Pos,
	obj3Pos,
	obj4Pos,
	herdDirection1,
	herdDirection2
};

inline int ToInt(AngleCS val)
{
	return static_cast<int>(val);
}