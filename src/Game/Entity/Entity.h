#pragma once

#include "common.h"
#include <engine.h>
#include "..\Map\Map.h"
#include <map>
#include "..\sarr.h"

struct game;

// Decided to do no ineritance component based system. 
	// Subject to change - could use single inheritance, inherited components, tree inheritance...?

typedef u32 entity;
#define e_null ((entity)0)

enum c_type : u8 {
	ct_pos = 1 << 0,
	ct_tex = 1 << 1,
	ct_text = 1 << 2,
	ct_phys = 1 << 3,

	ct_none = 0xff
};

struct c_pos {
	mpos pos;
};

struct c_tex {
	c_tex();
	c_tex(const c_tex& src);
	c_tex& operator=(const c_tex& src);

	std::string ID;
	s16 layer;
	r2<r32> posRect; // meters
	r2<s32> srcPxlRect; // pixels
	v2<r32> rotPt; // meters
	r32 rot; // radians
	flipmode flip;
	color mod;
	blendmode blend;
};

struct c_text : public c_tex { // inherited just for member reuse
	// does not use ID member
	c_text();
	c_text(const c_text& src);
	c_text& operator=(const c_text& src);
	std::string font;
	std::string msg;
};

struct c_phys {
	v2<r32> velocity;
	v2<r32> accel;
};

struct component {
	component(c_type t, void* data);
	component(const component& src);
	c_type type;
	union {
		void* any;

		c_pos* pos;
		c_tex* tex;
		c_text* text;
		c_phys* phys;
	};
};

namespace std {
	template<>
	struct std::less<std::pair<mpos, c_tex*>> { // for priority_queue
		bool operator()(const std::pair<mpos, c_tex*>& l, const std::pair<mpos, c_tex*>& r) {
			// This is flipped so lowest layer is highest priority
			if (l.second->layer < r.second->layer) return false;
			if (l.second->layer > r.second->layer) return true;
			return (l.first.chunk.y * CHUNK_SIZE_METERS + l.first.real.y + l.second->posRect.y + l.second->posRect.h) > 
				   (r.first.chunk.y * CHUNK_SIZE_METERS + r.first.real.y + r.second->posRect.y + r.second->posRect.h);
		}
	};
}

typedef std::multimap<c_type, component> entitydata;

class entityMgr {
public:
	entityMgr();
	~entityMgr();

	entity create();
	component getC(entity e, c_type type);
	component reqC(entity e, c_type type);
	component addC(entity e, c_type type);
	// returns vector of components if there are multiple of a type. the others only return the first match
	std::vector<component> getMultiC(entity e, c_type type);

private:
	u32 lastUID;

	// TODO: unordered or no or yes?
	std::unordered_map<entity, entitydata> edata;

	// SOA entity components - will be accessed by different systems all at once so this should be cache efficient...I think
		// TODO: there's no good way to deleting stuff from here. Free list?
	static_growing_array<c_pos> pos;
	static_growing_array<c_tex> tex;
	static_growing_array<c_text> text;
	static_growing_array<c_phys> phys;
};
