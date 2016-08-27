#include "time.h"
#include <SDL.h>

timer::timer(u64 s, u64 p, bool perf) {

}

timer::~timer() {

}

Time::Time() {

}

Time::~Time() {

}

bool Time::init() {
	return false;
}

bool Time::kill() {
	return false;
}

bool Time::addTimer(const std::string& tID, bool startPaused = false) {
	return false;
}

bool Time::addPerfCounter(const std::string& tID, bool startPaused = false) {
	return false;
}

bool Time::remove(const std::string& tID) {
	return false;
}

bool Time::addCallback(u32(*callback)(u32, void*), u32 delay, void* param, bool save = false, const std::string& cID = "") {
	return false;
}

bool Time::removeCallback(const std::string& cID) {
	return false;
}

bool Time::pause(const std::string& tID) {
	return false;
}

bool Time::resume(const std::string& tID) {
	return false;
}

bool Time::reset(const std::string& tID) {
	return false;
}

u64 Time::get(const std::string& tID) {
	return 0;
}

u64 Time::getPerfFreq() {
	return 0;
}

u64 Time::ticksSinceStart() {
	return 0;
}

u64 Time::perfSinceStart() {
	return 0;
}

