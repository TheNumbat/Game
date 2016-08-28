#pragma once

#include "common.h"

#include "audio\audio.h"
#include "file\file.h"
#include "gfx\gfx.h"
#include "input\input.h"
#include "time\time.h"
#include "log\log.h"
#include "thread\thread.h"

class engine
{
public:
	ENGINE_API engine();
	ENGINE_API ~engine();

	Audio audio;
	File file;
	Gfx gfx;
	Input input;
	Time time;
	Thread thread;
};
