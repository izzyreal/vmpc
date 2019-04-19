#include "Gui.hpp"

Gui::Gui()
{
    components.push_back(make_shared<SvgComponent>());
    initBackground();
}


Gui::~Gui()
{
}

void Gui::refreshDesktopSize() {
    for (int i = 0; i < SDL_GetNumVideoDisplays(); i++) {
        SDL_DisplayMode m;
        SDL_GetCurrentDisplayMode(i, &m);
        //       printf("Resolution of display %i: %i x %i\n", i, m.w, m.h);
        desktopWidth = m.w;
        desktopHeight = m.h;
        // We're only going for the main display for now
        break;
    }
}

void Gui::refreshDrawableSize() {
    SDL_GL_GetDrawableSize(sdlWindow, &drawableWidth, &drawableHeight);
    printf("Drawable: %i x %i\n", drawableWidth, drawableHeight);
}

void Gui::initSDL() {
    SDL_Init(SDL_INIT_VIDEO);
    
    refreshDesktopSize();
    
    initWindow();
    
    sdlSurface = SDL_CreateRGBSurface( 0, drawableWidth, drawableHeight, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0);
    
    sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, 0);
    sdlTexture = SDL_CreateTextureFromSurface(sdlRenderer, sdlSurface);
    

    initCairo();
    
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

void Gui::initBackground() {
    bgRatio = BG_HEIGHT / BG_WIDTH;
}

void Gui::initWindow() {
    initialWindowWidth = BG_WIDTH;
    initialWindowHeight = initialWindowWidth * bgRatio;
    
    sdlWindow = SDL_CreateWindow("vMPC2000XL", 0, 0, 0, 0, SDL_WINDOW_ALLOW_HIGHDPI);
    
    setWindowSize();
    setWindowPosition();
    refreshDrawableSize();
}

void Gui::setWindowSize() {
    SDL_SetWindowSize(sdlWindow, initialWindowWidth, initialWindowHeight);
}

void Gui::setWindowPosition() {
    SDL_SetWindowPosition(sdlWindow, (desktopWidth - initialWindowWidth) / 2, (desktopHeight - initialWindowHeight) / 2);
}

void Gui::initCairo() {
    cairoSurface = cairo_image_surface_create_for_data((unsigned char*)sdlSurface->pixels, CAIRO_FORMAT_RGB24, sdlSurface->w, sdlSurface->h, sdlSurface->pitch);
    
    cairoContext = cairo_create(cairoSurface);
    
    cairoScale = drawableWidth / BG_WIDTH;
    cairo_scale(cairoContext, cairoScale, cairoScale);
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
