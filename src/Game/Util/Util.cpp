#pragma once

#include "..\game.h"
#include "Util.h"

#undef beginFunc()
#undef trackVal()
#undef releaseVal()

Util::profNode::profNode(const std::string& n, u64 s, profNode* p) {
	name = n;
	start = s;
	parent = p;

	showChildren = false;
	self = 0;
	heir = 0;
	calls = 1;
}

Util::profNode::~profNode() {
	for (auto entry : children)
		delete entry.second;
}

Util::Util(engine* _e, game* _g) {
	logSetContext("DEBUG");

	e = _e;
	g = _g;

	e->time.addPerfCounter("debug");

	toggleProf = false;
	profilerPaused = false;
	fpsCap = 0;
	flags = 0;
	lastFrameTime = 0;
	totalFrameTime = 0;
	totalFrames = 0;

	trackVal("lastFrameTime", &lastFrameTime);

	head = new profNode("profiler", 0);
	current = head;
	selected = head;

	logInfo("Debug system initialized.");
}

Util::~Util() {
	e->time.remove("debug");
	delete head;
}

bool Util::callConsoleFunc(const std::string& input) {
	return false;
}

void Util::reloadConsoleFuncs() {

}

void Util::beginFrame() {
	e->time.reset("debug");
}

void Util::endFrame() {
	logSetContext("DEBUG");

	if (toggleProf) {
		toggleProf = false;
		profilerPaused = !profilerPaused;
	}

	if (fpsCap) {
		// TODO: actually do something, don't waste CPU time
		while(e->time.get("debug") < e->time.getPerfFreq() / fpsCap);
	}

	lastFrameTime = e->time.get("debug");
	totalFrameTime += lastFrameTime;
	totalFrames++;

	if (fpsCap && lastFrameTime > e->time.getPerfFreq() / (fpsCap - 1)) {
		logWarn("Last frame took " + std::to_string(1000.0f * lastFrameTime / (r64) e->time.getPerfFreq()) + " ms!");
	}

	if (!profilerPaused) {
		current = head;
		resetProfNodesRec(current);
	}
}

void Util::beginFunc(const std::string& name) {
	if (profilerPaused) return;

	u64 start = e->time.get("debug");

	auto entry = current->children.find(name);
	if (entry == current->children.end()) {
		profNode* newNode = new profNode(name, start, current);
		current->children.insert({ name, newNode });
		current = newNode;
		return;
	}

	current = entry->second;
	current->calls++;
	current->start = start;
}

void Util::endFunc() {
	if (profilerPaused) return;

	u64 end = e->time.get("debug");

	current->heir += end - current->start;

	u64 timeExceptSelf = 0;
	for (auto entry : current->children) {
		timeExceptSelf += entry.second->heir;
	}

	current->self = current->heir - timeExceptSelf;

	assert(current->parent);
	if (current->parent) {
		current = current->parent;
	}
}

template<typename T>
bool Util::trackVal(const std::string& ID, T* val) {
	return false;
}

bool Util::releaseVal(const std::string& ID) {
	return false;
}

void Util::resetAvgFrame() {

}

u64 Util::getAvgFrame() {
	return 0;
}

u64 Util::getLastFrame() {
	return 0;
}

void Util::setFpsCap(u16 fps) {

}

bool Util::getFlag(debug_flag f) {
	return false;
}

void Util::setFlag(debug_flag f) {

}

void Util::clearFlag(debug_flag f) {

}

void Util::toggleFlag(debug_flag f) {

}

void Util::toggleProfiler() {
	toggleProf = true;
}

void Util::selectedNodeUp() {

}

void Util::selectedNodeDown() {

}

void Util::resetProfNodesRec(profNode* place) {
	for (auto entry : place->children) {
		resetProfNodesRec(entry.second);
	}

	place->start = 0;
	place->self = 0;
	place->heir = 0;
	place->calls = 0;
}