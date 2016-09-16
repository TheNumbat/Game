#pragma once

#include "common.h"
#include <engine.h>

struct game;

enum input_state : u8 {
	input_none,
	input_game,
	input_profiler,
	input_console
};

class Events {
public:
	Events(engine* _e, game* _g);
	~Events();

	void handleEvents();
	void clearStrBuf();
	void setState(input_state s);

private:
	void handleGame(event* ev);
	void handleProfiler(event* ev);
	void handleConsole(event* ev);

	void screenText(event_text* ev, const std::string& exclude);

	std::string inStr;
	std::vector<std::string> inStrBuf;

	input_state state;

	engine* e;
	game* g;

	friend class Render;
};

