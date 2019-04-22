#pragma once

#include "gfx/SvgComponent.hpp"
#include "gfx/Group.hpp"

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
    SDL_Renderer* sdlRenderer;
    SDL_Window* sdlWindow;
    SDL_Surface* sdlSurface = nullptr;
    SDL_Texture* sdlTexture = nullptr;
    SDL_Event event;

private:
    cairo_surface_t* cairoSurface = nullptr;
    cairo_t* cairoContext = nullptr;

private:
    const float SMALL = 0.75f, MEDIUM = 1.0f, LARGE = 1.5f;
    const float BG_WIDTH = 940.f;
    const float BG_HEIGHT = 686.f;
    
private:
    float bgRatio;
    
private:
    float userScale = MEDIUM;
    bool quit = false;
	int drawableWidth, drawableHeight;
    int desktopWidth, desktopHeight;
    int initialWindowWidth, initialWindowHeight;
    float cairoScale;
    vector<shared_ptr<Component>> components;

private:
	void draw(bool dirtyOnly);

private:
    void initSDLSurface();
    void initSDLTexture();
    void initCairoSurface();
    void initCairoContext();
    void scaleCairoContext();
    void initWindow();
    void initBackground();
    void refreshDesktopSize();
    void refreshDrawableSize();
    void setWindowSize();
    void setWindowPosition();
    
private:
    void destroyCairoContext();
    void destroySDLSurface();
    void destroySDLTexture();
    void destroyCairoSurface();
    
public:
	void initSDL();
    void initCairo();
    void startLoop();
	void destroySDL();

public:
    void setUserScale(const float&);
    
private:
    void handleKeyDown(const SDL_KeyboardEvent&);
	void handleMouseDown(const SDL_MouseButtonEvent&);
	weak_ptr<Component> findUpperContains(const vector<weak_ptr<Component>>& components, const int x, const int y);

};

