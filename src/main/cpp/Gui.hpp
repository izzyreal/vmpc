#pragma once

#include "gfx/SvgComponent.hpp"

#include <vector>
#include <memory>

#include <cairo.h>
#include <SDL.h>        
#include <SDL_image.h>
#include <SDL_render.h>

using namespace std;

class Gui
{
public:
	Gui();
	~Gui();

private:
    SDL_Surface* sdlSurface;
    SDL_Renderer* sdlRenderer;
    SDL_Texture* sdlTexture;
    SDL_Window* sdlWindow;
    SDL_Event event;

private:
    cairo_surface_t* cairoSurface;
    cairo_t* cairoContext;

private:
    const float SMALL = 0.75f, MEDIUM = 1.0f, LARGE = 1.25f;
    const float BG_WIDTH = 940.f;
    const float BG_HEIGHT = 686.f;
    
private:
    float bgRatio;
    
private:
    float userSize = MEDIUM;
    bool quit = false;
	int drawableWidth, drawableHeight;
    int desktopWidth, desktopHeight;
    int initialWindowWidth, initialWindowHeight;
    float cairoScale;
    vector<shared_ptr<Component>> components;

private:
	void draw();

public:
	void initSDL();
    void initWindow();
    void initBackground();
    void initCairo();
	void destroySDL();
    void refreshDesktopSize();
    void refreshDrawableSize();
    void setWindowSize();
    void setWindowPosition();
};

