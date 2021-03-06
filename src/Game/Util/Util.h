#pragma once

#include "common.h"
#include <engine.h>
#include <map>

struct game;

typedef bool (*consoleFunc)(game* g, engine* e, const std::string& args);

enum debug_flag : u16 {
	renderDebugUI = 1<<0,
	renderChunkbounds = 1<<1,
	renderCamera = 1<<2,
	renderPositionText = 1<<3,
	renderCollisionBounds = 1<<4
};

class Util {
	struct profNode {
		profNode(const std::string& n, u64 s, profNode* p = NULL);
		~profNode();
		std::string name;
		u64 start, self, heir, calls;

		// TODO: should this really be a map? 
		std::map<std::string, profNode*> children;
		profNode* parent;
		bool showChildren;
	};

	struct utilVal {
		virtual std::string getStr() = 0;
	};

	template<typename T>
	struct trackedVal : public utilVal {
		trackedVal(T* v) { val = v; }
		std::string getStr() { return std::to_string(*val); }
		T* val;
	};

	template<typename T>
	struct staticVal : public utilVal {
		staticVal(T v) { val = v; }
		std::string getStr() { return std::to_string(val); }
		T val;
	};

public:
	Util(engine* _e, game* _g);
	~Util();

	// For mapping a string through a table of function pointers
		// (game chat console)
	bool callConsoleFunc(const std::string& input);
	void reloadConsoleFuncs();

	void beginFrame();
	void endFrame();

	void beginThreadProf(const std::string& name, u8 thread, bool select = false);

	void beginFunc(const std::string& name, u8 thread = 0);
	#define beginFunc(t) beginFunc(__func__, t)
	void endFunc(u8 thread = 0);

	template<typename T>
	bool trackVal(const std::string& ID, T* val);
	#define trackVal(val) trackVal(#val, &val);
	template<typename T>
	bool addVal(const std::string& ID, T val);
	#define addVal(val) addVal(#val, val)

	bool releaseVal(const std::string& ID);
	#define releaseVal(val) releaseVal(#val)

	void resetAvgFrame();
	u64 getAvgFrame();
	u64 getLastFrame();

	void setFpsCap(u8 fps = 0);

	bool getFlag(debug_flag f);
	void setFlag(debug_flag f);
	void clearFlag(debug_flag f);
	void toggleFlag(debug_flag f);

	void toggleProfiler();

	// Vertically - not necessarily to parent/child
	void selectedNodeUp();
	void selectedNodeDown();
	void toggleChildren();
	
	void resetProfNodesRec(profNode* place);

private:
	u8 fpsCap;
	u16 flags;
	u64 lastFrameTime;
	// are reset with resetavgframe
	u64 totalFrameTime;
	u64 totalFrames;

	bool profilerPaused;
	bool toggleProf;
	std::map<u8, profNode*> heads;
	std::map<u8, profNode*> currents;
	std::pair<u8, profNode*> selected;

	// TODO: set this up in constructor + reload function
	std::map<std::string, consoleFunc> consoleFuncs;
	std::map<std::string, utilVal*> values;

	engine* e;
	game* g;

	friend class Render;
};

