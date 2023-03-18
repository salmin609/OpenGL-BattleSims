#pragma once

enum class TotalBuffer
{
	objsPoses = 0,
	objsDirections,
	time,
	isDead,
	herdReachedDestination,
	herdAttackingCounts,
	objsCollisionStatus,
	animationIndices,
	attackedCount,
	frameBufferUsageIndex,
	rangeTimer,
};

inline int ToInt(TotalBuffer val)
{
	return static_cast<int>(val);
}

enum class MoveCS
{
	objsPoses = 0,
	isDead,
	herdReachedDestination,
	herdAttackingCounts,
	objsCollisionStatus,
	objsDirections,
	//herdNumCount,
	//herdMovable
};

inline int ToInt(MoveCS val)
{
	return static_cast<int>(val);
}


enum class AttackCS
{
	time = 0,
	animationIndices,
	objsPoses,
	attackedCount,
	isDead,
	//herdNumCount
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
	animationIndices = 0,
	isDead,
	objsCollisionStatus,
	herdAttackingCounts,
	//herdMovable
};

inline int ToInt(AnimationChangeCS val)
{
	return static_cast<int>(val);
}

enum class CollisionCheckCS
{
	objsCollisionStatus = 0,
	objsPoses,
	objsDirections,
	isDead,
	attackedCount,
	herdAttackingCounts,
};

inline int ToInt(CollisionCheckCS val)
{
	return static_cast<int>(val);
}

enum class RangeAttackTimerCS
{
	objsPoses = 0,
	rangeTimer,
	animationIndices
};

inline int ToInt(RangeAttackTimerCS val)
{
	return static_cast<int>(val);
}

enum class ResetCS
{
	herdAttackingCounts = 0,
	objsCollisionStatus,
	//herdMovable
};

inline int ToInt(ResetCS val)
{
	return static_cast<int>(val);
}