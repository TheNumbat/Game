#pragma once

#include "..\common.h"
#include <map>

class timer {
	timer(u64 s, u64 p, bool perf);
	~timer();

	u64 start;
	u64 pause;
	u64 lag;
	bool isPerfCounter;

	friend class Time;
};

class Time
{
public:
	Time();
	~Time();

	bool init();
	bool kill();

	bool addTimer(const std::string& tID, bool startPaused = false);
	bool addPerfCounter(const std::string& tID, bool startPaused = false);

	bool remove(const std::string& tID);

	bool addCallback(u32(*callback)(u32, void*), u32 delay, void* param, bool save = false, const std::string& cID = "");
	bool removeCallback(const std::string& cID);

	bool pause(const std::string& tID);
	bool resume(const std::string& tID);
	bool reset(const std::string& tID);
	bool toggle(const std::string& tID);

	u64 get(const std::string& tID);

	u64 getPerfFreq();
	u64 ticksSinceStart();
	u64 perfSinceStart();

private:
	// TODO: region allocation, free list
	std::map<std::string, timer*> timers;
	std::map<std::string, u32> callbacks;
	bool good;
};

