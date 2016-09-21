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

	selected = NULL;

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
	selected = NULL;
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
	}

	if (!profilerPaused) {
		for (auto head : heads) {
			resetProfNodesRec(head.second);
		}
	}
}

void Util::toggleChildren() {
	selected->showChildren = !selected->showChildren;
}

void Util::beginThreadProf(const std::string& name, u8 thread, bool select) {
	if (heads.find(thread) == heads.end()) {
		profNode* head = new profNode(name, 0);
		heads.insert({ thread, head });
		currents.insert({ thread, head });
		if (select) selected = head;
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
	if (selected != heads.find(0)->second) {
		// If selected: first node of children
		if (selected->parent->children.begin()->second == selected) {
			selected = selected->parent;
		// Else move up a child
		} else {
			// Iterator to previous child
			auto temp = std::prev(selected->parent->children.find(selected->name));
			selected = temp->second;
			// Move to bottom shown node under temp
			while (selected->showChildren && selected->children.size()) {
				selected = std::prev(selected->children.end())->second;
			}
		}
	} else {
		// Move to bottom shown node
		while (selected->showChildren && selected->children.size()) {
			selected = std::prev(selected->children.end())->second;
		}
	}
}

// TODO: test this
void Util::selectedNodeDown() {
	// Move to first child if possible
	if (selected->showChildren && selected->children.size()) {
		selected = selected->children.begin()->second;
	} else if (selected != heads.find(0)->second) {
		// Move to next child
		if (std::distance(selected->parent->children.find(selected->name), selected->parent->children.end()) > 1) {
			selected = std::next(selected->parent->children.find(selected->name))->second;
		} else {
			bool found = false;
			while (!found) {
				// Move up 
				selected = selected->parent;
				if (selected == heads.find(0)->second) {
					break;
				}
				// If the parent was not at the end of its parent's children we've found where we want
				if (std::distance(selected->parent->children.find(selected->name), selected->parent->children.end()) > 1) {
					selected = std::next(selected->parent->children.find(selected->name))->second;
					found = true;
				}
			}
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