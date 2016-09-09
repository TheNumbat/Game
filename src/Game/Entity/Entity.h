#pragma once

#include "common.h"
#include <engine.h>
#include "..\Map\Map.h"

struct game;

// Decided to do no ineritance component based system. 
	// Subject to change - could use single inheritance, inherited components, tree inheritance...?

enum c_type : u8 {
	ct_pos = 1 << 0,
	ct_mov = 1 << 1,
	ct_tex = 1 << 2,
	ct_text = 1 << 3,
	ct_phys = 1 << 4,

	ct_none = 0xff
};

struct c_pos {
	mpos pos;
};

struct c_mov {
	v2<r32> velocity;
	v2<r32> accel;
};

struct c_tex {

};

struct c_text {

};

struct c_phys {

};

struct component {
	component(c_type t, void* data);
	c_type type;
	union {
		void* any;

		// ptrs to components held in manager
		c_pos* pos;
		c_mov* mov;
		c_tex* tex;
		c_text* text;
		c_phys* phys;
	};
};

typedef u32 entity;
typedef std::map<c_type, component> entitydata;

class entityMgr {
public:
	entityMgr();
	~entityMgr();

	entity create();
	component getC(entity e, c_type type);
	component addC(entity e, c_type type);

private:
	u32 lastUID;

	// TODO: unordered or no or yes?
	std::unordered_map<entity, entitydata> edata;

	// AOS entity components - will be accessed by different systems all at once so this should be cache efficient...I think
		// TODO: there's no good way to deleting stuff from here. Free list?
	std::vector<c_pos> pos;
	std::vector<c_mov> mov;
	std::vector<c_tex> tex;
	std::vector<c_text> text;
	std::vector<c_phys> phys;
};