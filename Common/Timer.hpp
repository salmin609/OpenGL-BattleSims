/*
 * Author		: Ryan Kim.
 * Date			: 2023-03-17
 * Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#pragma once
#include <chrono>

class Timer
{
public:
	Timer();
	~Timer();
	bool CheckTime(float time);
private:
	std::chrono::system_clock::time_point startTime;
};

inline Timer::Timer()
{
	startTime = std::chrono::system_clock::now();
}

inline Timer::~Timer()
{
}

inline bool Timer::CheckTime(float time)
{
	const std::chrono::system_clock::time_point current = std::chrono::system_clock::now();
	const long long diff = std::chrono::duration_cast<std::chrono::milliseconds>(current - startTime).count();
	const float animationT = static_cast<float>(diff) / 1000.f;

	if (animationT > time)
	{
		startTime = std::chrono::system_clock::now();
		return true;
	}

	return false;
}
