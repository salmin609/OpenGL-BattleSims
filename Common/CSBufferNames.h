#pragma once

enum class MoveCS
{
	AnimationIndex = 0,
	objsPoses,
	objsDirections,
	time,
	randSpeed,
	targetEnemyPos,
	attackedCount,
	isDead
};

inline int ToInt(MoveCS val)
{
	return static_cast<int>(val);
}


enum class AttackCS
{
	time = 0,
	AnimationIndex,
	objsPoses,
	attackedCount,
	isDead
};

inline int ToInt(AttackCS val)
{
	return static_cast<int>(val);
}

enum class AngleCS
{
	frameBufferUsageIndex = 0,
	objsPoses,
	objsDirections
};

inline int ToInt(AngleCS val)
{
	return static_cast<int>(val);
}

enum class AnimationChangeCS
{
	animationIndex = 0,
	prevAnimationIndex,
	needReset,
};

inline int ToInt(AnimationChangeCS val)
{
	return static_cast<int>(val);
}