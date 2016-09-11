#pragma once

#include "..\game.h"
#include "Map.h"
#include "..\Entity\Entity.h"

// TODO: ew
#define GET_C(type,cmp) ((type*)g->emgr.get(cmp))

mpos::mpos() {};

mpos::mpos(rpos r, cpos c, cpos o) {
	real = r;
	chunk = c;
	offset = o;
}

mpos& mpos::move(rpos r) {
	real += r;
	clamp();
	return *this;
}

mpos& mpos::move(cpos c) {
	chunk += c;
	offset += c;
	return *this;
}

mpos& mpos::clamp() {
	while (real.x < 0) {
		real.x += CHUNK_SIZE_METERS;
		chunk.x--;
		offset.x--;
	}
	while (real.y < 0) {
		real.y += CHUNK_SIZE_METERS;
		chunk.y--;
		offset.y--;
	}
	while (real.x > CHUNK_SIZE_METERS) {
		real.x -= CHUNK_SIZE_METERS;
		chunk.x++;
		offset.x++;
	}
	while (real.y > CHUNK_SIZE_METERS) {
		real.y -= CHUNK_SIZE_METERS;
		chunk.y++;
		offset.y++;
	}
	return *this;
}

mpos mpos::operator+(const mpos& other) {
	mpos ret(real + other.real, chunk + other.chunk);
	ret.clamp();
	return ret;
}

mpos mpos::operator-(const mpos& other) {
	mpos ret(real - other.real, chunk - other.chunk);
	ret.clamp();
	return ret;
}

mpos& mpos::operator+=(const mpos& other) {
	real += other.real;
	chunk += other.chunk;
	offset = cpos();
	clamp();
	return *this;
}

mpos& mpos::operator-=(const mpos& other) {
	real -= other.real;
	chunk -= other.chunk;
	offset = cpos();
	clamp();
	return *this;
}

Map::Map(engine* _e, game* _g) {
	e = _e;
	g = _g;
}

Map::~Map() {
	
}

chunk* Map::reqChunk(cpos p) {
	logSetContext("MAP");

	auto entry = chunks.find(p);
	if (entry == chunks.end()) {
		auto iter = chunks.insert({ p, chunk() }).first;
		return &iter->second;
	}

	return &entry->second;
}

chunk* Map::getChunk(cpos p) {
	logSetContext("MAP");

	auto entry = chunks.find(p);
	if (entry == chunks.end()) {
		// logWarn("Failed to get chunk at position " + std::to_string(p.x) + " " + std::to_string(p.y) + " " + std::to_string(p.z));
		return NULL;
	}

	return &entry->second;
}

bool Map::addEntity(entity e) {
	logSetContext("MAP");

	component pos = g->emgr.getC(e, ct_pos);
	if (pos.type == ct_none) {
		logWarn("Entity " + std::to_string(e) + " does not have position component!");
		return false;
	}

	chunk* c = reqChunk(GET_C(c_pos,pos)->pos.chunk);
	c->insert(e);
	return true;
}

bool Map::updateEntity(entity e) {
	logSetContext("MAP");

	component pos = g->emgr.getC(e, ct_pos);
	if (pos.type == ct_none) {
		logWarn("Entity " + std::to_string(e) + " does not have position component!");
		return false;
	}

	chunk* old = getChunk(GET_C(c_pos, pos)->pos.chunk - GET_C(c_pos, pos)->pos.offset);
	if (!old) {
		int x = GET_C(c_pos, pos)->pos.chunk.x, y = GET_C(c_pos, pos)->pos.chunk.y, z = GET_C(c_pos, pos)->pos.chunk.z;
		logWarn("Could not find chunk at pos " + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(x));
		return false;
	}
	chunk* _new = reqChunk(GET_C(c_pos, pos)->pos.chunk);

	old->erase(e);
	_new->insert(e);

	GET_C(c_pos, pos)->pos.offset = cpos(0, 0, 0);
	return true;
}