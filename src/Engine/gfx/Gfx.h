#pragma once

#include "..\common.h"
#include "..\r2.h"
#include <map>

enum blendmode : u8 {
	blend_none,
	blend_alpha,
	blend_additive,
	blend_modulate
};

enum flipmode : u8 {
	flip_none,
	flip_horz,
	flip_vert,
	flip_both
};

enum fontstyle : u8 {
	font_normal,
	font_bold,
	font_italic,
	font_outline,
	font_strikethrough,
	font_underline
};

struct color {
	ENGINE_API color();
	ENGINE_API color(u8 _r, u8 _g, u8 _b, u8 _a);
	u8 r, g, b, a;
};

class texture {
	texture();
	~texture();

	bool load(const std::string& path, void* renderer, blendmode b = blend_alpha, color c = color(255, 255, 255, 0));
	bool load(void* surface, void* renderer, blendmode b = blend_alpha, color c = color(255, 255, 255, 0));
	bool free();

	bool setBlendmode(blendmode b);
	blendmode getBlendmode();
	bool setColormod(color c);
	color getColormod(color c);
	
	void* sdl_texture;
	blendmode blend;
	color colormod;
	bool good;

	friend class Gfx;
};

class font {
	font();
	~font();

	bool load(const std::string& path, void* renderer, s32 fsize, color c, fontstyle style);
	bool free();

	s32 getSize();

	std::string file;
	void* fc_font;
	s32 size;
	bool good;

	friend class Gfx;
};

class Gfx
{
public:
	Gfx();
	// Call kill() before destroying
	~Gfx();

	bool init(const std::string& win, u32 width, u32 height);
	bool kill();

	ENGINE_API bool getWinDim(s32& w, s32& h);
	ENGINE_API bool swapFrame(bool clear = true);
	ENGINE_API bool setViewport(r2<s32> port = r2<s32>(0, 0, 0, 0));

	ENGINE_API bool loadFont(const std::string& fontID, const std::string& path, s32 size, fontstyle style, color c = color(255, 255, 255, 255));
	ENGINE_API bool loadTexture(const std::string& texID, const std::string& path, blendmode b = blend_alpha, color c = color(255, 255, 255, 0));

	ENGINE_API bool setBlendmode(const std::string& texID, blendmode b = blend_alpha);
	ENGINE_API bool setColorMod(const std::string& texID, color c = color(255, 255, 255, 0));
	
	ENGINE_API s32 getFontSize(const std::string& fontID);

	ENGINE_API bool loadTexFolder(const std::string& path);
	ENGINE_API bool loadFontFolder(const std::string& path, s32 size);

	ENGINE_API bool freeTexture(const std::string& texID);
	ENGINE_API bool freeFont(const std::string& fontID);

	ENGINE_API bool renderTexture(const std::string& texID, r2<s32> dest_rect);
	ENGINE_API bool renderTextureEx(const std::string& texID, r2<s32> dest_rect, r2<s32> src_rect, v2<s32> rot_pos32, r32 rotation, flipmode flip);

	ENGINE_API bool renderText(const std::string& fontID, const std::string& text, r2<s32> dest_rect);

private:
	std::map<std::string, texture*> textures;
	std::map<std::string, font*> fonts;

	void* sdl_renderer;
	void* sdl_window;
	bool good;
};

