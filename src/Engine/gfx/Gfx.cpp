#include "gfx.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <dirent.h>
#include "..\log\log.h"

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
	sdl_texture = NULL;
	blend = blend_alpha;
	colormod = color(0, 0, 0, 0);
	good = false;
}

texture::~texture() {
	free();
}

bool texture::load(const std::string& path, void* renderer, blendmode b, color c) {
	free();

	SDL_Surface* buf = IMG_Load(path.c_str());
	assert(buf);
	if (!buf) {
		logSetContext("GRAPHICS");
		logWarn("Failed to load texture from path " + path + " error: " + IMG_GetError());
		return false;
	}

	bool result = load(buf, renderer, b, c);
	SDL_FreeSurface(buf);

	return result;
}

bool texture::load(void* surface, void* renderer, blendmode b, color c) {
	
	sdl_texture = SDL_CreateTextureFromSurface((SDL_Renderer*)renderer, (SDL_Surface*)surface);

	assert(sdl_texture);
	if (!sdl_texture) {
		logSetContext("GRAPHICS");
		logWarn((std::string)"Failed to create texture from surface. Error: " + SDL_GetError());
		return false;
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
		SDL_DestroyTexture((SDL_Texture*)sdl_texture);
		sdl_texture = NULL;
		blend = blend_alpha;
		colormod = color(0, 0, 0, 0);
		good = false;
		return true;
	}
	return false;
}

bool texture::setBlendmode(blendmode b) {
	int result = 1;

	switch (b)
	{
		case blend_alpha:
			result = SDL_SetTextureBlendMode((SDL_Texture*)sdl_texture, SDL_BLENDMODE_BLEND);
			break;
		case blend_additive:
			result = SDL_SetTextureBlendMode((SDL_Texture*)sdl_texture, SDL_BLENDMODE_ADD);
			break;
		case blend_modulate:
			result = SDL_SetTextureBlendMode((SDL_Texture*)sdl_texture, SDL_BLENDMODE_MOD);
			break;
		case blend_none:
			result = SDL_SetTextureBlendMode((SDL_Texture*)sdl_texture, SDL_BLENDMODE_NONE);
			break;
	}

	assert(result == 0);
	if (result != 0) {
		logSetContext("GRAPHICS");
		logWarn((std::string)"Failed to set blendmode. Error: " + SDL_GetError());
		return false;
	}

	blend = b;
	return true;
}

blendmode texture::getBlendmode() {
	return blend;
}

bool texture::setColormod(color c) {
	int result = SDL_SetTextureColorMod((SDL_Texture*)sdl_texture, c.r, c.g, c.b);
	assert(result == 0);
	if (result != 0) {
		logSetContext("GRAPHICS");
		logWarn((std::string)"Failed to color mod. Error: " + SDL_GetError());
		return false;
	}
	return true;
}

color texture::getColormod(color c) {
	return colormod;
}

font::font() {
	sdl_font = NULL;
	size = 0;
	good = false;
}

font::~font() {
	free();
}

bool font::load(const std::string& path, s32 fsize) {
	free();

	sdl_font = TTF_OpenFont(path.c_str(), fsize);
	assert(sdl_font);
	if (!sdl_font) {
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
		TTF_CloseFont((TTF_Font*)sdl_font);
		sdl_font = NULL;
		size = 0;
		good = false;
		return true;
	}
	return false;
}

bool font::loadSize(s32 s) {
	free();
	return load(file, s);
}

s32 font::getSize() {
	return size;
}

Gfx::Gfx() {
	sdl_renderer = NULL;
	sdl_window = NULL;
	good = false;
}

Gfx::~Gfx() {
	for (auto i : textures)
		delete i.second;
	for (auto i : fonts)
		delete i.second;
}

