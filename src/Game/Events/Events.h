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
	void clearLog();
	void setState(input_state s);

private:
	void handleGame(event* ev);
	void handleProfiler(event* ev);
	void handleConsole(event* ev);

	void screenText(event_text* ev, const std::string& exclude);

	std::string inStr;
	s16 logLoc;
	std::vector<std::string> chatLog;
	static const u32 MAX_CHAT = 100;

	input_state state;

	engine* e;
	game* g;

	friend class Render;
};

