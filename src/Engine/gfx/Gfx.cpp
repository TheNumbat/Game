
#include "gfx.h"
#include "..\log\log.h"
#include <SDL.h>
#include <SDL_gpu.h>
#include <SDL_FontCache.h>
#include <dirent.h>

color::color() {
	r = 0;
	g = 0;
	b = 0;
	a = 0;
}

color::color(u8 _r, u8 _g, u8 _b, u8 _a) {
	r = _r;
	g = _g;
	b = _b;
	a = _a;
}

texture::texture() {
	image = NULL;
	blend = blend_alpha;
	colormod = color(0, 0, 0, 0);
	good = false;
}

texture::~texture() {
	free();
}

bool texture::load(const std::string& path, blendmode b, color c) {
	free();

	image = GPU_LoadImage(path.c_str());
	assert(image);
	if (!image) {
		logSetContext("GRAPHICS");
		logWarn("Failed to load texture from path " + path);
	}

	if (!setBlendmode(b)) {
		free();
		return false;
	}
	if (!setColormod(c)) {
		free();
		return false;
	}

	good = true;
	return true;
}

bool texture::free() {
	if (good) {
		GPU_FreeImage((GPU_Image*)image);
		image = NULL;
		blend = blend_alpha;
		colormod = color(0, 0, 0, 0);
		good = false;
		return true;
	}
	return false;
}

bool texture::setBlendmode(blendmode b) {
	switch (b) {
		case blend_alpha:
			GPU_SetBlendMode((GPU_Image*)image, GPU_BLEND_NORMAL);
			break;
		case blend_additive:
			GPU_SetBlendMode((GPU_Image*)image, GPU_BLEND_ADD);
			break;
		case blend_modulate:
			GPU_SetBlendMode((GPU_Image*)image, GPU_BLEND_MOD_ALPHA);
			break;
		case blend_none:
			GPU_SetBlendMode((GPU_Image*)image, GPU_BLEND_SET);
			break;
	}

	blend = b;
	return true;
}

blendmode texture::getBlendmode() {
	return blend;
}

bool texture::setColormod(color c) {
	GPU_SetRGBA((GPU_Image*)image, c.r, c.g, c.b, c.a);
	return true;
}

color texture::getColormod(color c) {
	return colormod;
}

font::font() {
	fc_font = NULL;
	size = 0;
	good = false;
}

font::~font() {
	free();
}

bool font::load(const std::string& path, s32 fsize, color c, fontstyle style) {
	free();

	fc_font = FC_CreateFont();
	int result;
	switch (style) {
	case font_normal:
		result = FC_LoadFont((FC_Font*)fc_font, path.c_str(), fsize, FC_MakeColor(c.r, c.g, c.b, c.a), TTF_STYLE_NORMAL);
		break;
	case font_bold:
		result = FC_LoadFont((FC_Font*)fc_font, path.c_str(), fsize, FC_MakeColor(c.r, c.g, c.b, c.a), TTF_STYLE_BOLD);
		break;
	case font_italic:
		result = FC_LoadFont((FC_Font*)fc_font, path.c_str(), fsize, FC_MakeColor(c.r, c.g, c.b, c.a), TTF_STYLE_ITALIC);
		break;
	case font_outline:
		result = FC_LoadFont((FC_Font*)fc_font, path.c_str(), fsize, FC_MakeColor(c.r, c.g, c.b, c.a), TTF_STYLE_OUTLINE);
		break;
	case font_strikethrough:
		result = FC_LoadFont((FC_Font*)fc_font, path.c_str(), fsize, FC_MakeColor(c.r, c.g, c.b, c.a), TTF_STYLE_STRIKETHROUGH);
		break;
	case font_underline:
		result = FC_LoadFont((FC_Font*)fc_font, path.c_str(), fsize, FC_MakeColor(c.r, c.g, c.b, c.a), TTF_STYLE_UNDERLINE);
		break;
	}
	assert(result != 0);
	if (result == 0) {
		logSetContext("GRAPHICS");
		logWarn("Failed to load font from path " + path + " error: " + TTF_GetError());
		return false;
	}
	
	file = path;
	size = fsize;
	good = true;
	return true;
}

bool font::free() {
	if (good) {
		FC_FreeFont((FC_Font*)fc_font);
		fc_font = NULL;
		size = 0;
		good = false;
		return true;
	}
	return false;
}

s32 font::getSize() {
	return size;
}

Gfx::Gfx() {
	window = NULL;
	good = false;
}

Gfx::~Gfx() {}

