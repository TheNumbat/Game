#pragma once

#include "common.h"

#include "log\log.h"
#include "audio\audio.h"
#include "file\file.h"
#include "gfx\gfx.h"
#include "input\input.h"
#include "time\time.h"
#include "thread\thread.h"

struct engine
{
	ENGINE_API engine();
	ENGINE_API ~engine();

	ENGINE_API bool init(const std::string& wTitle, s32 wW, s32 wH);
	ENGINE_API bool kill();

	Audio audio;
	File file;
	Gfx gfx;
	Input input;
	Time time;
	Thread thread;
	Log log;
};
