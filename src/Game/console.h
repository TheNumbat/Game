
#include "common.h"
#include "game.h"
#include <engine.h>
#include <sstream>

#define FUNC(name) bool name(game* g, engine* e, const std::string& args)

// These should log any errors
namespace console {

	FUNC(quit) {
		g->running = false;
		return true;
	}

	FUNC(log) {
		logSetContext("DEBUG");
		std::stringstream ss(args);
		std::string context, msg;
		ss >> context;
		msg = args.substr(context.size() + 2, args.size());
		if (ss.fail()) {
			logWarn("Failed to parse input in console func: log");
			return false;
		}
		logChangeContext(context);
		logInfo(msg);
		return true;
	}
}