#include "Gui.hpp"

Gui::Gui()
{
	components.push_back(make_shared<SvgComponent>());
}


Gui::~Gui()
{
}

void Gui::initSDL() {
	SDL_Init(SDL_INIT_VIDEO);
	sdlWindow = SDL_CreateWindow("vMPC2000XL",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1920, 1080, SDL_WINDOW_ALLOW_HIGHDPI);
	SDL_GL_GetDrawableSize(sdlWindow, &drawableWidth, &drawableHeight);
	printf("Drawable: %i x %i\n", drawableWidth, drawableHeight);
	sdlSurface = SDL_CreateRGBSurface(
		0, drawableWidth, drawableHeight, 32,
		0x00FF0000,
		0x0000FF00,
		0x000000FF,
		0);

	cairoSurface = cairo_image_surface_create_for_data(
		(unsigned char*)sdlSurface->pixels,
		CAIRO_FORMAT_RGB24,
		sdlSurface->w,
		sdlSurface->h,
		sdlSurface->pitch);
	cairoContext = cairo_create(cairoSurface);

	sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, 0);
	sdlTexture = SDL_CreateTextureFromSurface(sdlRenderer, sdlSurface);
	cairo_scale(cairoContext, 2.5, 2.5);
	while (!quit)
	{
		SDL_WaitEventTimeout(&event, 1);
		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		}
		draw();
	}
}

void Gui::draw() {	
	for (auto c : components) {
		c->draw(cairoContext);
	}

	SDL_UpdateTexture(sdlTexture, NULL, (unsigned char*)sdlSurface->pixels, sdlSurface->pitch);
	SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, NULL);
	SDL_RenderPresent(sdlRenderer);
}

void Gui::destroySDL() {
	SDL_DestroyTexture(sdlTexture);
	SDL_FreeSurface(sdlSurface);
	SDL_DestroyRenderer(sdlRenderer);
	SDL_DestroyWindow(sdlWindow);
	SDL_Quit();
}

void Gui::rotate() {
}
