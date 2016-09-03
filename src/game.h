#pragma once

#include "common.h"

#include "log\log.h"
#include "audio\audio.h"
#include "file\file.h"
#include "gfx\gfx.h"
#include "input\input.h"
#include "time\time.h"
#include "thread\thread.h"

class game {
public:
	game();
	~game();
	
	bool run();
	void startReload();
	void endReload();

	bool running;
	bool runThreads;

	Audio audio;
	File file;
	Gfx gfx;
	Input input;
	Time time;
	Thread thread;
	Log log;
};
