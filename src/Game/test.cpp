
#include <engine.h>
#include <iostream>
#include <string>

bool run = true;

s32 threadTest(void* data) {
	engine* e = (engine*)data;
	while (run) {
		e->thread.delay(1000);
	}
	return 0;
}

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

	e->thread.add("test", &threadTest, e);

	bool cont = true;
	while (cont) {
		event* evt = e->input.getNext();
		if (evt->type == evt_quit) cont = false;
		delete evt;
	}

	run = false;
	int ret;
	e->thread.wait("test",ret);

	logSetContext("GAME");
	logInfo("Destroying game...");
	logEnterSec();

	logInfo("Done destroying game.");
	logExitSec();

	delete e;

	return 0;
}
