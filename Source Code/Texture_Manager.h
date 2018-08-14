#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

// Inclusions
#include "SDL.h"
#include "SDL_ttf.h"
#include <string>
#include <memory>

namespace Gui {

	class Texture_Manager final {

	private:

		// Friend declarations
		friend class Animated_Texture;
		friend class Final_Stats;
		friend class Gui_Utilities;
		friend class Match_Manager;
		friend class Pipe_Down;
		friend class Pipe_Up;
		friend class Pipes_Manager;
		friend class Score_Manager;

	public:

		// Public constructors declarations
		Texture_Manager();                                        // Default constructor
		Texture_Manager(const Texture_Manager & _other) = delete; // Deleted copy constructor
		Texture_Manager(Texture_Manager && _other) noexcept;      // Move constructor

		// Public destructor declaration
		~Texture_Manager(); // The destructor of this class

		// Public operators declarations
		Texture_Manager & operator=(const Texture_Manager & _other) = delete;     // Deleted Copy-assignment operator
		Texture_Manager & operator=(Texture_Manager && _other) noexcept = delete; // Deleted Move-Assignment operator

	private:

		// Private methods declarations
		void                   loadTexture(SDL_Renderer * _renderer,
			                               const std::string & _path);    // Loads a texture (Can be called only once, subsequent calls will be ignored)
		void                   loadTexture(SDL_Renderer * _renderer, 
			                               SDL_Surface * _surface);       // Creates a texture from the input surface (Can be called only once, subsequent calls will be ignored)
		void                   loadTexture(SDL_Renderer * _renderer,
			                               const std::string & _path,
			                               const unsigned char & _r,
			                               const unsigned char & _g,
			                               const unsigned char & _b);     // Loads a texture, color keying the input RGB color (Can be called only once, subsequent calls will be ignored)
		void                   createTexture(SDL_Renderer * _renderer,
			                                 TTF_Font * _font, 
			                                 const std::string & _text,
			                                 const unsigned char & _r,
			                                 const unsigned char & _g,
			                                 const unsigned char & _b);   // Creates a texture using a TTF_Font (Can be called only once, subsequent calls will be ignored)
		void                   allowAlphaBlending();                      // Allows transparency for this Texture
		void                   render(SDL_Renderer * _renderer, 
			                          const double & _angle = 0.0);       // Renders the texture
		const signed short &   getSRX() const;                            // Returns a const reference to the source rectangle x coordinate
		void                   setSRX(const signed short & _xC);          // Sets a new x coordinate for the source rectangle
		const signed short &   getSRY() const;                            // Returns a const reference to the source rectangle y coordinate
		void                   setSRY(const signed short & _yC);          // Sets a new y coordinate for the source rectangle 
		const unsigned short & getSRW() const;                            // Returns a const reference to the source rectangle width
		void                   setSRW(const unsigned short & _wdt);       // Sets a new width for the source rectangle
		const unsigned short & getSRH() const;                            // Returns a const reference the source rectangle height
		void                   setSRH(const unsigned short & _hgt);       // Sets a new height for the source rectangle
		const signed short &   getDRX() const;                            // Returns a const reference to the destination rectangle x coordinate
		void                   setDRX(const signed short & _xC);          // Sets a new x coordinate for the destination rectangle
		const signed short &   getDRY() const;                            // Returns a const reference to the destination rectangle y coordinate 
		void                   setDRY(const signed short & _yC);          // Sets a new y coordinate for the destination rectangle
		const unsigned short & getDRW() const;                            // Returns a const reference to the destination rectangle width 
		void                   setDRW(const unsigned short & _wdt);       // Sets a new width for the destination rectangle
		const unsigned short & getDRH() const;                            // Returns a const reference to the destination rectangle height
		void                   setDRH(const unsigned short & _hgt);       // Sets a new height for the destination rectangle
		void                   setAlphaValue(const unsigned char & _val); // Sets a new value for the alpha channel value of this Texture
		unsigned char          getAlphaValue() const;                     // Returns the alpha channel value of this Texture

		// Private fields
		std::unique_ptr<SDL_Texture *> texture;              // The texture managed by this object
		unsigned short                 nativeWidth;          // The native width of the loaded image
		unsigned short                 nativeHeight;         // The native height of the loaded image
		SDL_Rect                       sourceRectangle;      // The object indicating the portion of the texture needed
		SDL_Rect                       destinationRectangle; // The object indicating the destination rectangle of the texture portion
		bool                           loaded;               // Indicates if the object is already managing a texture

	};

}

#endif
