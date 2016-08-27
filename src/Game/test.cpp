
#include <engine.h>
#include <iostream>
#include <string>

int main() {
	
	engine* e = new engine;

	logSetContext("GAME");
	logMsg("Initializing game...");
	logEnterSec();

	logMsg("Done initializing game.");
	logExitSec();


	e->gfx.loadTexture("test", "test.png");
	e->gfx.renderTexture("test", r2<s32>(0, 0, 1280, 720));
	e->gfx.swapFrame();

	e->audio.loadSound("test", "test.mp3");
	e->audio.play("test");

	e->time.addPerfCounter("test");
	for (int i = 0; i < 10; i++) {
		logMsg(std::to_string(e->time.get("test")));
	}

	system("pause");

	logMsg("Deinitializing game...");
	logEnterSec();

	logMsg("Done deinitializing game.");
	logExitSec();

	delete e;

	return 0;
}
