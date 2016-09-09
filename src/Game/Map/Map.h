#pragma once

#include "common.h"
#include <engine.h>
#include <set>
#include <unordered_map>

struct game;
typedef u32 entity;

const r32 CHUNK_SIZE_METERS = 25.0f;
typedef std::set<entity> chunk;

template<typename T>
struct basepos {
	// TODO: implement stuff
	basepos() { x = y = z = 0; }
	basepos(T _x, T _y, T _z) { x = _x; y = _y; z = _z; }
	bool operator==(const basepos& other) const { return x == other.x && y == other.y && z == other.z; }
	T x, y, z;
};
typedef basepos<r32> rpos; // real position (used for within chunk)
typedef basepos<s32> cpos; // chunk position

struct mpos { // combined position (for entity)
	// TODO: implement stuff
	rpos real;
	cpos chunk;
	cpos movOffset; // stores offset needed to move in chunk map
};

namespace std { // hash for chunk hash map
	template<>
	struct hash<cpos> {
		u64 operator()(const cpos& hash) const {
			u64 hx = std::hash<s32>()(hash.x);
			u64 hy = std::hash<s32>()(hash.y);
			u64 hz = std::hash<s32>()(hash.z);
			// TODO: idk about this hash 
			return hx ^ ((hy ^ (hz << 1)) << 1);
		}
	};
}

class Map {
public:
	Map(engine* _e, game* _g);
	~Map();
	
	chunk* addChunk(cpos p);
	chunk* getChunk(cpos p);

	bool addEntity(entity e); // e needs a c_pos
	bool updateEntity(entity e);

private:
	std::unordered_map<cpos, chunk> chunks;

	engine* e;
	game* g;
};

