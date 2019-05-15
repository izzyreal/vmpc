#pragma once

#include "gfx/Component.hpp"

#include <vector>
#include <memory>

#include <cairo.h>
#include <SDL.h>        
#include <SDL_image.h>
#include <SDL_render.h>

#include <log4cplus/log4cplus.h>

using namespace std;

namespace mpc {
	class Mpc;
}

class KeyDownHandler;
class KeyUpHandler;
class MouseDownHandler;
class MouseUpHandler;

class Gui
{
public:
	Gui(mpc::Mpc* mpc);
	~Gui();

private:
	mpc::Mpc* mpc;
	KeyDownHandler* keyDownHandler;
	KeyUpHandler* keyUpHandler;
	MouseDownHandler* mouseDownHandler;
	MouseUpHandler* mouseUpHandler;

private:
	weak_ptr<Component> dataWheel;

private:
    SDL_Renderer* sdlRenderer;
    SDL_Window* sdlWindow;
    SDL_Surface* sdlSurface = nullptr;
    SDL_Texture* sdlTexture = nullptr;
    SDL_Event event;

private:
    cairo_surface_t* cairoSurface = nullptr;
    cairo_t* cairoContext = nullptr;

public:
    static const float SMALL, MEDIUM, LARGE;
    const float BG_WIDTH = 940.f;
    const float BG_HEIGHT = 686.f;
    
private:
    float bgRatio;
    
private:
    float userScale = LARGE;
    bool quit = false;
	int drawableWidth, drawableHeight;
    int desktopWidth, desktopHeight;
    int initialWindowWidth, initialWindowHeight;
	shared_ptr<Component> rootComponent;

private:
	void draw();

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
	const float getUserScale();
	weak_ptr<Component> getDataWheel();
	weak_ptr<Component> findTopChild(const int x, const int y);

private:
	log4cplus::Logger logger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("main"));
};

