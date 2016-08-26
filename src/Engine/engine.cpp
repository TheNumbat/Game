
#include "common.h"

#include "engine.h"

#include <SDL.h>

engine::engine()
{
	logInit();
	logSetContext("ENGINE");
	logMsg("Initializing engine...");
	logEnterSec();

	logMsg("Done initializing engine.");
	logExitSec();
}


engine::~engine()
{
	logSetContext("ENGINE");
	logMsg("Shutting down engine...");
	logEnterSec();
	
	logMsg("Done shutting down engine.");
	logExitSec();
	logQuit();
}
