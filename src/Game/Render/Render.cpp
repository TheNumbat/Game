#pragma once

#include "..\game.h"
#include "Render.h"

// TODO: ew
#define GET_C(type,cmp) ((type*)g->emgr.get(cmp))

s32 renderThread(void* _g) {
	game* g = (game*)_g;
	
	while (g->ren.running) {
		g->e->thread.lockMutex(g->ren.qlock);
		g->e->thread.condWait(g->ren.condRun, g->ren.qlock);
		while (g->ren.texq.size()) {
			std::pair<mpos, c_tex*> t = g->ren.texq.top();
			g->ren.texq.pop();
			g->ren.renderCTex(t);
		}
		g->e->thread.unlockMutex(g->ren.qlock);
	}
	return 0;
}

Render::camera::camera() {
	zoom = 1;
	following = e_null;
}

void Render::camera::update(game* g) {
	logSetContext("CAMERA");

	if (following == e_null) return;

	component cpos = g->emgr.getC(following, ct_pos);

	if (cpos.type == ct_none) {
		logWarn("Followed entity " + std::to_string(following) + " has no position component!");
		return;
	}

	pos = GET_C(c_pos,cpos)->pos;
}

Render::Render(engine* _e, game* _g) {
	e = _e;
	g = _g;
	qlock = e->thread.makeMutex();
	condRun = e->thread.makeCondVar();
	startThread();
}

void Render::init() {
	e->gfx.loadTexture("debug_chunkbounds", "debug/chunkbounds.bmp");
	e->gfx.loadTexture("debug_camera", "debug/camera.png");

	c_tex db;

	db.ID = "debug_chunkbounds";
	db.layer = INT16_MIN;
	db.posRect = r2<r32>(0, 0, CHUNK_SIZE_METERS, CHUNK_SIZE_METERS);
	debugTextures.push_back(db);

	db.ID = "debug_camera";
	db.layer = INT16_MAX;
	db.posRect = r2<r32>(-0.5, -0.5, 1, 1);
	debugTextures.push_back(db);
}

Render::~Render() {
	stopThread();
	e->thread.freeMutex(qlock);
	e->thread.freeCondVar(condRun);
}

void Render::startThread() {
	running = true;
	e->thread.add("render", &renderThread, g);
}

void Render::stopThread() {
	e->thread.lockMutex(qlock);
	running = false;
	e->thread.condSignal(condRun);
	e->thread.unlockMutex(qlock);
	int buf;
	e->thread.wait("render", buf);
}

void Render::zIn(r32 factor) {
	cam.zoom /= factor;
}

void Render::zOut(r32 factor) {
	cam.zoom *= factor;
}

void Render::renderMap() {
	g->debug.beginFunc();

	cam.update(g);
	
	// queue map textures

	s32 wW, wH;
	e->gfx.getWinDim(wW, wH);
	r32 halfWMeters = (wW / 2.0f) * (PIXELS_TO_METERS / cam.zoom);
	r32 halfHMeters = (wH / 2.0f) * (PIXELS_TO_METERS / cam.zoom);

	// Get chunk positions of Top Left Corner and Bottom Right Corner of window
	mpos winOffset(rpos(halfWMeters, halfHMeters, 0), cpos());
	mpos TLC = cam.pos - winOffset;
	mpos BRC = cam.pos + winOffset;

	// loop through chunks on screen
	e->thread.lockMutex(qlock);
	for (s16 relX = TLC.offset.x; relX <= BRC.offset.x; relX++) {
		for (s16 relY = TLC.offset.y; relY <= BRC.offset.y; relY++) {

			cpos currentCPos = cam.pos.chunk + cpos(relX, relY, 0);
			chunk* currentC = g->map.getChunk(currentCPos);

			if (!currentC) continue;

			// Debug: draw chunk boundary
			if (g->debug.getFlag(renderChunkbounds)) {
				texq.push({ mpos(rpos(), currentCPos), &debugTextures[dt_chunkbounds] });
			}

			for (entity e : *currentC) {
				component epos = g->emgr.getC(e, ct_pos);
				if (epos.type == ct_none) continue;

				// TODO: Debug: draw position text
				// TODO: Debug: draw collision bounds

				std::vector<component> etexs = g->emgr.getMultiC(e, ct_tex);

				if (!etexs.size()) continue;

				for (component& c : etexs) {
					texq.push({ GET_C(c_pos,epos)->pos, GET_C(c_tex,c) });
				}
			}
		}
	}

	// Debug: draw camera
	if (g->debug.getFlag(renderCamera)) {
		texq.push({ cam.pos, &debugTextures[dt_camera] });
	}

	e->thread.condSignal(condRun);
	e->thread.unlockMutex(qlock);
	g->debug.endFunc();
}

void Render::renderDebugHUD() {
	
}

void Render::endBatch() {
	e->thread.lockMutex(qlock);
	e->thread.unlockMutex(qlock);
}

v2<r32> Render::mapIntoPxSpace(mpos point, mpos origin) {
	v2<r32> realDist(point.real.x - origin.real.x, point.real.y - origin.real.y);
	v2<r32> chunkDist(point.chunk.x - origin.chunk.x, point.chunk.y - origin.chunk.y);

	v2<r32> meters = chunkDist * CHUNK_SIZE_METERS + realDist;
	v2<r32> pixels = meters * METERS_TO_PIXELS * cam.zoom;

	return pixels;
}

v2<r32> Render::mapPxTLC(mpos point) {
	s32 wW, wH;
	e->gfx.getWinDim(wW, wH);
	r32 halfWMeters = (wW / 2.0f) * (PIXELS_TO_METERS / cam.zoom);
	r32 halfHMeters = (wH / 2.0f) * (PIXELS_TO_METERS / cam.zoom);

	// Get chunk positions of Top Left Corner and Bottom Right Corner of window
	mpos winOffset(rpos(halfWMeters, halfHMeters, 0), cpos());
	mpos TLC = cam.pos - winOffset;
	
	return mapIntoPxSpace(point, TLC);
}

void Render::renderCTex(std::pair<mpos, c_tex*> t) {
	e->gfx.setBlendmode(t.second->ID, t.second->blend);
	e->gfx.setColorMod(t.second->ID, t.second->mod);

	v2<r32> pxOffset = mapPxTLC(t.first);
	r2<r32> pxRect = t.second->posRect * METERS_TO_PIXELS * cam.zoom + pxOffset;
	v2<r32> pxRotPt = t.second->rotPt * METERS_TO_PIXELS * cam.zoom;
	
	e->gfx.renderTextureEx(t.second->ID, pxRect.round(), t.second->srcPxlRect, pxRotPt.round(), t.second->rot, t.second->flip);
}