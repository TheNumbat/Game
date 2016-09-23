#pragma once

#include "common.h"
#include <engine.h>
#include <queue>
#include <tuple>
#include "..\Entity\Entity.h"

struct game;

const int METERS_TO_PIXELS = 64;
const r32 PIXELS_TO_METERS = 1.0f / METERS_TO_PIXELS;

class Render {
	typedef std::priority_queue<std::tuple<bool, mpos, c_tex*>> renderBatch;
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

	// not threaded ... feelsbadman
	void batchMap();
	void render(); // renders next batch 
	void renderDebugHUD(); 

	void zIn(r32 factor);
	void zOut(r32 factor);

private:
	u32 recProfRender(Util::profNode* node, u32 pos, u32 lvl, u32 fontsize);
	void renderCTex(std::tuple<bool, mpos, c_tex*> t);
	v2<r32> mapIntoPxSpace(mpos point, mpos origin);
	
	void updateTLC();
	void updateBRC();

	camera cam;
	mpos TLC, BRC;

	std::vector<c_tex*> debugTextures;
	enum debugtexturename { // index into debugTextures
		dt_chunkbounds = 0,
		dt_camera = 1
	};

	std::queue<renderBatch> batchQueue;

	engine* e;
	game* g;
};

