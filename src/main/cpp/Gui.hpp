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
	bool quit = false;
	SDL_Event event;
	int drawableWidth, drawableHeight;
	SDL_Surface* sdlSurface;
	cairo_surface_t* cairoSurface;
	SDL_Renderer* sdlRenderer;
	SDL_Texture* sdlTexture;
	SDL_Window* sdlWindow;
	vector<shared_ptr<Component>> components;
	cairo_t* cairoContext;

private:
	void draw();

public:
	void initSDL();
	void destroySDL();
	void rotate();
};

