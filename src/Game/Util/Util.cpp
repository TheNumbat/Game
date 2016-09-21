#pragma once

#include "..\game.h"
#include "..\console.h"
#include "Util.h"
#include <sstream>

#undef beginFunc
#undef trackVal
#undef releaseVal
#undef addVal

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

	selected.first = 0;
	selected.second = NULL;

	reloadConsoleFuncs();

	logInfo("Debug system initialized.");
}

Util::~Util() {
	e->time.remove("debug");
	for (auto p : heads) {
		delete p.second;
	}
	heads.clear();
	currents.clear();
}

bool Util::callConsoleFunc(const std::string& input) {
	logSetContext("DEBUG");

	std::stringstream ss(input);
	std::string name;
	ss >> name;
	if (ss.fail()) {
		logWarn("Failed to parse func name from input.");
		return false;
	}

	std::string arg = input.substr(name.size(), input.size());
	name = "console::" + name;

	auto entry = consoleFuncs.find(name);
	if (entry == consoleFuncs.end()) {
		logWarn("Failed to find funtion name " + name);
		return false;
	}

	// function will log error
	return (*entry->second)(g, e, arg);
}

#define ADD_FUNC(name) consoleFuncs.insert({#name,&name})
void Util::reloadConsoleFuncs() {
	consoleFuncs.clear();

	// Add all console functions here
	ADD_FUNC(console::quit);
	ADD_FUNC(console::log);
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

	u64 perf = e->time.getPerfFreq();
	if (fpsCap) {
		// TODO: actually do something or at least make blocking more accurate
		/*if (e->time.get("debug") < perf / (fpsCap - 1)) {
			r32 msperframe = 1000.0f / (fpsCap - 1);
			r32 mselapsed = 1000.0f * e->time.get("debug") / perf;
			u8 wait = std::round(msperframe - mselapsed);
			e->thread.delay(wait <= 2 ? wait : wait - 2 );
		}*/
	}

	lastFrameTime = e->time.get("debug");
	totalFrameTime += lastFrameTime;
	totalFrames++;

	if (fpsCap && lastFrameTime > perf / fpsCap) {
		logWarn("Last frame took " + std::to_string(1000.0f * lastFrameTime / (r64) perf) + " ms!");
		releaseVal("overTime");
		releaseVal("overFrame");
		addVal("overTime", 1000.0f * lastFrameTime / (r64) perf);
		addVal("overFrame", lastFrameTime);
		toggleProfiler();
	}

	if (!profilerPaused) {
		for (auto head : heads) {
			resetProfNodesRec(head.second);
		}
	}
}

void Util::toggleChildren() {
	selected.second->showChildren = !selected.second->showChildren;
}

void Util::beginThreadProf(const std::string& name, u8 thread, bool select) {
	if (heads.find(thread) == heads.end()) {
		profNode* head = new profNode(name, 0);
		heads.insert({ thread, head });
		currents.insert({ thread, head });
		if (select) selected.second = head;
	} else {
		assert(false);
	}
}

void Util::beginFunc(const std::string& name, u8 thread) {
	if (profilerPaused) return;

	u64 start = e->time.get("debug");

	auto centry = currents.find(thread);
	assert(centry != currents.end());

	auto entry = centry->second->children.find(name);
	if (entry == centry->second->children.end()) {
		profNode* newNode = new profNode(name, start, centry->second);
		centry->second->children.insert({ name, newNode });
		centry->second = newNode;
		return;
	}

	centry->second = entry->second;
	centry->second->calls++;
	centry->second->start = start;
}

void Util::endFunc(u8 thread) {
	if (profilerPaused) return;

	u64 end = e->time.get("debug");

	auto centry = currents.find(thread);
	assert(centry != currents.end());

	centry->second->heir += end - centry->second->start;

	u64 timeExceptSelf = 0;
	for (auto entry : centry->second->children) {
		timeExceptSelf += entry.second->heir;
	}

	centry->second->self = centry->second->heir - timeExceptSelf;

	assert(centry->second->parent);
	if (centry->second->parent) {
		centry->second = centry->second->parent;
	}
}

