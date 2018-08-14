#ifndef ANIMATED_TEXTURE_H
#define ANIMATED_TEXTURE_H

// Inclusions
#include "SDL.h"
#include <cstdlib>
#include <initializer_list>
#include <memory>
#include <string>

namespace Gui {

	// Forward declarations
	class Texture_Manager;

	class Animated_Texture final {

	private:

		// Friend declarations
		friend class Birb;
		friend class Gui_Utilities;
		friend class Match_Manager;

	public:

		// Public constructors declarations
		Animated_Texture(SDL_Renderer * _renderer, const std::initializer_list<std::string> & _paths); // Default constructor
		Animated_Texture(SDL_Renderer * _renderer, const std::initializer_list<std::string> & _paths,
			                                       const unsigned char & _r,
			                                       const unsigned char & _g,
			                                       const unsigned char & _b);                          // Color keying constructor

		// Public destructor declaration
		~Animated_Texture(); // The destructor of this class

	private:

		// Private methods declarations
		void render(SDL_Renderer * _renderer, const std::size_t & _index, 
			                                  const double & _angle = 0.0);                     // Renders the _index frame of this animation
		const std::size_t &    size() const;                                                    // Returns the number fo frames of this animation
		const signed short &   getSRX(const std::size_t & _index) const;                        // Returns a const reference to the source rectangle x coordinate of the _index frame of this animation
		void                   setSRX(const std::size_t & _index, const signed short & _xC);    // Sets a new x coordinate for the source rectangle of the _index frame of this animation
		const signed short &   getSRY(const std::size_t & _index) const;                        // Returns a const reference to the source rectangle y coordinate of the _index frame of this animation
		void                   setSRY(const std::size_t & _index, const signed short & _yC);    // Sets a new y coordinate for the source rectangle of the _index frame of this animation
		const unsigned short & getSRW(const std::size_t & _index) const;                        // Returns a const reference to the source rectangle width of the _index frame of this animation
		void                   setSRW(const std::size_t & _index, const unsigned short & _wdt); // Sets a new width for the source rectangle of the _index frame of this animation
		const unsigned short & getSRH(const std::size_t & _index) const;                        // Returns a const reference the source rectangle height of the _index frame of this animation
		void                   setSRH(const std::size_t & _index, const unsigned short & _hgt); // Sets a new height for the source rectangle of the _index frame of this animation
		const signed short &   getDRX(const std::size_t & _index) const;                        // Returns a const reference to the destination rectangle x coordinate of the _index frame of this animation
		void                   setDRX(const std::size_t & _index, const signed short & _xC);    // Sets a new x coordinate for the destination rectangle of the _index frame of this animation
		const signed short &   getDRY(const std::size_t & _index) const;                        // Returns a const reference to the destination rectangle y coordinate of the _index frame of this animation
		void                   setDRY(const std::size_t & _index, const signed short & _yC);    // Sets a new y coordinate for the destination rectangle of the _index frame of this animation
		const unsigned short & getDRW(const std::size_t & _index) const;                        // Returns a const reference to the destination rectangle width of the _index frame of this animation
		void                   setDRW(const std::size_t & _index, const unsigned short & _wdt); // Sets a new width for the destination rectangle of the _index frame of this animation
		const unsigned short & getDRH(const std::size_t & _index) const;                        // Returns a const reference to the destination rectangle height of the _index frame of this animation
		void                   setDRH(const std::size_t & _index, const unsigned short & _hgt); // Sets a new height for the destination rectangle of the _index frame of this animation

		// Private fields
		std::unique_ptr<Texture_Manager[]> sprites;        // This object stores all the sprites that form the complete animation
		std::size_t                        numberOfFrames; // The number of frames of this animation

	};

}

#endif
