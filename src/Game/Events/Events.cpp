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
	g->debug.beginFunc();

	event* ev;
	bool cont = true;
	while(cont) {
		ev = e->input.getNext();
		switch (ev->type) {
			case evt_bad: continue;
			case evt_endstream: cont = false;  break;
			case evt_quit: g->running = false; cont = false; break;
			default:
				switch (state) {
					case input_game: handleGame(ev); break;
					case input_profiler: handleProfiler(ev); break;
					case input_console: handleConsole(ev); break;
				}
		}
		delete ev;
	}

	g->debug.endFunc();
}

void Events::clearStrBuf() {
	inStrBuf.clear();
}

void Events::setState(input_state s) {
	state = s;
}

void Events::handleGame(event* ev) {
	g->debug.beginFunc();
	logSetContext("EVENT");

	switch (ev->type) {
		case evt_key:
			event_key* evk = (event_key*) ev;
			if (evk->flags & flag_key_press) {
				switch (evk->k) {
					case key_minus:
						g->ren.zIn(2);
						break;
					case key_plus:
						g->ren.zOut(2);
						break;
					case key_p:
						break;
					case key_grave:
						break;
				}
			} else if (evk->flags & flag_key_release) {
				switch (evk->k) {
					case key_a:
					case key_d:
					case key_left:
					case key_right:
						break;
					case key_w:
					case key_s:
					case key_up:
					case key_down:
						break;
				}
			} else if (evk->flags & flag_key_repeat) {

			}
			break;
	}

	g->debug.endFunc();
}

void Events::handleProfiler(event* ev) {
	g->debug.beginFunc();

	g->debug.endFunc();
}

void Events::handleConsole(event* ev) {
	g->debug.beginFunc();

	g->debug.endFunc();
}

void Events::screenText(event_text* ev, const std::string& exclude) {
	if (ev->text.find_first_of(exclude) != std::string::npos) {
		return;
	}
	inStr += ev->text;
}
