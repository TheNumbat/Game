#pragma once

#include "..\game.h"
#include "Render.h"
#include <sstream>

s32 renderThread(void* _g) {
	game* g = (game*)_g;
	
	while (g->ren.running) {
		g->e->thread.lockMutex(g->ren.qlock);
		g->e->thread.condWait(g->ren.condRun, g->ren.qlock);
		while (g->ren.texq.size()) {
			auto t = g->ren.texq.top();
			g->ren.texq.pop();
			g->ren.renderCTex(t);
			if (std::get<0>(t)) {
				delete std::get<2>(t);
			}
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

	pos = cpos.pos->pos;
}

Render::Render(engine* _e, game* _g) {
	e = _e;
	g = _g;
	qlock = e->thread.makeMutex();
	condRun = e->thread.makeCondVar();
	startThread();
}

void Render::init() {
	e->gfx.loadTexture("debug_chunkbounds", "debug_assets/chunkbounds.bmp");
	e->gfx.loadTexture("debug_camera", "debug_assets/camera.png");

	c_tex* db = new c_tex();

	db->ID = "debug_chunkbounds";
	db->layer = INT16_MIN;
	db->posRect = r2<r32>(0, 0, CHUNK_SIZE_METERS, CHUNK_SIZE_METERS);
	debugTextures.push_back(db);

	db = new c_tex();
	db->ID = "debug_camera";
	db->layer = INT16_MAX;
	db->posRect = r2<r32>(-0.5, -0.5, 1, 1);
	debugTextures.push_back(db);

	c_text* dt = new c_text();
	dt->font = "debug_small";
	dt->layer = INT16_MAX;
	dt->msg = "Press ~ to show profiler";
	r32 size = g->e->gfx.getFontSize("debug_small") * PIXELS_TO_METERS;
	dt->posRect = r2<r32>(0, 0, size * dt->msg.length(), size);
	debugTextures.push_back(dt);
}

Render::~Render() {}

void Render::kill() {
	stopThread();
	e->thread.freeMutex(qlock);
	e->thread.freeCondVar(condRun);
	for (c_tex* t : debugTextures) {
		delete t;
	}
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

void Render::zOut(r32 factor) {
	cam.zoom /= factor;
}

void Render::zIn(r32 factor) {
	cam.zoom *= factor;
}

void Render::renderMap() {
	g->debug.beginFunc();

	cam.update(g);
	
	// queue map textures

	mpos TLC = getTLC();
	mpos BRC = getBRC();

	// loop through chunks on screen
	e->thread.lockMutex(qlock);
	for (s16 relX = TLC.offset.x; relX <= BRC.offset.x; relX++) {
		for (s16 relY = TLC.offset.y; relY <= BRC.offset.y; relY++) {

			cpos currentCPos = cam.pos.chunk + cpos(relX, relY, 0);
			chunk* currentC = g->map.getChunk(currentCPos);

			if (!currentC) continue;

			// Debug: draw chunk boundary
			if (g->debug.getFlag(renderChunkbounds)) {
				texq.push({ false, mpos(rpos(), currentCPos), debugTextures[dt_chunkbounds] });
			}

			for (entity e : *currentC) {
				component epos = g->emgr.getC(e, ct_pos);
				if (epos.type == ct_none) continue;

				mpos pos = epos.pos->pos;

				// Debug: draw position text
				if (g->debug.getFlag(renderPositionText)) {
					std::stringstream posStr;
					posStr << pos.chunk.x << " " << pos.chunk.y << " " << pos.chunk.z
					   	   << " " << pos.real.x << " " << pos.real.y << " " << pos.real.z;
					c_text* text = new c_text();
					text->font = "debug_small";
					text->msg = posStr.str();
					text->layer = INT16_MAX;
					r32 size = g->e->gfx.getFontSize("debug_small") * PIXELS_TO_METERS;
					text->posRect = r2<r32>(0, 0, text->msg.length() * size, size);
					texq.push({ true, pos, text });
				}
				
				// TODO: Debug: draw collision bounds

				std::vector<component> etexs = g->emgr.getMultiC(e, ct_tex);

				if (!etexs.size()) continue;

				for (component& c : etexs) {
					texq.push({ false, pos, c.tex });
				}
			}
		}
	}

	// Debug: draw camera
	if (g->debug.getFlag(renderCamera)) {
		texq.push({ false, cam.pos, debugTextures[dt_camera] });
	}

	e->thread.condSignal(condRun);
	e->thread.unlockMutex(qlock);
	g->debug.endFunc();
}

void Render::renderDebugHUD() {
	g->debug.beginFunc();

	e->thread.lockMutex(qlock);
	if (!g->debug.getFlag(renderDebugUI)) {
		texq.push({ false, getTLC(), debugTextures[dt_profiler] });
	}
	e->thread.condSignal(condRun);
	e->thread.unlockMutex(qlock);

	g->debug.endFunc();
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

mpos Render::getTLC() {
	s32 wW, wH;
	e->gfx.getWinDim(wW, wH);
	r32 halfWMeters = (wW / 2.0f) * (PIXELS_TO_METERS / cam.zoom);
	r32 halfHMeters = (wH / 2.0f) * (PIXELS_TO_METERS / cam.zoom);

	// Get chunk positions of Top Left Corner and Bottom Right Corner of window
	mpos winOffset(rpos(halfWMeters, halfHMeters, 0), cpos());
	mpos TLC = cam.pos - winOffset;

	return TLC;
}


mpos Render::getBRC() {
	s32 wW, wH;
	e->gfx.getWinDim(wW, wH);
	r32 halfWMeters = (wW / 2.0f) * (PIXELS_TO_METERS / cam.zoom);
	r32 halfHMeters = (wH / 2.0f) * (PIXELS_TO_METERS / cam.zoom);

	// Get chunk positions of Top Left Corner and Bottom Right Corner of window
	mpos winOffset(rpos(halfWMeters, halfHMeters, 0), cpos());
	mpos BRC = cam.pos + winOffset;

	return BRC;
}

void Render::renderCTex(std::tuple<bool, mpos, c_tex*> val) {
	c_tex* t = std::get<2>(val);

	v2<r32> pxOffset = mapIntoPxSpace(std::get<1>(val), getTLC());
	r2<r32> pxRect = t->posRect * METERS_TO_PIXELS * cam.zoom + pxOffset;
	v2<r32> pxRotPt = t->rotPt * METERS_TO_PIXELS * cam.zoom;

	if (c_text* text = dynamic_cast<c_text*>(t)) {
		e->gfx.renderTextEx(text->font, text->msg, pxRect.round(), text->srcPxlRect, text->blend, text->mod, pxRotPt.round(), text->rot, text->flip);
	} else {
		e->gfx.setBlendmode(t->ID, t->blend);
		e->gfx.setColorMod(t->ID, t->mod);

		e->gfx.renderTextureEx(t->ID, pxRect.round(), t->srcPxlRect, pxRotPt.round(), t->rot, t->flip);
	}
}