#pragma once

#include "common.h"
#include <engine.h>
#include <map> // heh
#include <unordered_map>

#include "..\Entity\entity.h"

struct game;

const r32 CHUNK_SIZE_METERS = 25.0f;
typedef std::map<u64, entity*> chunk;

template<typename T>
struct pos {
	// TODO: implement stuff
	bool operator==(const pos& other) const;
	T x, y, z;
};
typedef pos<r32> rpos; // real position (used for within chunk)
typedef pos<s32> cpos; // chunk position

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

private:
	std::unordered_map<cpos, chunk*> chunks;

	engine* e;
	game* g;
};

