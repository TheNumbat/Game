#pragma once

#include "common.h"
#include <engine.h>

struct game;

class Map {
public:
	Map(engine* _e, game* _g);
	~Map();
	
private:

	engine* e;
	game* g;
};

