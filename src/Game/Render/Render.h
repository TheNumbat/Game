#pragma once

#include "common.h"
#include <engine.h>
#include <queue>
#include <tuple>
#include "..\Entity\Entity.h"

struct game;

s32 renderThread(void* _g); 

const int METERS_TO_PIXELS = 64;
const r32 PIXELS_TO_METERS = 1.0f / METERS_TO_PIXELS;

class Render {
	class camera {
		camera();
		void update(game* g);
		r32 zoom;
		mpos pos;
		entity following;
		friend class Render;
	};

public:

	Render(engine* _e, game* _g);
	~Render();

	void init();
	void kill();

	void endBatch();

	void renderMap();
	void renderDebugHUD();

	void zIn(r32 factor);
	void zOut(r32 factor);

	void startThread();
	void stopThread();

private:
	void renderCTex(std::tuple<bool, mpos, c_tex*> t);
	v2<r32> mapIntoPxSpace(mpos point, mpos origin);
	mpos getTLC();
	mpos getBRC();

	camera cam;

	std::vector<c_tex*> debugTextures;
	enum debugtexturename { // index into debugTextures
		dt_chunkbounds = 0,
		dt_camera = 1,
		dt_profiler = 2
	};

	// for render thread
		// TODO: c_text
	std::priority_queue<std::tuple<bool, mpos, c_tex*>> texq;
	mutex qlock;
	cond_var condRun;
	bool running;

	engine* e;
	game* g;

	friend s32 renderThread(void* _g);
};

