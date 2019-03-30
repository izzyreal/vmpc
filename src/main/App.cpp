#include <stdlib.h>
#include <cairo.h>


#include <SDL.h>        
#include <SDL_image.h>
#include <SDL_render.h>

#include "StartUp.hpp"

#include "file/FileUtil.hpp"

using namespace moduru;


#include "mpc.c"

int main(int argc, char *argv[]) {

	const auto sep = file::FileUtil::getSeparator();
	const auto resPath = mpc::StartUp::resPath;
	const auto imgPath = resPath + sep + "img" + sep;
	const auto bgPath = imgPath + "mpc-px.svg";

	int mpc_width = cairo_code_mpc_get_width();
	int mpc_height = cairo_code_mpc_get_height();

	int width = mpc_width * 2;
	int height = mpc_width * 2;

	SDL_Surface *sdlsurf = SDL_CreateRGBSurface(
		0, width, height, 32,
		0x00FF0000, /* Rmask */
		0x0000FF00, /* Gmask */
		0x000000FF, /* Bmask */
		0); /* Amask */

	cairo_surface_t *surface = cairo_image_surface_create_for_data(
		(unsigned char*) sdlsurf->pixels,
		CAIRO_FORMAT_RGB24,
		sdlsurf->w,
		sdlsurf->h,
		sdlsurf->pitch);

	cairo_t *cr = cairo_create(surface);
	cairo_scale(cr, 5, 5);

	bool quit = false;
	SDL_Event event;

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window * window = SDL_CreateWindow("vMPC2000XL",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);

	SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
	double angle = 0.01;
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, sdlsurf);
    
    cairo_translate(cr, -mpc_width / 2, -mpc_height / 2);
	while (!quit)
	{
		SDL_WaitEventTimeout(&event, 1);
		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		}
		cairo_set_source_rgb(cr, 255, 255, 255);
		cairo_paint(cr);
		//cairo_rotate(cr, angle);
		//cairo_translate(cr, -mpc_width / 2, -mpc_height / 2);
		cairo_code_mpc_render(cr);
        SDL_Rect r{0, 0, 500, 500};
        SDL_UpdateTexture(texture, &r, (unsigned char*) sdlsurf->pixels, sdlsurf->pitch);
		SDL_RenderCopy(renderer, texture, &r, &r);
		SDL_RenderPresent(renderer);
	}

    SDL_DestroyTexture(texture);
	SDL_FreeSurface(sdlsurf);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
