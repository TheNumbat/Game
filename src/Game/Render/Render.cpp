#pragma once

#include "..\game.h"
#include "Render.h"
#include <sstream>

s32 renderThread(void* _g) {
	logSetContext("RENDERTHREAD");
	logInfo("Started Render Thread");
	game* g = (game*)_g;
	
	g->debug.beginThreadProf("render", 1);
	while (g->ren.running) {
		g->debug.beginFunc(1);
		g->e->thread.lockMutex(g->ren.qlock);
		g->e->thread.condWait(g->ren.condRun, g->ren.qlock);
		if (g->ren.batchQueue.size()) {
			renderBatch& batch = g->ren.batchQueue.front();
			while (batch.size()) {
				auto& t = batch.top();
				g->ren.renderCTex(t);
				if (std::get<0>(t)) {
					delete std::get<2>(t);
				}
				batch.pop();
			}
			g->ren.batchQueue.pop();
		}
		g->e->thread.unlockMutex(g->ren.qlock);
		g->debug.endFunc(1);
	}
	logInfo("Exiting Render Thread");
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

void Render::batchMap() {
	g->debug.beginFunc(0);

	batchQueue.push(renderBatch());
	renderBatch& batch = batchQueue.back();
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
				batch.push({ false, mpos(rpos(), currentCPos), debugTextures[dt_chunkbounds] });
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
					batch.push({ true, pos, text });
				}
				
				// TODO: Debug: draw collision bounds

				std::vector<component> etexs = g->emgr.getMultiC(e, ct_tex);

				if (!etexs.size()) continue;

				for (component& c : etexs) {
					batch.push({ false, pos, c.tex });
				}
			}
		}
	}

	// Debug: draw camera
	if (g->debug.getFlag(renderCamera)) {
		batch.push({ false, cam.pos, debugTextures[dt_camera] });
	}
	e->thread.unlockMutex(qlock);
	g->debug.endFunc();
}

void Render::batchBegin() {
	g->debug.beginFunc(0);
	e->thread.lockMutex(qlock);
	e->thread.condSignal(condRun);
	e->thread.unlockMutex(qlock);
	g->debug.endFunc();
}

void Render::end() {
	g->debug.beginFunc(0);
	e->thread.lockMutex(qlock);
	e->thread.unlockMutex(qlock);
	g->debug.endFunc();
}

void Render::renderDebugHUD() {
	s32 lines = 0;
	s32 fontsize = e->gfx.getFontSize("debug_small");

	e->thread.lockMutex(qlock);
	if (!g->debug.getFlag(renderDebugUI)) {
		e->gfx.renderText("debug_small", "Press ~ for profiler", r2<s32>(10, 10 + lines * fontsize, 0, 0));
	} else {
		e->gfx.renderText("debug_small", "Press tab for console", r2<s32>(10, 10 + lines * fontsize, 0, 0));
		lines++;

		r64 avgFrame = 1000.0f * (r64)g->debug.getAvgFrame() / (r64)e->time.getPerfFreq();
		r64 lastFrame = 1000.0f * (r64)g->debug.getLastFrame() / (r64)e->time.getPerfFreq();
		std::string msg1 = "Average frame time (ms): " + std::to_string(avgFrame);
		std::string msg2 = "Last frame time (ms): " + std::to_string(lastFrame);

		e->gfx.renderText("debug_small", msg1, r2<s32>(10, 10 + lines * fontsize, 0, 0));
		lines++;
		e->gfx.renderText("debug_small", msg2, r2<s32>(10, 10 + lines * fontsize, 0, 0));
		lines++;

		e->gfx.renderText("debug_small", "Debug values:", r2<s32>(10, 10 + lines * fontsize, 0, 0));
		lines++;
		for (auto& entry : g->debug.values) {
			e->gfx.renderText("debug_small", "   " + entry.first + " - " + entry.second->getStr(), r2<s32>(10, 10 + lines * fontsize, 0, 0));
			lines++;
		}

		for (auto head : g->debug.heads) {
			lines += recProfRender(head.second, 10 + lines * fontsize, 0, fontsize);
			lines++;
		}

		if (g->events.state == input_console) {
		 	int sw, sh;
		 	e->gfx.getWinDim(sw, sh);
		 	e->gfx.renderText("debug_small", " >>> " + g->events.inStr, r2<s32>(10, sh - fontsize - 10, 0, 0));
		}
	}
	e->thread.unlockMutex(qlock);
}

u32 Render::recProfRender(Util::profNode* node, u32 pos, u32 lvl, u32 fontsize) {
	if (!node) return 0;

	std::string msg;
	for (u32 i = 0; i < lvl; i++) {
		msg += "   ";
	}
	msg = msg + node->name + " - self: " + std::to_string(node->self) +
		  " heir: " + std::to_string(node->heir) + " calls: " + std::to_string(node->calls);

	if (g->debug.selected == node) {
		e->gfx.renderText("debug_small", msg, r2<s32>(10, pos, 0, 0), blend_alpha, color(50, 100, 255, 0));
	}
	else {
		e->gfx.renderText("debug_small", msg, r2<s32>(10, pos, 0, 0));
	}

	int numchildren = 0;
	if (node->showChildren) {
		for (auto& entry : node->children) {
			numchildren++;
			numchildren += recProfRender(entry.second, pos + numchildren * fontsize , lvl + 1, fontsize);
		}
	}

	return numchildren;
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
	r2<r32> pxRect = t->posRect * METERS_TO_PIXELS * (t->zoom ? cam.zoom : 1) + pxOffset;
	v2<r32> pxRotPt = t->rotPt * METERS_TO_PIXELS * (t->zoom ? cam.zoom : 1);

	if (c_text* text = dynamic_cast<c_text*>(t)) {
		e->gfx.renderTextEx(text->font, text->msg, pxRect.round(), text->srcPxlRect, text->blend, text->mod, pxRotPt.round(), text->rot, text->flip);
	} else {
		e->gfx.setBlendmode(t->ID, t->blend);
		e->gfx.setColorMod(t->ID, t->mod);

		e->gfx.renderTextureEx(t->ID, pxRect.round(), t->srcPxlRect, pxRotPt.round(), t->rot, t->flip);
	}
}