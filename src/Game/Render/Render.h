#pragma once

#include "common.h"
#include <engine.h>

struct game;

class Render {
public:
	Render(engine* _e, game* _g);
	~Render();

private:
	engine* e;
	game* g;
};

