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
	void handleGame(event* e);
	void handleProfiler(event* e);
	void handleConsole(event* e);

	void screenText(event_text* e, const std::string& exclude);

	std::string inStr;
	std::vector<std::string> inStrBuf;

	input_state state;

	engine* e;
	game* g;
};

