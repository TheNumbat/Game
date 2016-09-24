#pragma once

#include "common.h"
#include <engine.h>
#include <queue>
#include "..\Entity\Entity.h"

struct game;

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
	class renderable {
		renderable(c_tex* t, mpos b, bool del);
		c_tex* texture;
		mpos base;
		bool deleteAfter;
		friend class Render;
	};
	struct renderableless { // for priority_queue
		bool operator()(const renderable& l, const renderable& r) {
			// This is flipped so lowest layer is highest priority
			if (l.texture->layer < r.texture->layer) return false;
			if (l.texture->layer > r.texture->layer) return true;
			return (l.base.chunk.y * CHUNK_SIZE_METERS + l.base.real.y + l.texture->posRect.y + l.texture->posRect.h) >
				(r.base.chunk.y * CHUNK_SIZE_METERS + r.base.real.y + r.texture->posRect.y + r.texture->posRect.h);
		}
	};

	typedef std::priority_queue<renderable, std::vector<renderable>, renderableless> renderBatch;

public:

	Render(engine* _e, game* _g);
	~Render();

	void init();
	void kill();

	// not threaded ... feelsbadman
	void batchMap();
	void batchDebugHUD(); 
	void render(); // renders next batch 

	void zIn(r32 factor);
	void zOut(r32 factor);

private:
	u32 recProfRender(Util::profNode* node, u32 pos, u32 lvl, u32 fontsize);
	void renderRenderable(const renderable* r);
	v2<r32> mapIntoPxSpace(mpos point, mpos origin);
	
	void updateCorners();

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
