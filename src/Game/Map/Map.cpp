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
			v2<r32> dPTotal = phys->velocity * (dT / 1000.0f);

			// TODO: acceleration

			// Collision loop
			if (phys->volumes.empty()) {
				pos->pos.real += rpos(dPTotal.x, dPTotal.y, 0);
			} else {
				while (dT > 0) {
					// move increment
					v2<r32> dPincr = phys->velocity * (dT / 1000.0f);

					// velocity is zero, we're done
					if (!dPincr) break;

					// Origin-based move segment
					seg sPincr(0, 0, dPincr.x, dPincr.y);

					std::vector<shape*> nearby = getNearbyVolumes(en, pos->pos, dPincr);
					// nothing to collide with
					if (nearby.empty()) {
						phys->velocity += phys->accel * (dT / 1000.0f);
						pos->pos.real += rpos(dPTotal.x, dPTotal.y, 0);
						break;
					}

					// wew lad
					v2<r32> closestVec = dPincr;
					seg collidedWith;
					for (shape* mine : phys->volumes) {
						// TODO: more than r2
						r2<r32>* miner = (r2<r32>*)mine;
						for (shape* other : nearby) {
							r2<r32>* otherr = (r2<r32>*)other;

							r2<r32> sweptr = otherr->sweep_move(*miner);

							std::vector<seg> segs = sweptr.getSegs();
							for (seg s : segs) {
								if (s.includes(v2<r32>(0, 0)) && sweptr.includes(dPTotal)) {
									closestVec = v2<r32>(0, 0);
									collidedWith = s;
									break;
								}
								// point where segments intersect, or zero if they don't
								v2<r32> intersect = s.intersect(sPincr);
								// if this is closer than the last collision
								if (intersect.length() < closestVec.length()) {
									closestVec = intersect;
									collidedWith = s;
								}
							}
						}
					}

					// no collision, move full distance
					if (closestVec == dPTotal) {
						pos->pos.real += rpos(dPTotal.x, dPTotal.y, 0);
						dT = 0;
						// collision
					} else {
						u64 tUsed = std::round((r32)dT * std::abs(closestVec.x / dPincr.x));
						dT -= tUsed;
						pos->pos.real += rpos(closestVec.x, closestVec.y, 0);
						phys->velocity = phys->velocity.parallel(collidedWith.vec());
					}

					for (shape* s : nearby) {
						delete s;
					}
				}
			}

			// Update position in chunk map
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

std::vector<shape*> Map::getNearbyVolumes(entity exclude, mpos pos, v2<r32> move) {
	g->debug.beginFunc(0);

	std::vector<shape*> result;
	v2<s32> chunkArea;
	if (move.x >= 0) chunkArea.x = std::floor(move.x / CHUNK_SIZE_METERS);
	else chunkArea.x = std::ceil(move.x / CHUNK_SIZE_METERS);
	if (move.y >= 0) chunkArea.y = std::floor(move.y / CHUNK_SIZE_METERS);
	else chunkArea.y = std::ceil(move.y / CHUNK_SIZE_METERS);

	for (s32 x = pos.chunk.x - chunkArea.x; x <= pos.chunk.x + chunkArea.x; x++) {
		for (s32 y = pos.chunk.y - chunkArea.y; y <= pos.chunk.y + chunkArea.y; y++) {
			chunk* current = getChunk(cpos(x, y, pos.chunk.z));
			if (!current) continue;

			for (entity en : *current) {
				if (en != exclude) {
					component cph = g->emgr.getC(en, ct_phys);
					component cp = g->emgr.getC(en, ct_pos);
					if (!cph.any || !cp.any) continue;

					c_pos* cpos = cp.pos;
					c_phys* cphys = cph.phys;

					for (shape* s : cphys->volumes) {
						// TODO: more than r2
						r2<r32>* r = new r2<r32>();
						*r = *((r2<r32>*)s) - getDistance(pos, cpos->pos);
						result.push_back(r);
					}
				}
			}
		}
	}

	g->debug.endFunc();
	return result;
}

v2<r32> Map::getDistance(mpos one, mpos two) {
	v2<r32> realD(one.real.x - two.real.x, one.real.y - one.real.y);
	v2<r32> chunkD(one.chunk.x - two.chunk.x, one.chunk.y - one.chunk.y);
	return (chunkD * CHUNK_SIZE_METERS) + realD;
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