template<typename T>
bool Util::trackVal(const std::string& ID, T* val) {
	logSetContext("DEBUG");

	if (values.find(ID) != values.end()) {
		logWarn("Value ID " + ID + "already in use!");
		return false;
	}

	values.insert({ ID, new trackedVal<T>(val) });
	return true;
}

template<typename T>
bool Util::addVal(const std::string& ID, T val) {
	logSetContext("DEBUG");

	if (values.find(ID) != values.end()) {
		logWarn("Value ID " + ID + "already in use!");
		return false;
	}

	values.insert({ ID, new staticVal<T>(val) });
	return true;
}

bool Util::releaseVal(const std::string& ID) {
	logSetContext("DEBUG");

	auto entry = values.find(ID);
	if (entry == values.end()) {
		logWarn("Value ID " + ID + "not found!");
		return false;
	}

	delete entry->second;
	values.erase(entry);
	return true;
}

void Util::resetAvgFrame() {
	totalFrames = 0;
	totalFrameTime = 0;
}

u64 Util::getAvgFrame() {
	return totalFrameTime / totalFrames;
}

u64 Util::getLastFrame() {
	return lastFrameTime;
}

void Util::setFpsCap(u8 fps) {
	fpsCap = fps;
}

#pragma warning (disable : 4800)
bool Util::getFlag(debug_flag f) {
	return flags & (u16) f;
}

void Util::setFlag(debug_flag f) {
	flags |= (u16) f;
}

void Util::clearFlag(debug_flag f) {
	flags &= ~((u16) f);
}

void Util::toggleFlag(debug_flag f) {
	if (getFlag(f)) {
		clearFlag(f);
	} else {
		setFlag(f);
	}
}

void Util::toggleProfiler() {
	toggleProf = true;
}

// TODO: test this
void Util::selectedNodeUp() {
	if (selected.second != heads.find(selected.first)->second) {
		// If selected: first node of children
		if (selected.second->parent->children.begin()->second == selected.second) {
			selected.second = selected.second->parent;
		// Else move up a child
		} else {
			// Iterator to previous child
			auto temp = std::prev(selected.second->parent->children.find(selected.second->name));
			selected.second = temp->second;
			// Move to bottom shown node under temp
			while (selected.second->showChildren && selected.second->children.size()) {
				selected.second = std::prev(selected.second->children.end())->second;
			}
		}
	} else {
		// Move up in threads
		if (heads.find(selected.first) == heads.begin()) {
			selected.second = std::prev(heads.end())->second;
			selected.first = std::prev(heads.end())->first;
		} else {
			selected.second = std::prev(heads.find(selected.first))->second;
			selected.first = std::prev(heads.find(selected.first))->first;
		}
		// Move to bottom shown node
		while (selected.second->showChildren && selected.second->children.size()) {
			selected.second = std::prev(selected.second->children.end())->second;
		}
	}
}

// TODO: test this
void Util::selectedNodeDown() {
	// Move to first child if possible
	if (selected.second->showChildren && selected.second->children.size()) {
		selected.second = selected.second->children.begin()->second;
	} else if (selected.second != heads.find(selected.first)->second) {
		bool found = false;
		while (!found) {
			if (selected.second == heads.find(selected.first)->second) {
				if (heads.find(selected.first) == std::prev(heads.end())) {
					selected.second = heads.begin()->second;
					selected.first = heads.begin()->first;
				} else {
					selected.second = std::next(heads.find(selected.first))->second;
					selected.first = std::next(heads.find(selected.first))->first;
				}
				break;
			}
			// If the parent was not at the end of its parent's children we've found where we want
			if (std::distance(selected.second->parent->children.find(selected.second->name), selected.second->parent->children.end()) > 1) {
				selected.second = std::next(selected.second->parent->children.find(selected.second->name))->second;
				found = true;
			}
			if (!found) {
				selected.second = selected.second->parent;
			}
		}
	} else {
		if (heads.find(selected.first) == std::prev(heads.end())) {
			selected.second = heads.begin()->second;
			selected.first = heads.begin()->first;
		} else {
			selected.second = std::next(heads.find(selected.first))->second;
			selected.first = std::next(heads.find(selected.first))->first;
		}
	}
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