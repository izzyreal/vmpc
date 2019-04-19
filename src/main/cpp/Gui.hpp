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
    bool quit = false;
	int drawableWidth, drawableHeight;
    int desktopWidth, desktopHeight;
//    float scale = 0.5f;
    vector<shared_ptr<Component>> components;

private:
	void draw();

public:
	void initSDL();
	void destroySDL();
    void refreshDesktopDimensions();
	void rotate();
};

