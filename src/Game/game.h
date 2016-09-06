#pragma once

#include "common.h"
#include <engine.h>

#include "Events\Events.h"
#include "Util\Util.h"
#include "Map\Map.h"
#include "Render\Render.h"

struct game {
	game(engine* _e);
	~game();
	
	bool run();
	void startReload();
	void endReload();

	Events events;
	Util debug;
	Map map;
	Render ren;

	bool running;
	bool runThreads;
	engine* e;
};
