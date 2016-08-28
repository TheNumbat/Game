
#include <engine.h>
#include <iostream>
#include <string>

s32 main() {

	engine* e = new engine;

	logSetContext("GAME");
	logInfo("Initializing game...");
	logEnterSec();

	logInfo("Done initializing game.");
	logExitSec();

	e->gfx.loadTexFolder("test/");
	e->gfx.renderTexture("test", r2<s32>(0, 0, 1280, 720));
	e->gfx.swapFrame();

	e->audio.loadSoundFolder("test/");
	e->audio.play("test");

	e->file.loadFile("test", "test/test.png", file_binary, file_read);

	bool cont = true;
	while (cont) {
		event* evt = e->input.getNext();
		if (evt->type == evt_quit) cont = false;
		delete evt;
	}

	logSetContext("GAME");
	logInfo("Destroying game...");
	logEnterSec();

	logInfo("Done destroying game.");
	logExitSec();

	delete e;

	return 0;
}
