
// Inclusions
#include "Texture_Manager.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <iostream>
#include <memory>
#include <string>
#include <stdexcept>
#include <utility>

// Public constructors definitions
Gui::Texture_Manager::Texture_Manager() : texture(),
                                          nativeWidth(),
                                          nativeHeight(),
                                          sourceRectangle(),
                                          destinationRectangle(),
                                          loaded(false) {
	
}
Gui::Texture_Manager::Texture_Manager(Texture_Manager && _other) noexcept : texture(std::move(_other.texture)),
                                                                            nativeWidth(std::move(_other.nativeWidth)),
                                                                            nativeHeight(std::move(_other.nativeHeight)),
                                                                            sourceRectangle(std::move(_other.sourceRectangle)),
                                                                            destinationRectangle(std::move(_other.destinationRectangle)),
                                                                            loaded(std::move(_other.loaded)) {

	_other.texture = nullptr;
}

// Public destructor definition
Gui::Texture_Manager::~Texture_Manager() {
	
	if (texture)
		SDL_DestroyTexture(*texture);

}

// Private methods declarations
void                   Gui::Texture_Manager::loadTexture(SDL_Renderer * _renderer, const std::string & _path) {

	if (!loaded) {
		SDL_Surface * tempSurface = IMG_Load(_path.c_str());

		if (!tempSurface)
			throw std::runtime_error("Image loading failed!");

		nativeWidth = tempSurface->w;
		nativeHeight = tempSurface->h;
		texture = std::make_unique<SDL_Texture *>(SDL_CreateTextureFromSurface(_renderer, tempSurface));

		sourceRectangle.x = 0;
		sourceRectangle.y = 0;
		sourceRectangle.w = nativeWidth;
		sourceRectangle.h = nativeHeight;

		destinationRectangle.x = 0;
		destinationRectangle.y = 0;
		destinationRectangle.w = nativeWidth;
		destinationRectangle.h = nativeHeight;

		SDL_FreeSurface(tempSurface);

		loaded = true;
	}

}
void                   Gui::Texture_Manager::loadTexture(SDL_Renderer * _renderer, SDL_Surface * _surface) {

	if (!loaded) {
		nativeWidth = _surface->w;
		nativeHeight = _surface->h;
		texture = std::make_unique<SDL_Texture *>(SDL_CreateTextureFromSurface(_renderer, _surface));

		sourceRectangle.x = 0;
		sourceRectangle.y = 0;
		sourceRectangle.w = nativeWidth;
		sourceRectangle.h = nativeHeight;

		destinationRectangle.x = 0;
		destinationRectangle.y = 0;
		destinationRectangle.w = nativeWidth;
		destinationRectangle.h = nativeHeight;

		loaded = true;
	}

}
void                   Gui::Texture_Manager::loadTexture(SDL_Renderer * _renderer, const std::string & _path,
	                                                     const unsigned char & _r, const unsigned char & _g, const unsigned char & _b) {

	if (!loaded) {
		SDL_Surface * tempSurface = IMG_Load(_path.c_str());

		SDL_SetColorKey(tempSurface, SDL_TRUE, SDL_MapRGB(tempSurface->format, _r, _g, _b));

		if (!tempSurface)
			throw std::runtime_error("Image loading failed!");

		nativeWidth = tempSurface->w;
		nativeHeight = tempSurface->h;
		texture = std::make_unique<SDL_Texture *>(SDL_CreateTextureFromSurface(_renderer, tempSurface));

		sourceRectangle.x = 0;
		sourceRectangle.y = 0;
		sourceRectangle.w = nativeWidth;
		sourceRectangle.h = nativeHeight;

		destinationRectangle.x = 0;
		destinationRectangle.y = 0;
		destinationRectangle.w = nativeWidth;
		destinationRectangle.h = nativeHeight;

		SDL_FreeSurface(tempSurface);

		loaded = true;
	}

}
void                   Gui::Texture_Manager::createTexture(SDL_Renderer * _renderer, TTF_Font * _font, const std::string & _text,
	                                                       const unsigned char & _r,const unsigned char & _g, const unsigned char & _b) {

	if (!loaded) {
		SDL_Surface* tempSurface = TTF_RenderText_Solid(_font, _text.c_str(), { _r, _g, _b });

		if (!tempSurface)
			throw std::runtime_error("Image loading failed!");

		nativeWidth = tempSurface->w;
		nativeHeight = tempSurface->h;
		texture = std::make_unique<SDL_Texture *>(SDL_CreateTextureFromSurface(_renderer, tempSurface));

		sourceRectangle.x = 0;
		sourceRectangle.y = 0;
		sourceRectangle.w = nativeWidth;
		sourceRectangle.h = nativeHeight;

		destinationRectangle.x = 0;
		destinationRectangle.y = 0;
		destinationRectangle.w = nativeWidth;
		destinationRectangle.h = nativeHeight;

		SDL_FreeSurface(tempSurface);

		loaded = true;
	}

}
void                   Gui::Texture_Manager::allowAlphaBlending() {
	SDL_SetTextureBlendMode(*texture, SDL_BLENDMODE_BLEND);
}
void                   Gui::Texture_Manager::render(SDL_Renderer * _renderer, const double & _angle) {
	SDL_RenderCopyEx(_renderer, *texture, &sourceRectangle, &destinationRectangle, _angle, nullptr, SDL_FLIP_NONE);
}
const signed short &   Gui::Texture_Manager::getSRX() const {
	return sourceRectangle.x;
}
void                   Gui::Texture_Manager::setSRX(const signed short & _xC) {
	sourceRectangle.x = _xC;
}
const signed short &   Gui::Texture_Manager::getSRY() const {
	return sourceRectangle.y;
}
void                   Gui::Texture_Manager::setSRY(const signed short & _yC) {
	sourceRectangle.y = _yC;
}
const unsigned short & Gui::Texture_Manager::getSRW() const {
	return sourceRectangle.w;
}
void                   Gui::Texture_Manager::setSRW(const unsigned short & _wdt) {
	sourceRectangle.w = _wdt;
}
const unsigned short & Gui::Texture_Manager::getSRH() const {
	return sourceRectangle.h;
}
void                   Gui::Texture_Manager::setSRH(const unsigned short & _hgt) {
	sourceRectangle.x = _hgt;
}
const signed short &   Gui::Texture_Manager::getDRX() const {
	return destinationRectangle.x;
}
void                   Gui::Texture_Manager::setDRX(const signed short & _xC) {
	destinationRectangle.x = _xC;
}
const signed short &   Gui::Texture_Manager::getDRY() const {
	return destinationRectangle.y;
}
void                   Gui::Texture_Manager::setDRY(const signed short & _yC) {
	destinationRectangle.y = _yC;
}
const unsigned short & Gui::Texture_Manager::getDRW() const {
	return destinationRectangle.w;
}
void                   Gui::Texture_Manager::setDRW(const unsigned short & _wdt) {
	destinationRectangle.w = _wdt;
}
const unsigned short & Gui::Texture_Manager::getDRH() const {
	return destinationRectangle.h;
}
void                   Gui::Texture_Manager::setDRH(const unsigned short & _hgt) {
	destinationRectangle.h = _hgt;
}
void                   Gui::Texture_Manager::setAlphaValue(const unsigned char & _val) {
	SDL_SetTextureAlphaMod(*texture, _val);
}
unsigned char          Gui::Texture_Manager::getAlphaValue() const {
	Uint8 val = 0;

	SDL_GetTextureAlphaMod(*texture, &val);

	return val;
}
