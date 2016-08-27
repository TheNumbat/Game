#include "time.h"

#include "..\log\log.h"
#include <SDL.h>

timer::timer(u64 s, u64 p, bool perf) {
	start = s;
	pause = p;
	isPerfCounter = perf;
	lag = 0;
}

timer::~timer() {}

Time::Time() {
	good = false;
}

Time::~Time() {
	for (auto i : timers)
		delete i.second;
	for (auto i : callbacks)
		removeCallback(i.first);
}

bool Time::init() {
	logSetContext("TIME");
	
	if (good) {
		logWarn("Time already initialized");
		return false;
	}

	logMsg("Initializing timer...");
	logEnterSec();

	logMsg("Initialzing SDL timer subsystem");
	int result = SDL_InitSubSystem(SDL_INIT_TIMER);
	assert(result == 0);
	if (result != 0) {
		logErr((std::string) "Failed to initialize SDL timer, Error: " + SDL_GetError());
		return false;
	}

	logMsg("Done initializing timer.");
	logExitSec();
	good = true;
	return true;
}

bool Time::kill() {
	logSetContext("TIME");
	
	if (!good) {
		logWarn("Timer already deinitialized");
		return false;
	}

	logMsg("Deinitializing timer...");
	logEnterSec();

	logMsg("Quitting timer subsystem");
	SDL_QuitSubSystem(SDL_INIT_TIMER);

	logMsg("Done deinitializing timer.");
	logExitSec();

	good = false;
	return true;
}

bool Time::addTimer(const std::string& tID, bool startPaused) {
	logSetContext("TIME");
	
	if (timers.find(tID) != timers.end()) {
		logWarn("Timer ID: " + tID + " already in use!");
		return false;
	}

	u64 currentTime = ticksSinceStart();
	timer* newT = new timer(currentTime, startPaused ? currentTime : 0, false);
	
	timers.insert({tID, newT});
	return true;
}

bool Time::addPerfCounter(const std::string& tID, bool startPaused) {
	logSetContext("TIME");

	if (timers.find(tID) != timers.end()) {
		logWarn("Perf counter ID: " + tID + " already in use!");
		return false;
	}

	u64 currentTime = perfSinceStart();
	timer* newPC = new timer(currentTime, startPaused ? currentTime : 0, true);
	
	timers.insert({tID, newPC});
	return true;
}

bool Time::remove(const std::string& tID) {
	logSetContext("TIME");

	auto entry = timers.find(tID);
	if (entry == timers.end()) {
		logWarn("Timer / perf counter ID " + tID + " not found");
		return false;
	}

	delete entry->second;
	timers.erase(entry);
	return true;
}

bool Time::addCallback(u32(*callback)(u32, void*), u32 delay, void* param, bool save, const std::string& tID) {
	logSetContext("TIME");

	if (save && tID == "") {
		logWarn("Cannot create saved callback with no ID!");
		return false;
	}

	if (save && callbacks.find(tID) != callbacks.end()) {
		logWarn("Callback ID " + tID + " already in use");
		return false;
	}

	SDL_TimerID result = SDL_AddTimer(delay,(SDL_TimerCallback)callback,param);
	assert(result != 0);
	if (result == 0) {
		logWarn((std::string) "Could not create callback , Error: " + SDL_GetError());
		return false;
	}

	if (save) {
		callbacks.insert({tID, result});
	}
	return true;
}

bool Time::removeCallback(const std::string& tID) {
	logSetContext("TIME");

	auto entry = callbacks.find(tID);
	if (entry == callbacks.end()) {
		logWarn("Saved callback ID " + tID + " not found");
		return false;
	}

	bool result = SDL_RemoveTimer(entry->second);
	callbacks.erase(entry);
	if (!result) {
		logWarn("Could not remove saved callback ID " + tID + ", already ended.");
		return false;
	}

	return true;
}

bool Time::pause(const std::string& tID) {
	logSetContext("TIME");
	
	auto entry = timers.find(tID);
	if (entry == timers.end()) {
		logWarn("Timer / perf counter to pause ID " + tID + " not found");
		return false;
	}

	if (entry->second->pause) {
		logWarn("Timer ID " + tID + " already paused");
		return false;
	}

	if (entry->second->isPerfCounter)
		entry->second->pause = perfSinceStart() - entry->second->pause - entry->second->start;
	else
		entry->second->pause = ticksSinceStart() - entry->second->pause - entry->second->start;

	return true;
}

bool Time::resume(const std::string& tID) {
	logSetContext("TIME");

	auto entry = timers.find(tID);
	if (entry == timers.end()) {
		logWarn("Timer / perf counter to resume ID " + tID + " not found");
		return false;
	}

	if (!entry->second->pause) {
		logWarn("Timer ID " + tID + " already playing");
		return false;
	}

	if (entry->second->isPerfCounter)
		entry->second->lag = perfSinceStart() - entry->second->pause - entry->second->start;
	else
		entry->second->lag = ticksSinceStart() - entry->second->pause - entry->second->start;
	
	entry->second->pause = 0;
	return true;
}

bool Time::reset(const std::string& tID) {
	logSetContext("TIME");

	auto entry = timers.find(tID);
	if (entry == timers.end()) {
		logWarn("Timer / perf counter ID " + tID + " not found");
		return false;
	}

	if (entry->second->isPerfCounter)
		entry->second->start = perfSinceStart();
	else
		entry->second->start = ticksSinceStart();

	entry->second->pause = 0;
	entry->second->lag = 0;
	return true;
}

bool Time::toggle(const std::string& tID) {
	logSetContext("TIME");

	auto entry = timers.find(tID);
	if (entry == timers.end()) {
		logWarn("Timer / perf counter ID " + tID + " not found");
		return false;
	}

	if (entry->second->pause)
		resume(tID);
	else
		pause(tID);

	return true;
}

u64 Time::get(const std::string& tID) {
	logSetContext("TIME");

	auto entry = timers.find(tID);
	if (entry == timers.end()) {
		logWarn("Timer / perf counter to get ID " + tID + " not found");
		return -1;
	}

	if (entry->second->pause)
		return entry->second->pause;

	if (entry->second->isPerfCounter)
		return perfSinceStart() - entry->second->start - entry->second->lag;
	else
		return ticksSinceStart() - entry->second->start - entry->second->lag;
}

u64 Time::getPerfFreq() {
	return SDL_GetPerformanceFrequency();
}

u64 Time::ticksSinceStart() {
	return SDL_GetTicks();
}

u64 Time::perfSinceStart() {
	return SDL_GetPerformanceCounter();
}

