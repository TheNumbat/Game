
#include <engine.h>
#include <iostream>

int main() {
	
	engine e;

	logSetContext("GAME");
	logMsg("Initializing game...");
	logEnterSec();

	logMsg("Done initializing game.");
	logExitSec();

	return 0;
}
