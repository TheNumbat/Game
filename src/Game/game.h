#pragma once

#include "common.h"
#include <engine.h>

class game {
public:
	game(engine* _e);
	~game();
	
	bool run();
	void startReload();
	void endReload();

	bool running;
	bool runThreads;
	engine* e;
};
