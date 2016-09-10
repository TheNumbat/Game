#pragma once

#include "..\game.h"
#include "Entity.h"

component::component(c_type t, void* data) {
	type = t;
	any = data;
}

c_tex::c_tex() {
	layer = 0;
	rot = 0;
	flip = flip_none;
	blend = blend_alpha;
	mod = color(255, 255, 255, 0);
}

entityMgr::entityMgr() {
	lastUID = 1;
}

entityMgr::~entityMgr() {
	edata.clear();
	pos.clear();
	mov.clear();
	tex.clear();
	text.clear();
	phys.clear();
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
		case ct_pos:
			pos.push_back(c_pos());
			eEntry->second.insert({ type, component(type, &pos.back()) });
			return component(type, &pos.back());
			break;
		case ct_mov:
			mov.push_back(c_mov());
			eEntry->second.insert({ type, component(type, &mov.back()) });
			return component(type, &mov.back());
			break;
		case ct_tex:
			tex.push_back(c_tex());
			eEntry->second.insert({ type, component(type, &tex.back()) });
			return component(type, &tex.back());
			break;
		case ct_text:
			text.push_back(c_text());
			eEntry->second.insert({ type, component(type, &text.back()) });
			return component(type, &text.back());
			break;
		case ct_phys:
			phys.push_back(c_phys());
			eEntry->second.insert({ type, component(type, &phys.back()) });
			return component(type, &phys.back());
			break;
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
	for (auto& itr = cRange.first; itr != cRange.second; ++itr) {
		ret.push_back(itr->second);
	}
	return ret;
}
