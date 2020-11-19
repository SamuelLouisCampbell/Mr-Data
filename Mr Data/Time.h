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

class Timer
{
public:
	Timer(const char* name)
		:
		m_name(name)
	{
		m_startTimePoint = std::chrono::high_resolution_clock::now();
	}
	~Timer()
	{
		if (!m_isStopped)
		{
			Stop();
		}
	}
	void Stop()
	{
		auto m_endTimePoint = std::chrono::high_resolution_clock::now();
		long long start = std::chrono::time_point_cast<std::chrono::milliseconds>(m_startTimePoint).time_since_epoch().count();
		long long end   = std::chrono::time_point_cast<std::chrono::milliseconds>(m_startTimePoint).time_since_epoch().count();

		//std::cout << m_name << " : " << (end - start) << "ms\n";

		m_isStopped = true;

	}
private:
	const char* m_name;
	std::chrono::time_point<std::chrono::steady_clock> m_startTimePoint;
	bool m_isStopped = false;

};