
#include <engine.h>
#include <iostream>
#include <string>

int main() {
	
	engine* e = new engine;

	logSetContext("GAME");
	logInfo("Initializing game...");
	logEnterSec();

	logInfo("Done initializing game.");
	logExitSec();

	e->gfx.loadTexture("test", "test.png");
	e->gfx.renderTexture("test", r2<s32>(0, 0, 1280, 720));
	e->gfx.swapFrame();

	e->audio.loadSound("test", "test.mp3");
	e->audio.play("test");

	e->file.loadFile("test", "test.png", file_binary, file_read);

	system("pause");

	logSetContext("GAME");
	logInfo("Destroying game...");
	logEnterSec();

	logInfo("Done destroying game.");
	logExitSec();

	delete e;

	return 0;
}
