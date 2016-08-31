
#include "common.h"

#include "engine.h"

#include <SDL.h>

engine::engine() : log(this)
{
	globalLog = &log;
	log.init();

	logSetContext("ENGINE");
	logInfo("Initializing engine...");
	logEnterSec();

	logInfo("Initializing SDL");
	bool result = SDL_Init(0) == 0;
	assert(result);
	if (!result) {
		logErr((std::string) "Failed to initialize SDL! Error: " + SDL_GetError());
		logExitSec();
		return;
	}

	gfx.init("Game",1280,720);
	audio.init();
	time.init();
	file.init();
	input.init();
	thread.init();

	logInfo("Done initializing engine.");
	logExitSec();
}


engine::~engine()
{
	logSetContext("ENGINE");
	logInfo("Shutting down engine...");
	logEnterSec();
	
	gfx.kill();
	audio.kill();
	time.kill();
	file.kill();
	input.kill();
	thread.kill();

	logInfo("Done shutting down engine.");
	log.kill();
	SDL_Quit();
}
