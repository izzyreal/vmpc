#include "Gui.hpp"

#include "kb/KeyDownHandler.hpp"
#include "kb/KeyUpHandler.hpp"
#include "mouse/MouseDownHandler.hpp"
#include "mouse/MouseUpHandler.hpp"

#include "gfx/SvgComponent.hpp"
#include "gfx/Panel.hpp"
#include "gfx/Label.hpp"
#include "gfx/Lcd.hpp"

#include <Mpc.hpp>
#include <hardware/Hardware.hpp>
#include <hardware/HwPad.hpp>
#include <hardware/Button.hpp>
#include <hardware/DataWheel.hpp>

#include "gfx/assets/bg.h"
#include "gfx/assets/pad.h"
#include "gfx/assets/wheel.h"

/* ctor & dtor */

const float Gui::SMALL = 0.75f;
const float Gui::MEDIUM = 1.0f;
const float Gui::LARGE = 1.5f;

Gui::Gui(mpc::Mpc* mpc)
{
	this->mpc = mpc;
	
	keyDownHandler = new KeyDownHandler(mpc, this);
	keyUpHandler = new KeyUpHandler(mpc);
	mouseDownHandler = new MouseDownHandler(mpc, this);
	mouseUpHandler = new MouseUpHandler(mpc, this);

	const auto width = cairo_code_bg_get_width();
	const auto height = cairo_code_bg_get_height();
    rootComponent = make_shared<SvgComponent>(MRECT(0, 0, width, height), "bg", cairo_code_bg_render);
	
	const int padX = 548;
	const int padY = 285;
	const int padSpace = 10;
	const int padWidth = cairo_code_pad_get_width();
	
	vector<string> row1{ "13", "9", "5", "1" };
	vector<string> row2{ "14", "10", "6", "2" };
	vector<string> row3{ "15", "11", "7", "3" };
	vector<string> row4{ "16", "12", "8", "4" };
	vector<vector<string>> rows{ row1, row2, row3, row4 };

	for (int col = 0; col < 4; col++) {
		for (int row = 0; row < 4; row++) {
			int x = padX + ((padWidth + padSpace) * col);
			int y = padY + ((padWidth + padSpace) * row);
			rootComponent->addChild(make_shared<SvgComponent>(MRECT(x, y, x + padWidth, y + padWidth), "pad" + rows[col][row], cairo_code_pad_render));
		}
	}

    addLcd();
    
	const int wheelX = 277;
	const int wheelY = 308;

	auto dataWheelShared = make_shared<SvgComponent>(MRECT(wheelX, wheelY, wheelX + cairo_code_wheel_get_width(), wheelY + cairo_code_wheel_get_height()), "datawheel", cairo_code_wheel_render);
	dataWheel = dataWheelShared;
	rootComponent->addChild(dataWheelShared);
}

Gui::~Gui()
{
	delete keyDownHandler;
	delete keyUpHandler;
	delete mouseDownHandler;
	delete mouseUpHandler;
}

/* end of ctor & dtor */


void Gui::addLcd() {
     const int lcdX = 140;
     const int lcdY = 67;
     const int lcdR = lcdX + 248;
     const int lcdB = lcdY + 60;
     const auto pixels = mpc->getLayeredScreen().lock()->getPixels();
     const auto lcdShared = make_shared<Lcd>(MRECT(lcdX, lcdY, lcdR, lcdB), pixels, "lcd");
     
     const MColorF lcdBgColor(1.0f, 0.8f, 0.8f, 0.9f);
     const MColorF lcdFgColor(1.0f, 0.1f, 0.2f, 0.15f);
     lcdShared->setBgColor(lcdBgColor);
     lcdShared->setFgColor(lcdFgColor);
     
     lcd = lcdShared;
     rootComponent->addChild(lcdShared);
}

void Gui::refreshDesktopSize() {
    for (int i = 0; i < SDL_GetNumVideoDisplays(); i++) {
        SDL_DisplayMode m;
        SDL_GetCurrentDisplayMode(i, &m);
        desktopWidth = m.w;
        desktopHeight = m.h;
        // We're only going for the main display for now
        break;
    }
}

void Gui::refreshDrawableSize() {
    SDL_GL_GetDrawableSize(sdlWindow, &drawableWidth, &drawableHeight);
    LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Drawable size of SDL window: ") << drawableWidth << ", " << drawableHeight);
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
    const auto cairoScale = drawableWidth / BG_WIDTH;
    cairo_scale(cairoContext, cairoScale, cairoScale);
}

void Gui::draw() {
    
    if (mpc != nullptr) {
        if (mpc->getLayeredScreen().lock()->IsDirty()) {
            mpc->getLayeredScreen().lock()->Draw();
            lcd.lock()->setDirty(true);
        }
    }
    
    vector<MRECT*> updatedRects;
    
	rootComponent->draw(cairoContext, true, updatedRects);
    
    if (updatedRects.size() > 0) {
        SDL_UpdateTexture(sdlTexture, NULL, (unsigned char*)sdlSurface->pixels, sdlSurface->pitch);
        SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, NULL);
        SDL_RenderPresent(sdlRenderer);
    }
}

void Gui::setUserScale(const float& userScale) {
    this->userScale = userScale;
    setWindowSize();
    setWindowPosition();
    refreshDrawableSize();
    initSDLSurface();
    initSDLTexture();
    initCairo();
}

const float Gui::getUserScale() {
	return userScale;
}

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

void Gui::initWindow() {
    initialWindowWidth = BG_WIDTH;
    initialWindowHeight = initialWindowWidth * (BG_HEIGHT / BG_WIDTH);
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
        SDL_WaitEventTimeout(&event, 20);
     
        switch (event.type)
        {
			case SDL_MOUSEBUTTONDOWN:
				mouseDownHandler->handle(event.button);
				break;
			case SDL_MOUSEBUTTONUP:
				mouseUpHandler->handle(event.button);
				break;
            case SDL_KEYDOWN:
                keyDownHandler->handle(event.key);
                break;
            case SDL_KEYUP:
                keyUpHandler->handle(event.key);
                break;
            case SDL_QUIT:
                quit = true;
                break;
        }
        draw();
		event = SDL_Event();

    }
}

/* end of one-time methods */

weak_ptr<Component> Gui::getDataWheel() {
	return dataWheel;
}

weak_ptr<Component> Gui::findTopChild(const int x, const int y) {
	auto c = rootComponent->findTopChild(x, y).lock();
	if (!c) c = rootComponent;
	return c;
}
