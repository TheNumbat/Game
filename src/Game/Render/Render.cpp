#pragma once

#include "..\game.h"
#include "Render.h"
#include <sstream>

#pragma warning (disable: 4244)

void Render::render() {
	g->debug.beginFunc(0);
	if (batchQueue.size()) {
		renderBatch& batch = batchQueue.front();
		cam.update(g);
		while (batch.size()) {
			const renderable* t = &batch.top();
			renderRenderable(t);
			if (t->deleteAfter) {
				delete t->texture;
			}
			batch.pop();
		}
		batchQueue.pop();
	}
	g->debug.endFunc(0);
}

Render::camera::camera() {
	zoom = 1;
	following = e_null;
}

Render::renderable::renderable(c_tex* t, mpos b, bool del) {
	texture = t;
	base = b;
	deleteAfter = del;
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
	g->ren.updateCorners();
}

Render::Render(engine* _e, game* _g) {
	e = _e;
	g = _g;
}

void Render::setFollow(entity en) {
	cam.following = en;
	cam.update(g);
}

void Render::init() {
	e->gfx.loadTexture("debug_chunkbounds", "debug_assets/chunkbounds.bmp");
	e->gfx.loadTexture("debug_collision", "debug_assets/collision.bmp");
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

	updateCorners();
}

Render::~Render() {}

void Render::kill() {
	for (c_tex* t : debugTextures) {
		delete t;
	}
}

void Render::zOut(r32 factor) {
	cam.zoom /= factor;
	updateCorners();
}

void Render::zIn(r32 factor) {
	cam.zoom *= factor;
	updateCorners();
}

void Render::batchMap() {
	g->debug.beginFunc(0);

	batchQueue.push(renderBatch());
	renderBatch& batch = batchQueue.back();
	
	// queue map textures

	// loop through chunks on screen
	for (s16 relX = TLC.offset.x; relX <= BRC.offset.x; relX++) {
		for (s16 relY = TLC.offset.y; relY <= BRC.offset.y; relY++) {

			cpos currentCPos = cam.pos.chunk + cpos(relX, relY, 0);
			chunk* currentC = g->map.getChunk(currentCPos);

			if (!currentC) continue;

			// Debug: draw chunk boundary
			if (g->debug.getFlag(renderChunkbounds)) {
				batch.push(renderable( debugTextures[dt_chunkbounds], mpos(rpos(), currentCPos), false ));
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
					text->font = "debug_small_black";
					text->msg = posStr.str();
					text->layer = INT16_MAX;
					r32 size = g->e->gfx.getFontSize("debug_small_black") * PIXELS_TO_METERS;
					text->posRect = r2<r32>(0, 0, text->msg.length() * size, size);
					batch.push(renderable( text, pos, true ));
				}
				
				// Debug: draw collision bounds
				if (g->debug.getFlag(renderCollisionBounds)) {
					component cphys = g->emgr.getC(e, ct_phys);
					if (c_phys* phys = cphys.phys) {
						for (shape* s : phys->volumes) {
							// TODO: more than r2 - polygon rendering
							r2<r32>* r = (r2<r32>*)s;
							c_tex* t = new c_tex();
							t->ID = "debug_collision";
							t->layer = INT16_MAX;
							t->posRect = *r;
							//t->posRect = r->sweep_move(*((r2<r32>*)g->emgr.getC(g->map.getPlayer(0),ct_phys).phys->volumes[0]));
							t->blend = blend_additive;
							batch.push(renderable(t, pos, true));
						}
					}
				}

				std::vector<component> etexs = g->emgr.getMultiC(e, ct_tex);
				std::vector<component> etexts = g->emgr.getMultiC(e, ct_text);

				if (!etexs.size() && !etexts.size()) continue;

				for (component& c : etexs) {
					batch.push(renderable(c.tex, pos, false));
				}
				for (component& c : etexts) {
					batch.push(renderable(c.text, pos, false));
				}
			}
		}
	}

	// Debug: draw camera
	if (g->debug.getFlag(renderCamera)) {
		batch.push(renderable( debugTextures[dt_camera], cam.pos, false ));
	}
	g->debug.endFunc();
}

// Only for batchDebugHUD (wew lad)
#define PUSH_TEXT(tmsg,rect) \
	text = new c_text(); \
	text->respectMeters = false; \
	text->font = "debug_small"; \
	text->layer = INT16_MAX; \
	text->msg = tmsg; \
	text->posRect = rect; \
	batch.push(renderable(text, mpos(), true)); \
	lines++;

