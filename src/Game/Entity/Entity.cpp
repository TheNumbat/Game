#pragma once

#include "..\game.h"
#include "Entity.h"

component::component(c_type t, void* data) {
	type = t;
	any = data;
}

component::component(const component& src) {
	type = src.type;
	any = src.any;
}

c_tex::c_tex() {
	layer = 0;
	rot = 0;
	zoom = true;
	flip = flip_none;
	blend = blend_alpha;
	mod = color(255, 255, 255, 0);
}

c_tex::c_tex(const c_tex& src) {
	*this = src;
}

c_tex& c_tex::operator=(const c_tex& src) {
	if (this != &src) {
		ID = src.ID;
		layer = src.layer;
		posRect = src.posRect;
		srcPxlRect = src.srcPxlRect;
		rotPt = src.rotPt;
		rot = src.rot;
		flip = src.flip;
		mod = src.mod;
		blend = src.blend;
	}
	return *this;
}

c_text::c_text() {

}

c_text::c_text(const c_text& src) {
	*this = src;
}

c_text& c_text::operator=(const c_text& src) {
	if (this != &src) {
		ID = src.ID; // unused
		font = src.font;
		msg = src.msg;
		layer = src.layer;
		posRect = src.posRect;
		srcPxlRect = src.srcPxlRect;
		rotPt = src.rotPt;
		rot = src.rot;
		flip = src.flip;
		mod = src.mod;
		blend = src.blend;
	}
	return *this;
}

entityMgr::entityMgr() {
	lastUID = 1;
}

entityMgr::~entityMgr() {
	edata.clear();
}

entity entityMgr::create() {
	lastUID++;
	edata.insert({ lastUID, entitydata() });
	return lastUID;
}

component entityMgr::getC(entity e, c_type type) {
	logSetContext("ENTITY");

	auto eEntry = edata.find(e);
	if (eEntry == edata.end()) {
		logWarn("Failed to find entity ID " + (u32) e);
		return component(ct_none, NULL);
	}

	auto cEntry = eEntry->second.find(type);
	if (cEntry == eEntry->second.end()) {
		logWarn("Failed to find component type " + std::to_string(type) + " in entity ID" + std::to_string((u32) e));
		return component(ct_none, NULL);
	}

	return cEntry->second;
}

component entityMgr::reqC(entity e, c_type type) {
	logSetContext("ENTITY");

	auto eEntry = edata.find(e);
	if (eEntry == edata.end()) {
		logWarn("Failed to find entity ID " + (u32)e);
		return component(ct_none, NULL);
	}

	auto cEntry = eEntry->second.find(type);
	if (cEntry == eEntry->second.end()) {
		return addC(e, type);
	}
	return cEntry->second;
}

component entityMgr::addC(entity e, c_type type) {
	logSetContext("ENTITY");

	auto eEntry = edata.find(e);
	if (eEntry == edata.end()) {
		logWarn("Failed to find entity ID " + (u32)e);
		return component(ct_none, NULL);
	}

	switch (type) {
		case ct_pos: {
			c_pos* cpos = pos.push(c_pos());
			eEntry->second.insert({ type, component(type, cpos) });
			return component(type, cpos);
			break; 
		}
		case ct_tex: {
			c_tex* ctex = tex.push(c_tex());
			eEntry->second.insert({ type, component(type, ctex) });
			return component(type, ctex);
			break; 
		}
		case ct_text: {
			c_text* ctext = text.push(c_text());
			eEntry->second.insert({ type, component(type, ctext) });
			return component(type, ctext);
			break; 
		}
		case ct_phys: {
			c_phys* cphys = phys.push(c_phys());
			eEntry->second.insert({ type, component(type, cphys) });
			return component(type, cphys);
			break;
		}
	}
	assert(false);
	return component(ct_none, NULL);
}

std::vector<component> entityMgr::getMultiC(entity e, c_type type) {
	logSetContext("ENTITY");

	auto eEntry = edata.find(e);
	if (eEntry == edata.end()) {
		logWarn("Failed to find entity ID " + (u32)e);
		return std::vector<component>();
	}

	std::vector<component> ret;
	auto cRange = eEntry->second.equal_range(type);
	for (auto itr = cRange.first; itr != cRange.second; ++itr) {
		ret.push_back(itr->second);
	}
	return ret;
}
