#pragma once
#include <chrono>
#include <string>
#include <iomanip>

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