bool Gfx::init(const std::string& win, u32 width, u32 height) {
	logSetContext("GRAPHICS");

	if (good) {
		logWarn("Graphics already initialized");
		return false;
	}

	logMsg("Initializing Graphics...");
	logEnterSec();

	logMsg("Initializing SDL Video");
	bool result = SDL_InitSubSystem(SDL_INIT_VIDEO) == 0;
	assert(result);
	if (!result) {
		logErr((std::string) "Failed to initialize SDL Video. Error: " + SDL_GetError());
		return false;
	}

	logMsg("Initializing SDL_Image");
	result = IMG_Init(IMG_INIT_JPG | IMG_INIT_JPG) != 0;
	assert(result);
	if (!result) {
		logErr((std::string) "Failed to initialize SDL_Image. Error: " + IMG_GetError());
		return false;
	}

	logMsg("Initializing SDL_TTF");
	result = TTF_Init() == 0;
	assert(result);
	if (!result) {
		logErr((std::string) "Failed to initialize SDL_TTF. Error: " + TTF_GetError());
		return false;
	}

	logMsg("Creating Window and Renderer");
	// TODO OpenGL
	SDL_Window* temp1;
	SDL_Renderer* temp2;
	result = SDL_CreateWindowAndRenderer(width, height, NULL, &temp1, &temp2) == 0;
	assert(result);
	if (!result) {
		logErr((std::string) "Failed to create SDL window and renderer. Error: " + SDL_GetError());
		return false;
	}
	sdl_window = temp1;
	sdl_renderer = temp2;

	logMsg("Done initializing graphics.");
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

	logMsg("Deinitializing Graphics");

	logMsg("Destroying Renderer");
	SDL_DestroyRenderer((SDL_Renderer*)sdl_renderer);
	logMsg("Destroying Window");
	SDL_DestroyWindow((SDL_Window*)sdl_window);

	sdl_renderer = NULL;
	sdl_window = NULL;

	logMsg("Quitting SDL Video");
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
	logMsg("Quitting SDL_Image");
	IMG_Quit();
	logMsg("Quitting SDL_TTF");
	TTF_Quit();

	logMsg("Done deinitializing graphics.");
	logExitSec();

	good = false;
	return true;
}


bool Gfx::getWinDim(s32& w, s32& h) {
	logSetContext("GRPAHICS");

	if (!good) {
		logWarn("Cannot get window size, graphics not initialized.");
		return false;
	}

	assert(sdl_window);
	SDL_GetWindowSize((SDL_Window*)sdl_window, &w, &h);
	return true;
}

bool Gfx::swapFrame(bool clear) {
	logSetContext("GRPAHICS");

	if (!good) {
		logWarn("Cannot get display window, graphics not initialized.");
		return false;
	}

	SDL_RenderPresent((SDL_Renderer*)sdl_renderer);

	if (clear) {
		if (SDL_RenderClear((SDL_Renderer*)sdl_renderer)) {
			logWarn((std::string) "Could not clear renderer, error: " + SDL_GetError());
			return false;
		}
	}

	return true;
}

bool Gfx::setViewport(r2<s32> port) {
	logSetContext("GRPAHICS");

	SDL_Rect viewport;

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
	
	if (SDL_RenderSetViewport((SDL_Renderer*)sdl_renderer, &viewport) != 0) {
		logWarn((std::string) "Could not set viewport! Error: " + SDL_GetError());
		return false;
	}

	return true;
}

bool Gfx::loadFont(const std::string& fontID, const std::string& path, s32 size) {
	logSetContext("GRPAHICS");

	if (fonts.find(fontID) != fonts.end())
	{
		logWarn("Font ID " + fontID + " already taken!");
		return false;
	}

	font* newF = new font;

	if (!newF->load(path, size))
	{
		logWarn("Failed to load font ID: " + fontID + " from " + path);
		delete newF;
		return false;
	}

	fonts.insert({fontID, newF});
	logMsg("Loaded font ID: " + fontID + " from " + path);
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

	if (!newT->load(path, sdl_renderer, b, c))
	{
		logWarn("Failed to load texture ID: " + texID + " from " + path);
		delete newT;
		return false;
	}

	textures.insert({texID, newT});
	logMsg("Loaded texture ID: " + texID + " from " + path);
	return true;
}

