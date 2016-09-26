#pragma once

#include "common.h"
#include <engine.h>
#include <set>
#include <unordered_map>

struct game;
typedef u32 entity;

const r32 CHUNK_SIZE_METERS = 25.0f;
typedef std::set<entity> chunk;

// SO FAR: Z IN REAL POSITION DOES NOT AFFECT CHUNK POSITION
	// TODO: should it?

template<typename T>
struct basepos {
	basepos() { x = y = z = 0; }
	basepos(T _x, T _y, T _z) { x = _x; y = _y; z = _z; }
	bool operator==(const basepos& other) const 
		{ return x == other.x && y == other.y && z == other.z; }
	basepos operator+(const basepos& other) const
		{ return basepos(x + other.x, y + other.y, z + other.z); }
	basepos operator-(const basepos& other) const
		{ return basepos(x - other.x, y - other.y, z - other.z); }
	basepos& operator+=(const basepos& other)
		{ x += other.x; y += other.y; z += other.z; return *this; }
	basepos& operator-=(const basepos& other)
		{ x -= other.x; y -= other.y; z -= other.z; return *this; }
	T x, y, z;
};
typedef basepos<r32> rpos; // real position (used for within chunk)
typedef basepos<s32> cpos; // chunk position

struct mpos { // combined position (for entity)
	mpos();
	mpos(rpos r, cpos c, cpos o = cpos(0, 0, 0));
	mpos& move(rpos r); // can update offset
	mpos& move(cpos c); // will update offset
	mpos& clamp(); // clamps real to CHUNK_SIZE_METERS, updates chunk, offset

	// offset will become only result of addition/subtraction
	mpos operator+(const mpos& other);
	mpos operator-(const mpos& other);
	mpos& operator+=(const mpos& other);
	mpos& operator-=(const mpos& other);

	rpos real;
	cpos chunk;
	cpos offset; // stores offset needed to update position in chunk map
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
	
	// TODO: chunk paging in/out

	chunk* reqChunk(cpos p); // gets or adds chunk (reqiureChunk)
	chunk* getChunk(cpos p);

	bool registerEntity(entity e); // e needs a c_pos
	bool updateEntity(entity e, bool remove = true);

	bool registerPlayer(u8 id, entity en);
	entity getPlayer(u8 id);

	void runPhysics();
	
private:
	// allocates new shapes, need to delete
	std::vector<shape*> getNearbyVolumes(entity exclude, mpos pos, v2<r32> move);
	v2<r32> getDistance(mpos one, mpos two);

	std::unordered_map<cpos, chunk> chunks;
	std::map<u8, entity> players;

	engine* e;
	game* g;
};