bool Gfx::init(const std::string& win, u32 width, u32 height) {
	logSetContext("GRAPHICS");

	if (good) {
		logWarn("Graphics already initialized");
		return false;
	}

	logInfo("Initializing Graphics...");
	logEnterSec();

	logInfo("Initializing SDL Video");
	bool result = SDL_InitSubSystem(SDL_INIT_VIDEO) == 0;
	assert(result);
	if (!result) {
		logErr((std::string) "Failed to initialize SDL Video. Error: " + SDL_GetError());
		return false;
	}

	logInfo("Initializing SDL_TTF");
	result = TTF_Init() == 0;
	assert(result);
	if (!result) {
		logErr((std::string) "Failed to initialize SDL_TTF. Error: " + TTF_GetError());
		return false;
	}

	logInfo("Creating Window and Renderer");
	GPU_SetDebugLevel(GPU_DEBUG_LEVEL_MAX);
	window = GPU_Init(width, height, 0);
	// TODO: tune attributes
	SDL_GL_SetSwapInterval(0);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
	Ww = width;
	Wh = height;
	assert(window);
	if (!window) {
		logErr((std::string) "Failed to create window rendering target.");
		return false;
	}

	logInfo("Done initializing graphics.");
	logExitSec();


	good = true;
	return true;
}

bool Gfx::kill() {
	logSetContext("GRPAHICS");

	if (!good) {
		logWarn("Graphics already deinitialized.");
		return false;
	}

	logInfo("Destroying Graphics");
	logEnterSec();

	for (auto i : textures)
		delete i.second;
	for (auto i : fonts)
		delete i.second;

	logInfo("Destroying Window");
	GPU_Quit();
	window = NULL;

	logInfo("Quitting SDL Video");
	SDL_QuitSubSystem(SDL_INIT_VIDEO);

	logInfo("Quitting SDL_TTF");
	TTF_Quit();

	logInfo("Done destroying graphics.");
	logExitSec();

	good = false;
	return true;
}


bool Gfx::getWinDim(s32& w, s32& h) {
	logSetContext("GRPAHICS");

	w = Ww;
	h = Wh;
	return true;
}

bool Gfx::swapFrame(bool clear) {
	logSetContext("GRPAHICS");

	GPU_Flip((GPU_Target*)window);

	if (clear) {
		GPU_Clear((GPU_Target*)window);
	}

	return true;
}

bool Gfx::setViewport(r2<r32> port) {
	logSetContext("GRPAHICS");

	GPU_Rect viewport;

	if (port.w == 0 || port.h == 0) {
		s32 w, h;
		if (!getWinDim(w, h)) return false;
		viewport.x = 0;
		viewport.y = 0;
		viewport.w = w;
		viewport.h = h;
	} else {
		viewport.x = port.x;
		viewport.y = port.y;
		viewport.w = port.w;
		viewport.h = port.h;
	}
	
	GPU_SetViewport((GPU_Target*)window, viewport);
	return true;
}

bool Gfx::loadFont(const std::string& fontID, const std::string& path, s32 size, fontstyle style, color c) {
	logSetContext("GRPAHICS");

	if (fonts.find(fontID) != fonts.end())
	{
		logWarn("Font ID " + fontID + " already taken!");
		return false;
	}

	font* newF = new font;

	if (!newF->load(path, size, c, style))
	{
		logWarn("Failed to load font ID: " + fontID + " from " + path);
		delete newF;
		return false;
	}

	fonts.insert({fontID, newF});
	logInfo("Loaded font ID: " + fontID + " from " + path);
	return true;
}

bool Gfx::loadTexture(const std::string& texID, const std::string& path, blendmode b, color c) {
	logSetContext("GRPAHICS");

	if (textures.find(texID) != textures.end())
	{
		logWarn("Texture ID " + texID + " already taken!");
		return false;
	}

	texture* newT = new texture;

	if (!newT->load(path, b, c))
	{
		logWarn("Failed to load texture ID: " + texID + " from " + path);
		delete newT;
		return false;
	}

	textures.insert({texID, newT});
	logInfo("Loaded texture ID: " + texID + " from " + path);
	return true;
}

bool Gfx::setBlendmode(const std::string& texID, blendmode b) {
	logSetContext("GRAPHICS");

	auto textureItem = textures.find(texID);
	if(textureItem == textures.end()) {
		logWarn("Can't set texture blend mode, could not find loaded texture with ID: " + texID);
		return false;
	}

	if(!textureItem->second->setBlendmode(b))
	{
		logWarn("Failed to set texture blend mode for texture ID: " + texID);
		return false;
	}

	return true;
}

bool Gfx::setColorMod(const std::string& texID, color c) {
	logSetContext("GRAPHICS");

	auto textureItem = textures.find(texID);
	if(textureItem == textures.end()) {
		logWarn("Can't set texture blend mode, could not find loaded texture with ID: " + texID);
		return false;
	}

	if(!textureItem->second->setColormod(c))
	{
		logWarn("Failed to set texture color mod for texture ID: " + texID);
		return false;
	}

	return true;
}

s32 Gfx::getFontSize(const std::string& fontID) {
	logSetContext("GRAPHICS");

	auto fontItem = fonts.find(fontID);
	if(fontItem == fonts.end()) {
		logWarn("Could not find loaded font with ID: " + fontID);
		return 0;
	}

	return fontItem->second->getSize();
}

