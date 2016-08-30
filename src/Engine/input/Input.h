#pragma once

#include "..\common.h"
#include "..\log\log.h"

// TODO: controllers (joystick)
enum eventtype {
	evt_bad,
	evt_endstream,
	evt_window,
	evt_key,
	evt_mouse,
	evt_quit,
	evt_text
};

enum key {
	key_0,
	key_1,
	key_2,
	key_3,
	key_4,
	key_5,
	key_6,
	key_7,
	key_8,
	key_9,
	key_a,
	key_b,
	key_c,
	key_d,
	key_e,
	key_f,
	key_g,
	key_h,
	key_i,
	key_j,
	key_k,
	key_l,
	key_m,
	key_n,
	key_o,
	key_p,
	key_q,
	key_r,
	key_s,
	key_t,
	key_u,
	key_v,
	key_w,
	key_x,
	key_y,
	key_z,
	key_tab,
	key_grave,
	key_minus,
	key_comma,
	key_slash,
	key_space,
	key_plus,
	key_enter,
	key_period,
	key_rbracket,
	key_lbracket,
	key_semicolon,
	key_backslash,
	key_np_enter,
	key_np_0,
	key_np_1,
	key_np_2,
	key_np_3,
	key_np_4,
	key_np_5,
	key_np_6,
	key_np_7,
	key_np_8,
	key_np_9,
	key_np_add,
	key_np_period,
	key_np_divide,
	key_np_multiply,
	key_np_subtract,
	key_backspace,
	key_capslock,
	key_delete,
	key_down,
	key_up,
	key_left,
	key_right,
	key_end,
	key_escape,
	key_f1,
	key_f2,
	key_f3,
	key_f4,
	key_f5,
	key_f6,
	key_f7,
	key_f8,
	key_f9,
	key_f10,
	key_f11,
	key_f12,
	key_home,
	key_insert,
	key_lalt,
	key_ralt,
	key_lctrl,
	key_rctrl,
	key_lshift,
	key_rshift,
	key_numlock,
	key_pgup,
	key_pgdown,
	key_scrolllock
};

enum keyflags {
	flag_key_press = 1<<0,
	flag_key_release = 1<<1,
	flag_key_repeat = 1<<2,
	flag_key_lshift = 1<<3,
	flag_key_rshift = 1<<4,
	flag_key_shift = flag_key_rshift | flag_key_lshift,
	flag_key_lctrl = 1<<5,
	flag_key_rctrl = 1<<6,
	flag_key_ctrl = flag_key_rctrl | flag_key_lctrl,
	flag_key_lalt = 1<<7,
	flag_key_ralt = 1<<8,
	flag_key_alt = flag_key_ralt | flag_key_lalt,
	flag_key_lgui = 1<<9,
	flag_key_rgui = 1<<10,
	flag_key_gui = flag_key_rgui | flag_key_lgui,
	flag_key_numlock = 1<<11,
	flag_key_capslock = 1<<12,
	flag_key_altgr = 1<<13
};

enum mouseflags {
	flag_mouse_press = 1<<0,
	flag_mouse_release = 1<<1,
	flag_mouse_wheel = 1<<2,
	flag_mouse_button = flag_mouse_press | flag_mouse_release,
	flag_mouse_motion = 1<<4,
	flag_mouse_double = 1<<5,
	flag_mouse_lclick = 1<<6,
	flag_mouse_rclick = 1<<7,
	flag_mouse_mclick = 1<<8,
	flag_mouse_x1click = 1<<9,
	flag_mouse_x2click = 1<<10,
	flag_mouse_click = flag_mouse_rclick | flag_mouse_lclick | flag_mouse_mclick | flag_mouse_x1click | flag_mouse_x2click
};

enum windowflags {
	flag_window_resized = 1<<0,
	flag_window_moved = 1<<1,
	flag_window_shown = 1<<2,
	flag_window_hidden = 1<<3,
	flag_window_exposed = 1<<4,
	flag_window_minimized = 1<<5,
	flag_window_maximized = 1<<6,
	flag_window_restored = 1<<7,
	flag_window_enter = 1<<8,
	flag_window_leave = 1<<9,
	flag_window_focused = 1<<10,
	flag_window_unfocused = 1<<11,
	flag_window_close = 1<<12
};

struct event {
	event() { type = evt_bad; }
	virtual ~event() {}

	eventtype type;
};

struct event_endstream : public event {
	event_endstream() { type = evt_endstream; }
	~event_endstream() {}
};

struct event_window : public event {
	event_window() { type = evt_window; }
	~event_window() {}

	u32 x, y;
	u16 flags;
};

struct event_key : public event {
	event_key() { type = evt_key; }
	~event_key() {}

	key k;
	u16 flags;
};

struct event_mouse : public event {
	event_mouse() { type = evt_mouse; }
	~event_mouse() {}

	u32 x, y;
	u16 flags;
};

struct event_quit : public event {
	event_quit() { type = evt_quit; }
	~event_quit() {}
};

struct event_text : public event {
	event_text() { type = evt_text; }
	~event_text() {}

	std::string text;
};

class Input {
public:
	ENGINE_API Input();
	ENGINE_API ~Input();

	bool init();
	bool kill();

	// straight up allocates a new event, need to delete
		// TODO: should this use region based memory?
	ENGINE_API event* getNext();

	ENGINE_API bool testKey(key k);

	ENGINE_API bool copy(const std::string& text);
	ENGINE_API bool paste(std::string& text);

	ENGINE_API void startTextIn();
	ENGINE_API void endTextIn();

private:
	event* translateWindow(void* sdl_ev);
	event* translateKey(void* sdl_ev);
	event* translateMouse(void* sdl_ev);
	event* translateText(void* sdl_ev);

	u8* sdl_kbstate;
	bool good;
};
