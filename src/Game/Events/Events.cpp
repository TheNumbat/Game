#pragma once

#include "..\game.h"
#include "Events.h"

Events::Events(engine* _e, game* _g) {
	e = _e;
	g = _g;
	state = input_game;
}

Events::~Events() {}

void Events::handleEvents() {

}

void Events::clearStrBuf() {

}

void Events::setState(input_state s) {

}

void Events::handleGame(event* e) {

}

void Events::handleProfiler(event* e) {

}

void Events::handleConsole(event* e) {

}

void Events::screenText(event_text* e, const std::string& exclude) {

}