bool Gfx::addTextTexture(const std::string& texID, const std::string& fontID, const std::string& text, blendmode b, color c) {
	logSetContext("GRAPHICS");

	if(textures.find(texID) != textures.end()) {
		logWarn("Texture ID " + texID + " already taken!");
		return false;
	}

	auto fontEntry = fonts.find(fontID);
	if(fontEntry == fonts.end()) {
		logWarn("Font ID " + fontID + " could not be found!");
		return false;
	}

	SDL_Color sdl_color;
	sdl_color.r = c.r;
	sdl_color.g = c.g;
	sdl_color.b = c.b;
	sdl_color.a = c.a;

	// TODO more text rendering options
	SDL_Surface* textSurface = TTF_RenderText_Solid((TTF_Font*)fontEntry->second->sdl_font,text.c_str(),sdl_color);

	assert(textSurface);
	if(!textSurface) {
		logWarn("Failed to create surface from font ID " + fontID + " Error: " + TTF_GetError());
		return false;
	}

	texture* newT = new texture;
	if(!newT->load(textSurface,sdl_renderer,b,c)) {
		SDL_FreeSurface(textSurface);
		logWarn("Failed to load texture from font surface");
		delete newT;
		return false;
	}

	SDL_FreeSurface(textSurface);
	textures.insert({texID,newT});
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

bool Gfx::setFontSize(const std::string& fontID, s32 size) {
	logSetContext("GRAPHICS");

	auto fontItem = fonts.find(fontID);
	if(fontItem == fonts.end()) {
		logWarn("Can't set font size, could not find loaded texture with ID: " + fontID);
		return false;
	}

	if(!fontItem->second->loadSize(size))
	{
		logWarn("Failed to set font size for font ID: " + fontID);
		return false;
	}

	return true;
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

	logMsg("Loading texture directory at " + dirPath);
	logEnterSec();

	while (entry = readdir(directory)) {
		std::string entryName = entry->d_name;

		if (entryName != ".." && entryName != ".") {
			// TODO actually test for file vs. folder 
			if (entryName[entryName.size() - 4] == '.') {
				loadTexture(entryName.substr(0,entryName.length() - 4), dirPath + entryName);
			} else {
				loadTexFolder(dirPath + entryName + '/');
			}
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

	logMsg("Loading font directory at " + dirPath);
	logEnterSec();

	while (entry = readdir(directory))
	{
		std::string entryName = entry->d_name;

		if (entryName != ".." && entryName != ".") {
			// TODO actually test for file vs. folder 
			if (entryName[entryName.size() - 4] == '.') {
				loadFont(entryName.substr(0, entryName.length() - 4), dirPath + entryName, size);
			} else {
				loadFontFolder(dirPath + entryName + '/', size);
			}
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

bool Gfx::renderTexture(const std::string& texID, r2<s32> dest_rect) {
	logSetContext("GRAPHICS");

	if (!good) {
		logWarn("Can't render texture, graphics not initialized");
		return false;
	}

	auto textureItem = textures.find(texID);
	if (textureItem == textures.end()) {
		logWarn("Can't display texture, could not ID: " + texID);
		return false;
	}

	int tw, th;
	if (SDL_QueryTexture((SDL_Texture*)textureItem->second->sdl_texture,NULL,NULL,&tw,&th) != 0) {
		logWarn((std::string)"Couldn't query texture, SDL_Error: " + SDL_GetError());
		return false;
	}

	SDL_Rect sdl_dest_rect;
	sdl_dest_rect.x = dest_rect.x;
	sdl_dest_rect.y = dest_rect.y;

	if (dest_rect.w == 0)
		sdl_dest_rect.w = tw;
	else
		sdl_dest_rect.w = dest_rect.w;
	if (dest_rect.h == 0)
		sdl_dest_rect.h = th;
	else
		sdl_dest_rect.h = dest_rect.h;

	if (SDL_RenderCopy((SDL_Renderer*)sdl_renderer,(SDL_Texture*)textureItem->second->sdl_texture,NULL,&sdl_dest_rect) != 0)
	{
		logWarn("Failed to Render texture ID: " + texID + ", Error: " + SDL_GetError());
		return false;
	}

	return true;
}

bool Gfx::renderTextureEx(const std::string& texID, r2<s32> dest_rect, r2<s32> src_rect, v2<s32> rot_point, r32 rotation, flipmode flip) {
	logSetContext("GRAPHICS");

	if (!good) {
		logWarn("Can't display texture, graphics not initialized");
		return false;
	}

	auto textureItem = textures.find(texID);
	if (textureItem == textures.end()) {
		logWarn("Can't display texture, could not find loaded texture with ID: " + texID);
		return false;
	}

	int tw, th;
	if (SDL_QueryTexture((SDL_Texture*)textureItem->second->sdl_texture,NULL,NULL,&tw,&th) != 0)
	{
		logWarn((std::string)"Couldn't query texture, Error: " + SDL_GetError());
		return false;
	}

	SDL_Rect sdl_dest_rect;
	sdl_dest_rect.x = dest_rect.x;
	sdl_dest_rect.y = dest_rect.y;

	if (dest_rect.w == 0)
		sdl_dest_rect.w = tw;
	else
		sdl_dest_rect.w = dest_rect.w;
	if (dest_rect.h == 0)
		sdl_dest_rect.h = th;
	else
		sdl_dest_rect.h = dest_rect.h;

	SDL_Rect sdl_src_rect;
	sdl_src_rect.x = src_rect.x;
	sdl_src_rect.y = src_rect.y;

	if (src_rect.w == 0)
		sdl_src_rect.w = tw;
	else
		sdl_src_rect.w = src_rect.w;
	if (src_rect.h == 0)
		sdl_src_rect.h = th;
	else
		sdl_src_rect.h = src_rect.h;

	SDL_Point sdl_rot_point;
	sdl_rot_point.x = rot_point.x;
	sdl_rot_point.y = rot_point.y;

	SDL_RendererFlip sdl_flip = SDL_FLIP_NONE;
	switch(flip) {
		case flip_both:
			sdl_flip = (SDL_RendererFlip)(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
			break;
		case flip_horz:
			sdl_flip = (SDL_RendererFlip)SDL_FLIP_HORIZONTAL;
			break;
		case flip_vert:
			sdl_flip = (SDL_RendererFlip)SDL_FLIP_VERTICAL;
			break;	
	}

	if (SDL_RenderCopyEx((SDL_Renderer*)sdl_renderer,(SDL_Texture*)textureItem->second->sdl_texture,
						 &sdl_src_rect,&sdl_dest_rect,rotation,&sdl_rot_point,sdl_flip) != 0) {
		logWarn("Failed to Render texture ID: " + texID + ", Error: " + SDL_GetError());
		return false;
	}
	
	return true;
}

bool Gfx::renderText(const std::string& fontID, const std::string& text, r2<s32> dest_rect, blendmode b, color c) {
	logSetContext("GRAPHICS");

	if(!addTextTexture("textrender_temp",fontID,text,b,c))
	{
		return false;
	}
	if(!renderTexture("textrender_temp",dest_rect))
	{
		return false;
	}
	if(!freeTexture("textrender_temp"))
	{
		return false;
	}
	return true;
}

bool Gfx::renderTextEx(const std::string& fontID, const std::string& text, r2<s32> dest_rect, r2<s32> src_rect, blendmode b, color c, v2<s32> rot_point, r32 rot, flipmode flip) {
	logSetContext("GRAPHICS");

	if(!addTextTexture("textrender_temp",fontID,text,b,c))
	{
		return false;
	}
	if(!renderTextureEx("textrender_temp",dest_rect,src_rect,rot_point,rot,flip))
	{
		return false;
	}
	if(!freeTexture("textrender_temp"))
	{
		return false;
	}
	return true;
}
