#pragma once
#include <chrono>
#include <string>
#include <iomanip>
#include "BenchMarker.h"

class Time
{
public:
	Time()
	{
		last = std::chrono::steady_clock::now();
	}
	float Mark()
	{
		const auto old = last;
		last = std::chrono::steady_clock::now();
		const std::chrono::duration<float, std::milli> elapsedTime = last - old;
		return elapsedTime.count();
	}

	float Peek()
	{
		return std::chrono::duration<float>(
			std::chrono::steady_clock::now() -
			last).count();
	}
private:
	std::chrono::steady_clock::time_point last;
	std::chrono::steady_clock::time_point now;
};

class FPS
{
public:
	void Update(float deltaTime)
	{
		PROFILE_FUNCTION();
		fpsCount++;
		accumulatedTime += deltaTime;
		if (accumulatedTime > 1000.0f)
		{
			fps = fpsCount;
			fpsCount = 0;
			accumulatedTime = 0;	
		}
	}
	int Get() const
	{
		return fps;
	}
private:
	int fps = 0;
	int fpsCount = 0;
	float accumulatedTime = 0.0f;
};