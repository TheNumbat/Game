#pragma once

#include "common.h"
#include <engine.h>

class game {
public:
	game(engine* _e);
	~game();

	bool running;
	engine* e;
};