void Render::batchDebugHUD() {
	std::string msg;
	s32 lines = 0;
	s32 fontsize = e->gfx.getFontSize("debug_small");

	c_text* text;

	batchQueue.push(renderBatch());
	renderBatch& batch = batchQueue.back();

	if (!g->debug.getFlag(renderDebugUI)) {
		r64 fps = (r64)e->time.getPerfFreq() / (r64)g->debug.getLastFrame();
		std::string msg = "fps: " + std::to_string((s32)std::round(fps));
		PUSH_TEXT(msg, r2<r32>(10, 10, 0, 0));
	} else {
		r64 avgFrame = 1000.0f * (r64)g->debug.getAvgFrame() / (r64)e->time.getPerfFreq();
		r64 lastFrame = 1000.0f * (r64)g->debug.getLastFrame() / (r64)e->time.getPerfFreq();

		msg = "Average frame time (ms): " + std::to_string(avgFrame);
		PUSH_TEXT(msg, r2<r32>(10, 10 + lines * fontsize, 0, 0));

		msg = "Last frame time (ms): " + std::to_string(lastFrame);
		PUSH_TEXT(msg, r2<r32>(10, 10 + lines * fontsize, 0, 0));

		msg = "Debug Values:";
		PUSH_TEXT(msg, r2<r32>(10, 10 + lines * fontsize, 0, 0));

		for (auto& entry : g->debug.values) {
			msg = "   " + entry.first + " - " + entry.second->getStr();
			PUSH_TEXT(msg, r2<r32>(10, 10 + lines * fontsize, 0, 0));
		}

		for (auto head : g->debug.heads) {
			lines += recProfRender(head.second, 10 + lines * fontsize, 0, fontsize);
			lines++;
		}

		if (g->events.state == input_console) {
		 	int sw, sh;
		 	e->gfx.getWinDim(sw, sh);
			msg = " >>> " + g->events.inStr;
		 	PUSH_TEXT(msg, r2<r32>(10, sh - fontsize - 10, 0, 0));

			for (u32 i = 0; i < g->events.chatLog.size() && i < 10; i++) {
				PUSH_TEXT(g->events.chatLog[i], r2<r32>(10, sh - 10 - (i + 2) * fontsize, 0, 0));
			}
		}
	}
}
#undef PUSH_TEXT

u32 Render::recProfRender(Util::profNode* node, u32 pos, u32 lvl, u32 fontsize) {
	if (!node) return 0;

	std::string msg;
	for (u32 i = 0; i < lvl; i++) {
		msg += "   ";
	}
	msg = msg + node->name + " - self: " + std::to_string(node->self) +
		  " heir: " + std::to_string(node->heir) + " calls: " + std::to_string(node->calls);

	c_text* text = new c_text();
	text->respectMeters = false;
	text->font = "debug_small";
	text->layer = INT16_MAX;
	text->msg = msg;
	text->posRect = r2<r32>(10, pos, 0, 0);

	if (g->debug.selected.second == node) {
		text->font = "debug_small_blue";
	}

	renderBatch& batch = batchQueue.back();
	batch.push(renderable(text, mpos(), true));

	int numchildren = 0;
	if (node->showChildren) {
		for (auto& entry : node->children) {
			numchildren++;
			numchildren += recProfRender(entry.second, pos + numchildren * fontsize , lvl + 1, fontsize);
		}
	}

	return numchildren;
}

v2<s32> Render::mapIntoPxSpace(mpos point, mpos origin) {
	v2<r32> realDist(point.real.x - origin.real.x, point.real.y - origin.real.y);
	v2<r32> chunkDist(point.chunk.x - origin.chunk.x, point.chunk.y - origin.chunk.y);

	v2<r32> meters = chunkDist * CHUNK_SIZE_METERS + realDist;
	v2<r32> pixels = meters * METERS_TO_PIXELS * cam.zoom;

	return pixels.round();
}

void Render::updateCorners() {
	s32 wW, wH;
	e->gfx.getWinDim(wW, wH);
	r32 halfWMeters = (wW / 2.0f) * (PIXELS_TO_METERS / cam.zoom);
	r32 halfHMeters = (wH / 2.0f) * (PIXELS_TO_METERS / cam.zoom);

	// Get chunk positions of Top Left Corner and Bottom Right Corner of window
	mpos winOffset(rpos(halfWMeters, halfHMeters, 0), cpos());
	TLC = cam.pos - winOffset;
	BRC = cam.pos + winOffset;
}

void Render::renderRenderable(const renderable* r) {
	g->debug.beginFunc(0);
	c_tex* t = r->texture;

	v2<s32> pxOffset = mapIntoPxSpace(r->base, TLC);
	r2<r32> pxRect;
	v2<r32> pxRotPt;
	if (t->respectMeters) {
		pxRect = t->posRect * METERS_TO_PIXELS * (t->respectZoom ? cam.zoom : 1) + v2<r32>(pxOffset.x,pxOffset.y);
		pxRotPt = t->rotPt * METERS_TO_PIXELS * (t->respectZoom ? cam.zoom : 1);
	} else {
		pxRect = t->posRect;
		pxRotPt = t->rotPt;
	}

	if (t->isText) {
		c_text* text = (c_text*) t;
		e->gfx.renderText(text->font, text->msg, pxRect);
	} else {
		e->gfx.setBlendmode(t->ID, t->blend);
		e->gfx.setColorMod(t->ID, t->mod);

		e->gfx.renderTextureEx(t->ID, pxRect, t->srcPxlRect, pxRotPt, t->rot);
	}
	g->debug.endFunc(0);
}