
#include "common.h"

#include "engine.h"

#include <SDL.h>

engine::engine()
{
	logInit();
	logSetContext("ENGINE");
	logMsg("Initializing engine...");
	logEnterSec();

	gfx.init("Game",1280,720);
	audio.init();

	logSetContext("ENGINE");
	logMsg("Done initializing engine.");
	logExitSec();
}


engine::~engine()
{
	logSetContext("ENGINE");
	logMsg("Shutting down engine...");
	logEnterSec();
	
	gfx.kill();
	audio.kill();

	logSetContext("ENGINE");
	logMsg("Done shutting down engine.");
	logExitSec();
	logQuit();
}
