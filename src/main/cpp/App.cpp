#include <stdlib.h>

#include <Gui.hpp>

#include <cairo.h>


#include <SDL.h>        
#include <SDL_image.h>
#include <SDL_render.h>

#include "StartUp.hpp"

#include "file/FileUtil.hpp"

using namespace moduru;

int main(int argc, char *argv[]) {

	Gui gui;
	gui.initSDL();
	gui.destroySDL();
	return 0;
}