bool Gfx::loadTexFolder(const std::string& path) {
	logSetContext("GRAPHICS");

	DIR *directory;
	dirent *entry;
	std::string dirPath = path;

	if (dirPath.back() != '/' && dirPath.back() != '\\')
	{
		dirPath.append("/");
	} 

	directory = opendir(dirPath.c_str());
	assert(directory);
	if (!directory) {
		logWarn("Failed to open texture directory at " + dirPath);
		return false;
	}

	logInfo("Loading texture directory at " + dirPath);
	logEnterSec();

	while (entry = readdir(directory)) {
		std::string entryName = entry->d_name;

		if (entryName != ".." && entryName != ".") {
			std::string ext = entryName.substr(entryName.length() - 4, entryName.length());
			if (ext == ".bmp" ||
				ext == ".gif" ||
				ext == ".jpg" ||
				ext == ".png" )
				loadTexture(entryName.substr(0,entryName.length() - 4), dirPath + entryName);
			else if (ext[0] != '.')
				loadTexFolder(dirPath + entryName + '/');
		}
	}

	logExitSec();
	return true;
}

bool Gfx::loadFontFolder(const std::string& path, s32 size) {
	logSetContext("GRAPHICS");

	DIR *directory;
	dirent *entry;
	std::string dirPath = path;

	if (dirPath.back() != '/' && dirPath.back() != '\\')
	{
		dirPath.append("/");
	} 

	directory = opendir(dirPath.c_str());
	assert(directory);
	if (!directory) {
		logWarn("Failed to open font directory at " + dirPath);
		return false;
	}

	logInfo("Loading font directory at " + dirPath);
	logEnterSec();

	while (entry = readdir(directory))
	{
		std::string entryName = entry->d_name;

		if (entryName != ".." && entryName != ".") {
			std::string ext = entryName.substr(entryName.length() - 4, entryName.length());
			if (ext == ".ttf")
				loadFont(entryName.substr(0, entryName.length() - 4), dirPath + entryName, size, font_normal);
			else if (ext[0] != '.')
				loadFontFolder(dirPath + entryName + '/', size);
		}
	}

	logExitSec();
	return true;
}

bool Gfx::freeTexture(const std::string& texID) {
	logSetContext("GRAPHICS");

	auto tex = textures.find(texID);
	if (tex == textures.end()) {
		logWarn("Could not find to free texture ID: " + texID);
		return false;
	}

	delete tex->second;
	textures.erase(tex);
	return true;
}

bool Gfx::freeFont(const std::string& fontID) {
	logSetContext("GRAPHICS");

	auto f = fonts.find(fontID);
	if (f == fonts.end()) {
		logWarn("Could not find to free font ID: " + fontID);
		return false;
	}

	delete f->second;
	fonts.erase(f);
	return true;
}

bool Gfx::renderTexture(const std::string& texID, r2<r32> dest_rect) {
	logSetContext("GRAPHICS");

	auto textureItem = textures.find(texID);
	if (textureItem == textures.end()) {
		logWarn("Can't display texture, could not ID: " + texID);
		return false;
	}

	GPU_Image* image = (GPU_Image*)textureItem->second->image;

	GPU_BlitScale(image, NULL, (GPU_Target*)window, dest_rect.x, dest_rect.y, dest_rect.w / image->w, dest_rect.h / image->h);

	return true;
}

bool Gfx::renderTextureEx(const std::string& texID, r2<r32> dest_rect, r2<r32> src_rect, v2<r32> rot_pt, r32 rotation) {
	logSetContext("GRAPHICS");

	auto textureItem = textures.find(texID);
	if (textureItem == textures.end()) {
		logWarn("Can't display texture, could not find loaded texture with ID: " + texID);
		return false;
	}

	GPU_Image* image = (GPU_Image*)textureItem->second->image;
	
	GPU_Rect src;
	src.x = src_rect.x;
	src.y = src_rect.y;
	if (src_rect.w == 0 || src_rect.h == 0) {
		src.w = image->w;
		src.h = image->h;
	} else {
		src.w = src_rect.w;
		src.h = src_rect.h;
	}
	if (dest_rect.w == 0 || dest_rect.h == 0) {
		dest_rect.w = image->w;
		dest_rect.h = image->h;
	}

	GPU_BlitTransformX(image, &src, (GPU_Target*)window, dest_rect.x, dest_rect.y, rot_pt.x, rot_pt.y, rotation, dest_rect.w / image->w, dest_rect.h / image->h);

	return true;
}

bool Gfx::renderText(const std::string& fontID, const std::string& text, r2<r32> dest_rect) {
	logSetContext("GRAPHICS");

	auto fentry = fonts.find(fontID);
	if (fentry == fonts.end()) {
		logWarn("Failed to find font ID " + fontID);
		return false;
	}

	FC_Draw((FC_Font*)fentry->second->fc_font, (GPU_Target*)window, dest_rect.x, dest_rect.y, text.c_str());

	return true;
}
