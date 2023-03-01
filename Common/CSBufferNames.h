#pragma once

enum class MoveCS
{
	animationIndices = 0,
	objsPoses,
	objsDirections,
	time,
	targetEnemyPos,
	attackedCount,
	isDead,
	herdReachedDestination,
	herdAttackingCounts,
	objsCollisionStatus,
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
	animationIndices = 0,
	prevAnimationIndex,
	needReset,
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