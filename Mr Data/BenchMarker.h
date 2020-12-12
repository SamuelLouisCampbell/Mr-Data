#pragma once
#include <chrono>
#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include <algorithm>
#include <thread>

struct ProfileResult
{
    std::string name;
    long long start;
    long long end;
    uint32_t threadID;
};

class InstrumentationSession
{
public:
    InstrumentationSession(const std::string name_in)
        :
        name(name_in)
    {}
    std::string name;
};

class Instrumentor
{
public:
    Instrumentor() {};
    void BeginSession(const std::string name, const std::string fileName = "results.json")
    {
        outputStream.open(fileName);
        WriteHeader();
        currentSession = new InstrumentationSession(name);
    }
    void EndSession()
    {
        WriteFooter();
        outputStream.close();
        delete currentSession;
        currentSession = nullptr;
        profileCount = 0;
    }
    void WriteProfile(const ProfileResult& result)
    {
        if (profileCount++ > 0)
            outputStream << ",";

        std::string name = result.name;
        std::replace(name.begin(), name.end(), '"', '\'');

        outputStream << "{";
        outputStream << "\"cat\":\"function\",";
        outputStream << "\"dur\":" << (result.end - result.start) << ',';
        outputStream << "\"name\":\"" << name << "\",";
        outputStream << "\"ph\":\"X\",";
        outputStream << "\"pid\":0,";
        outputStream << "\"tid\":\"" << result.threadID << "\",";
        outputStream << "\"ts\":" << result.start;
        outputStream << "}";

        outputStream.flush();
    }

    void WriteFooter()
    {
        outputStream << "]}";
        outputStream.flush();
    }
    void WriteHeader()
    {
        outputStream << "{\"otherData\": {},\"traceEvents\":[";
        outputStream.flush();
    }
    static Instrumentor& Get()
    {
        static Instrumentor* instance = new Instrumentor();
        return *instance;
    }
private:
    InstrumentationSession* currentSession = nullptr;
    std::ofstream outputStream;
    int profileCount = 0;
};

class InstrumentationTimer
{
public:
    InstrumentationTimer(const char* funcName = "Unnamed")
        :
        functionName(funcName)
    {
        startPoint = std::chrono::high_resolution_clock::now();
    }
    ~InstrumentationTimer()
    {
        Stop();
    }
    void Stop()
    {
        auto endPoint = std::chrono::high_resolution_clock::now();
        auto start = std::chrono::time_point_cast<std::chrono::microseconds>(startPoint).time_since_epoch().count();
        auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endPoint).time_since_epoch().count();
        uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
        Instrumentor::Get().WriteProfile({ functionName, start, end, threadID });

    }
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> startPoint;
    const char* functionName;
};

#define PROFILING false
#if PROFILING
#define PROFILE_SCOPE(name) InstrumentationTimer timer##__LINE__(name)
#define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCSIG__)
#else
#define PROFILE_SCOPE(name)
#endif
