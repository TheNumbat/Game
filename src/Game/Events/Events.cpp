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
	g->debug.beginFunc(0);

	event* ev;
	bool cont = true;
	while(cont) {
		ev = e->input.getNext(); // this can take some time
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
	g->debug.beginFunc(0);
	logSetContext("EVENT");

	switch (ev->type) {
		case evt_key:
			event_key* evk = (event_key*) ev;
			if (evk->flags & flag_key_press) {
				switch (evk->k) {
					case key_minus:
						g->ren.zOut(2);
						break;
					case key_plus:
						g->ren.zIn(2);
						break;
					case key_p:
						break;
					case key_grave:
						g->debug.toggleFlag(renderDebugUI);
						state = input_profiler;
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
	g->debug.beginFunc(0);
	logSetContext("EVENTS");
	
	if (ev->type == evt_window) {
		event_window* ew = (event_window*) ev;
		if (ew->flags & flag_window_resized) {
			u32 w = ew->x, h = ew->y;
			logInfo("New screen w: " + std::to_string(w) + " h: " + std::to_string(h));
		}
	}
	else if (ev->type == evt_mouse) {
		event_mouse* ew = (event_mouse*) ev;
		if (ew->flags & flag_mouse_press) {
			u32 x = ew->x, y = ew->y;
			logInfo("Mouse x: " + std::to_string(x) + " y: " + std::to_string(y));
		}
	}
	else if (ev->type == evt_key) {
		event_key* ek = (event_key*) ev;
		if (ek->flags & flag_key_press) {
			switch (ek->k) {
				case key_up:
					g->debug.selectedNodeUp();
					break;
				case key_down:
					g->debug.selectedNodeDown();
					break;
				case key_enter:
					g->debug.toggleChildren();
					break;
				case key_grave:
					g->debug.toggleFlag(renderDebugUI);
					state = input_game;
					break;
				case key_tab:
					state = input_console;
					e->input.startTextIn();
					break;
				case key_p:
					g->debug.toggleProfiler();
					break;
			}
		}
		else if (ek->flags & flag_key_repeat) {
			switch (ek->k) {
				case key_up:
					g->debug.selectedNodeUp();
					break;
				case key_down:
					g->debug.selectedNodeDown();
					break;
			}
		}
	}

	g->debug.endFunc();
}

void Events::handleConsole(event* ev) {
	g->debug.beginFunc(0);

	if (ev->type == evt_key) {
		event_key* ek = (event_key*) ev;
		if (ek->flags & flag_key_press) {
			if (ek->k == key_backspace && inStr.length()) {
				inStr.pop_back();
			} else if (ek->k == key_enter) {
				if (inStr == "reload") {
					g->debug.reloadConsoleFuncs();
					inStr = "";
				} else if (inStr == "exit") {
					g->debug.clearFlag(renderDebugUI);
					state = input_game;
					e->input.endTextIn();
					inStr = "";
				} else {
					if (g->debug.callConsoleFunc(inStr + " ")) {
						inStr = "";
					}
				}
			} else if (ek->k == key_tab) {
				state = input_profiler;
				e->input.endTextIn();
			} else if (ek->k == key_grave) {
				g->debug.clearFlag(renderDebugUI);
				state = input_game;
				e->input.endTextIn();
			}

			if (ek->flags & flag_key_ctrl) {
				switch (ek->k) {
					case key_v:
						e->input.paste(inStr);
						break;
					case key_c:
						e->input.copy(inStr);
						break;
				}
			}
		} else if (ek->flags & flag_key_repeat) {
			if (ek->k == key_backspace && inStr.length()) {
				inStr.pop_back();
			}
		}
	} else if (ev->type == evt_text) {
		screenText((event_text*) ev,"`");
	}

	g->debug.endFunc();
}

void Events::screenText(event_text* ev, const std::string& exclude) {
	if (ev->text.find_first_of(exclude) != std::string::npos) {
		return;
	}
	inStr += ev->text;
}
