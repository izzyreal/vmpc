#include "Gui.hpp"

#include "mpc2.h"
#include "pad.h"

/* ctor & dtor */

Gui::Gui()
{
    components.push_back(make_shared<SvgComponent>(cairo_code_mpc2_render));
    components.push_back(make_shared<SvgComponent>(cairo_code_pad_render));
    initBackground();
}

Gui::~Gui()
{
}

/* end of ctor & dtor */


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

// First create surface
void Gui::initSDLSurface() {
    destroySDLSurface();
    sdlSurface = SDL_CreateRGBSurface(0, drawableWidth, drawableHeight, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0);
}

// Then create texture
void Gui::initSDLTexture() {
    destroySDLTexture();
    sdlTexture = SDL_CreateTextureFromSurface(sdlRenderer, sdlSurface);
}

void Gui::setWindowSize() {
    SDL_SetWindowSize(sdlWindow, initialWindowWidth * userScale, initialWindowHeight * userScale);
}

void Gui::setWindowPosition() {
    SDL_SetWindowPosition(sdlWindow, (desktopWidth - (initialWindowWidth * userScale)) / 2, (desktopHeight - (initialWindowHeight * userScale)) / 2);
}

void Gui::initCairo() {
    initCairoSurface();
    initCairoContext();
    scaleCairoContext();
}

void Gui::initCairoSurface() {
    destroyCairoSurface();
    cairoSurface = cairo_image_surface_create_for_data((unsigned char*)sdlSurface->pixels, CAIRO_FORMAT_RGB24, sdlSurface->w, sdlSurface->h, sdlSurface->pitch);
}

void Gui::initCairoContext() {
    destroyCairoContext();
    cairoContext = cairo_create(cairoSurface);
}

void Gui::scaleCairoContext() {
    cairoScale = drawableWidth / BG_WIDTH;
    cairo_scale(cairoContext, cairoScale, cairoScale);
}

void Gui::draw(bool dirtyOnly) {
    for (auto c : components) {
        if (!dirtyOnly || c->isDirty())
            c->draw(cairoContext);
    }
    
    SDL_UpdateTexture(sdlTexture, NULL, (unsigned char*)sdlSurface->pixels, sdlSurface->pitch);
    SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, NULL);
    SDL_RenderPresent(sdlRenderer);
}

void Gui::setUserScale(const float& userScale) {
    this->userScale = userScale;
    setWindowSize();
    setWindowPosition();
    refreshDrawableSize();
    initSDLSurface();
    initSDLTexture();
    initCairo();
    draw(false);
}


/* keyboard handler */

void Gui::handleKeyDown(const SDL_KeyboardEvent& event) {
    switch(event.keysym.sym ){
        case SDLK_LEFT:
            setUserScale(SMALL);
            break;
        case SDLK_UP:
            setUserScale(MEDIUM);
            break;
        case SDLK_RIGHT:
            setUserScale(LARGE);
            break;
    }
}

/* end of keyboard handler */


/* re-usable destruction methods */

void Gui::destroySDLSurface() {
    if (sdlSurface == nullptr) return;
    SDL_FreeSurface(sdlSurface);
}

void Gui::destroySDLTexture() {
    if (sdlTexture == nullptr) return;
    SDL_DestroyTexture(sdlTexture);
}

void Gui::destroyCairoSurface() {
    if (cairoSurface == nullptr) return;
    cairo_surface_destroy(cairoSurface);
}

void Gui::destroyCairoContext() {
    if (cairoContext == nullptr) return;
    cairo_destroy(cairoContext);
}

/* end of re-usable destruction methods */


/* one-time methods */

void Gui::initSDL() {
    SDL_Init(SDL_INIT_VIDEO);
    
    refreshDesktopSize();
    
    initWindow();
    sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, 0);
}

void Gui::initBackground() {
    bgRatio = BG_HEIGHT / BG_WIDTH;
}

void Gui::initWindow() {
    initialWindowWidth = BG_WIDTH;
    initialWindowHeight = initialWindowWidth * bgRatio;
    sdlWindow = SDL_CreateWindow("vMPC2000XL", 0, 0, 0, 0, SDL_WINDOW_ALLOW_HIGHDPI);
}

void Gui::destroySDL() {
    destroySDLTexture();
    destroySDLSurface();
    SDL_DestroyRenderer(sdlRenderer);
    SDL_DestroyWindow(sdlWindow);
    SDL_Quit();
}

void Gui::startLoop() {
    while (!quit)
    {
        SDL_WaitEvent(&event);
        switch (event.type)
        {
            case SDL_KEYDOWN:
                handleKeyDown(event.key);
                break;
            case SDL_QUIT:
                quit = true;
                break;
        }
    }
}
/* end of one-time methods */
