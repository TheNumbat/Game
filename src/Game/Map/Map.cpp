#pragma once

#include "..\game.h"
#include "Map.h"

template<typename T>
bool pos<T>::operator==(const pos& other) const {
	return x == other.x &&
		   y == other.y &&
		   z == other.z;
}

Map::Map(engine* _e, game* _g) {
	e = _e;
	g = _g;
}

Map::~Map() {
	
}

chunk* Map::addChunk(cpos p) {
	logSetContext("MAP");

	auto entry = chunks.find(p);
	if (entry == chunks.end()) {
		chunk* newC = new chunk();
		chunks.insert({ p, newC });
		return newC;
	}

	return entry->second;
}

chunk* Map::getChunk(cpos p) {
	logSetContext("MAP");

	auto entry = chunks.find(p);
	if (entry == chunks.end()) {
		logWarn("Failed to get chunk at position " + std::to_string(p.x) + " " + std::to_string(p.y) + " " + std::to_string(p.z));
		return NULL;
	}

	return entry->second;
}
