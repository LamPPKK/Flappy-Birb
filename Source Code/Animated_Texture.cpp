
// Inclusions
#include "Animated_Texture.h"
#include "SDL.h"
#include "Texture_Manager.h"
#include <cstdlib>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

// Public constructors definitions
Gui::Animated_Texture::Animated_Texture(SDL_Renderer * _renderer, const std::initializer_list<std::string> & _paths) : sprites(new Texture_Manager[_paths.size()]),
                                                                                                                       numberOfFrames(_paths.size()) {
	std::initializer_list<std::string>::iterator begin = _paths.begin(), end = _paths.end();
	std::size_t index = 0;

	while (begin != end) {
		sprites[index].loadTexture(_renderer, *begin);
		++begin;
		++index;
	}

}
Gui::Animated_Texture::Animated_Texture(SDL_Renderer * _renderer, const std::initializer_list<std::string> & _paths,
	                                    const unsigned char & _r, const unsigned char & _g, const unsigned char & _b) : sprites(new Texture_Manager[_paths.size()]),
	                                                                                                                    numberOfFrames(_paths.size()) {

	std::initializer_list<std::string>::iterator begin = _paths.begin(), end = _paths.end();
	std::size_t index = 0;

	while (begin != end) {
		sprites[index].loadTexture(_renderer, *begin, _r, _g, _b);
		++begin;
		++index;
	}

}

// Public destructor definition
Gui::Animated_Texture::~Animated_Texture() {

}

// Private methods definitions
void                   Gui::Animated_Texture::render(SDL_Renderer * _renderer, const std::size_t & _index, const double & _angle) {
	sprites[_index].render(_renderer, _angle);
}
const std::size_t &    Gui::Animated_Texture::size() const {
	return numberOfFrames;
}
const signed short &   Gui::Animated_Texture::getSRX(const std::size_t & _index) const {
	return sprites[_index].getSRX();
}
void                   Gui::Animated_Texture::setSRX(const std::size_t & _index, const signed short & _xC) {
	sprites[_index].setSRX(_xC);
}
const signed short &   Gui::Animated_Texture::getSRY(const std::size_t & _index) const {
	return sprites[_index].getSRY();
}
void                   Gui::Animated_Texture::setSRY(const std::size_t & _index, const signed short & _yC) {
	sprites[_index].setSRY(_yC);
}
const unsigned short & Gui::Animated_Texture::getSRW(const std::size_t & _index) const {
	return sprites[_index].getSRW();
}
void                   Gui::Animated_Texture::setSRW(const std::size_t & _index, const unsigned short & _wdt) {
	sprites[_index].setSRW(_wdt);
}
const unsigned short & Gui::Animated_Texture::getSRH(const std::size_t & _index) const {
	return sprites[_index].getSRH();
}
void                   Gui::Animated_Texture::setSRH(const std::size_t & _index, const unsigned short & _hgt) {
	sprites[_index].setSRH(_hgt);
}
const signed short &   Gui::Animated_Texture::getDRX(const std::size_t & _index) const {
	return sprites[_index].getDRX();
}
void                   Gui::Animated_Texture::setDRX(const std::size_t & _index, const signed short & _xC) {
	sprites[_index].setDRX(_xC);
}
const signed short &   Gui::Animated_Texture::getDRY(const std::size_t & _index) const {
	return sprites[_index].getDRY();
}
void                   Gui::Animated_Texture::setDRY(const std::size_t & _index, const signed short & _yC) {
	sprites[_index].setDRY(_yC);
}
const unsigned short & Gui::Animated_Texture::getDRW(const std::size_t & _index) const {
	return sprites[_index].getDRW();
}
void                   Gui::Animated_Texture::setDRW(const std::size_t & _index, const unsigned short & _wdt) {
	sprites[_index].setDRW(_wdt);
}
const unsigned short & Gui::Animated_Texture::getDRH(const std::size_t & _index) const {
	return sprites[_index].getDRH();
}
void                   Gui::Animated_Texture::setDRH(const std::size_t & _index, const unsigned short & _hgt) {
	sprites[_index].setDRH(_hgt);
}
