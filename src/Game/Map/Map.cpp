#pragma once

#include "..\game.h"
#include "Map.h"
#include "..\Entity\Entity.h"

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
	e->time.addTimer("physics");
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

bool Map::registerEntity(entity e) {
	logSetContext("MAP");

	component pos = g->emgr.getC(e, ct_pos);
	if (pos.type == ct_none) {
		logWarn("Entity " + std::to_string(e) + " does not have position component!");
		return false;
	}

	chunk* c = reqChunk(pos.pos->pos.chunk);
	c->insert(e);
	return true;
}

bool Map::updateEntity(entity e, bool remove) {
	logSetContext("MAP");

	component pos = g->emgr.getC(e, ct_pos);
	if (pos.type == ct_none) {
		logWarn("Entity " + std::to_string(e) + " does not have position component!");
		return false;
	}

	chunk* old = getChunk(pos.pos->pos.chunk - pos.pos->pos.offset);
	if (!old) {
		int x = pos.pos->pos.chunk.x, y = pos.pos->pos.chunk.y, z = pos.pos->pos.chunk.z;
		logWarn("Could not find chunk at pos " + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(x));
		return false;
	}
	chunk* _new = reqChunk(pos.pos->pos.chunk);

	if (_new == old)
		return false;

	_new->insert(e);
	if (remove)
		old->erase(e);

	pos.pos->pos.offset = cpos(0, 0, 0);
	return true;
}

void Map::runPhysics() {
	g->debug.beginFunc(0);

	// TODO: simulation regions
	for (auto& entry : chunks) {
		chunk& c = entry.second;

		bool moved = false;
		for (auto eEntry = c.begin(); eEntry != c.end(); moved ? eEntry : ++eEntry) {
			moved = false;

			entity en = *eEntry;
			component cp(ct_none, NULL);

			cp = g->emgr.getC(en, ct_pos);
			c_pos* pos = cp.pos;
			cp = g->emgr.getC(en, ct_phys);
			c_phys* phys = cp.phys;

			if (!pos || !phys) continue;

			u64 current = e->time.get("physics");
			u64 dT = current - phys->lastUpdate;

			phys->velocity += phys->accel * (dT / 1000.0f);
			v2<r32> dP = phys->velocity * (dT / 1000.0f);
			pos->pos.real += rpos(dP.x, dP.y, 0);
			pos->pos.clamp();

			if (updateEntity(en, false)) {
				moved = true;
				c.erase(eEntry++);
			}

			phys->lastUpdate = current;
		}
	}

	g->debug.endFunc();
}

bool Map::registerPlayer(u8 id, entity en) {
	logSetContext("MAP");
	
	auto entry = players.find(id);
	if (entry != players.end()) {
		logWarn("Player ID " + std::to_string(id) + " already in use!");
		return false;
	}

	players.insert({ id, en });
	return true;
}

entity Map::getPlayer(u8 id) {
	logSetContext("MAP");

	auto entry = players.find(id);
	if (entry == players.end()) {
		logWarn("Player ID " + std::to_string(id) + " not found!");
		return false;
	}

	return entry->second;
}