#ifndef GUI_UTILITIES_H
#define GUI_UTILITIES_H

// Inclusions
#include "SDL.h"
#include "SDL_ttf.h"
#include <string>

namespace Gui {

	class Gui_Utilities {

	public:

		// Public constructor definition
		Gui_Utilities() = delete; // Deleted constructor to prevent instantiation of this class

		// Public static methods declarations
		static void programInitialization(); // Initializes all the required SDL elements
		static void initializeGuiElements(); // Initializes all the needed textures
		static void guiMainLoop();           // The main GUI loop that allows the program to run until a close request
		static void increaseFrameCounter();  // Updates the FRAME_COUNTER field
		static void userEventsHandler();     // The function handling the user events
		static void updateProgramStatus();   // The function handling the status of every object involved
		static void rendering();             // Where all the rendering takes place
		static void programTermination();    // Terminates the program, along with all the initialized SDL elements

		// Public static fields declarations
		static std::string        PROGRAM_NAME;         // A string representing the name of this program
		static unsigned short     MAIN_WINDOW_WIDTH;    // The current width of the main window
		static unsigned short     MAIN_WINDOW_HEIGHT;   // The current height of the main window
		static SDL_Window *       MAIN_WINDOW;          // A pointer to the main window of this program
		static SDL_Surface *      MAIN_WINDOW_SURFACE;  // A pointer to the surface of the main window
		static SDL_Renderer *     MAIN_WINDOW_RENDERER; // A pointer to the renderer of the main window
		static SDL_Event          USER_EVENT_HANDLER;   // The user event handler of this program
		static SDL_DisplayMode    DISPLAY_MODE;         // The object storing the native desktop resolution
		static TTF_Font *         MAIN_FONT;            // The main font used inside the program
		static unsigned long long FRAME_COUNTER;        // The current frame
		static bool               CLOSE_REQUEST;        // Indicates if the program must be closed

	};

}

#endif
