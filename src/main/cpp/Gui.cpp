#include "Gui.hpp"

Gui::Gui()
{
	components.push_back(make_shared<SvgComponent>());
}


Gui::~Gui()
{
}

void Gui::refreshDesktopDimensions() {
    for (int i = 0; i < SDL_GetNumVideoDisplays(); i++) {
        SDL_DisplayMode m;
        SDL_GetCurrentDisplayMode(i, &m);
        auto h = m.h;
        auto w = m.w;
        printf("Resolution of display %i: %i x %i\n", i, w, h);
        float ddpi, hdpi, vdpi;
        SDL_GetDisplayDPI(i, &ddpi, &hdpi, &vdpi);
        printf("Diagonal   DPI: %.6f\n", ddpi);
        printf("Horizontal DPI: %.6f\n", hdpi);
        printf("Vertical   DPI: %.6f\n", vdpi);
        printf("\n");
        desktopWidth = w;
        desktopHeight = h;
        
        // We're only going for the main display for now
        break;
    }
}

void Gui::initSDL() {
	SDL_Init(SDL_INIT_VIDEO);
    
    refreshDesktopDimensions();
    
    const float bgWidth = 940.f;
    const float bgHeight = 686.f;
    
    const auto ratio = bgHeight / bgWidth;
    
    int initialWindowWidth = bgWidth;
    int initialWindowHeight = initialWindowWidth * ratio;
    
    sdlWindow = SDL_CreateWindow("vMPC2000XL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, initialWindowWidth, initialWindowHeight, SDL_WINDOW_ALLOW_HIGHDPI);
	SDL_GL_GetDrawableSize(sdlWindow, &drawableWidth, &drawableHeight);
	printf("Drawable: %i x %i\n", drawableWidth, drawableHeight);
	sdlSurface = SDL_CreateRGBSurface( 0, drawableWidth, drawableHeight, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0);

	cairoSurface = cairo_image_surface_create_for_data((unsigned char*)sdlSurface->pixels, CAIRO_FORMAT_RGB24, sdlSurface->w, sdlSurface->h, sdlSurface->pitch);
	
    cairoContext = cairo_create(cairoSurface);

    sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, 0);
    sdlTexture = SDL_CreateTextureFromSurface(sdlRenderer, sdlSurface);
    
    float scale = drawableWidth / bgWidth;

    cairo_scale(cairoContext, scale, scale);
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
