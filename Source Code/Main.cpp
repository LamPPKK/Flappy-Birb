
// Inclusions
#include "Gui_Utilities.h"
#include <cstdlib>
#include <iostream>
#include "SDL.h"
#include "SDL_image.h"

int WinMain(int argc, char * args[]) {
	Gui::Gui_Utilities::programInitialization();
	Gui::Gui_Utilities::initializeGuiElements();
	Gui::Gui_Utilities::guiMainLoop();
	Gui::Gui_Utilities::programTermination();

	return EXIT_SUCCESS;
}
