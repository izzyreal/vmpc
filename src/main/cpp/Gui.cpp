#include "Gui.hpp"

#include "gfx/SvgComponent.hpp"
#include "gfx/Panel.hpp"
#include "gfx/Label.hpp"
#include "gfx/Lcd.hpp"

#include <Mpc.hpp>
#include <hardware/Hardware.hpp>
#include <hardware/HwPad.hpp>
#include <hardware/Button.hpp>
#include <hardware/DataWheel.hpp>

#include "mpc2.h"
#include "pad.h"

/* ctor & dtor */

Gui::Gui(mpc::Mpc* mpc)
{
	this->mpc = mpc;
	const auto width = cairo_code_mpc2_get_width();
	const auto height = cairo_code_mpc2_get_height();
    rootComponent = make_shared<SvgComponent>(MRECT(0, 0, width, height), "bg", cairo_code_mpc2_render);
	
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

	const int panelWidth = 400;
	const int panelHeight = 400;

	const int panelX = (width - panelWidth) / 2;
	const int panelY = (height - panelHeight) / 2;

	const int lcdX = 140;
	const int lcdY = 67;
	const int lcdR = lcdX + 248;
	const int lcdB = lcdY + 60;
	const auto pixels = mpc->getLayeredScreen().lock()->getPixels();
	const auto lcd = make_shared<Lcd>(MRECT(lcdX, lcdY, lcdR, lcdB), pixels, "lcd");

	const MColorF lcdBgColor(1.0f, 0.8f, 0.8f, 0.9f);
	const MColorF lcdFgColor(1.0f, 0.1f, 0.2f, 0.15f);
	lcd->setBgColor(lcdBgColor);
	lcd->setFgColor(lcdFgColor);

	rootComponent->addChild(lcd);
	initBackground();

	mpc->getLayeredScreen().lock()->Draw();
}

Gui::~Gui()
{
}

/* end of ctor & dtor */


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
    cairoScale = drawableWidth / BG_WIDTH;
    cairo_scale(cairoContext, cairoScale, cairoScale);
}

void Gui::draw() {
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
	rootComponent->draw(cairoContext, false);
	draw();
}


/* keyboard handler */

void Gui::handleKeyDown(const SDL_KeyboardEvent& event) {
	auto hw = mpc->getHardware().lock();
	switch (event.keysym.sym) {
	case SDLK_F1:
		hw->getButton("f1").lock()->push();
		break;
	case SDLK_F2:
		hw->getButton("f2").lock()->push();
		break;
	case SDLK_F3:
		hw->getButton("f3").lock()->push();
		break;
	case SDLK_F4:
		hw->getButton("f4").lock()->push();
		break;
	case SDLK_F5:
		hw->getButton("f5").lock()->push();
		break;
	case SDLK_F6:
		hw->getButton("f6").lock()->push();
		break;
	case SDLK_ESCAPE:
		hw->getButton("mainscreen").lock()->push();
		break;
	case SDLK_i:
		hw->getButton("openwindow").lock()->push();
		break;
	case SDLK_LSHIFT:
		hw->getButton("shift").lock()->push();
		break;
	case SDLK_EQUALS:
		if (event.keysym.mod == KMOD_LCTRL) {
			if (userScale == SMALL) {
				setUserScale(MEDIUM);
			}
			else if (userScale == MEDIUM) {
				setUserScale(LARGE);
			}
		}
		else {
			if (event.keysym.mod == KMOD_LSHIFT) {
				hw->getDataWheel().lock()->turn(10);
			}
			else {
				hw->getDataWheel().lock()->turn(1);
			}
		}
		break;
	case SDLK_MINUS:
		if (event.keysym.mod == KMOD_LCTRL) {
			if (userScale == LARGE) {
				setUserScale(MEDIUM);
			}
			else if (userScale == MEDIUM) {
				setUserScale(SMALL);
			}
		}
		else {
			if (event.keysym.mod == KMOD_LSHIFT) {
				hw->getDataWheel().lock()->turn(-10);
			}
			else {
				hw->getDataWheel().lock()->turn(-1);
			}
		}
		break;
	case SDLK_LEFT:
		hw->getButton("left").lock()->push();
		break;
	case SDLK_UP:
		hw->getButton("up").lock()->push();
		break;
	case SDLK_RIGHT:
		hw->getButton("right").lock()->push();
		break;
	case SDLK_DOWN:
		hw->getButton("down").lock()->push();
		break;
	case SDLK_z:
		hw->getPad(0).lock()->push(127);
		break;
	case SDLK_x:
		hw->getPad(1).lock()->push(127);
		break;
	case SDLK_c:
		hw->getPad(2).lock()->push(127);
		break;
	}
	mpc->getLayeredScreen().lock()->Draw();
	rootComponent->draw(cairoContext, false);
	draw();
}

void Gui::handleKeyUp(const SDL_KeyboardEvent& event) {
	auto hw = mpc->getHardware().lock();
	switch (event.keysym.sym) {
	case SDLK_F1:
		hw->getButton("f1").lock()->release();
		break;
	case SDLK_F2:
		hw->getButton("f2").lock()->release();
		break;
	case SDLK_F3:
		hw->getButton("f3").lock()->release();
		break;
	case SDLK_F4:
		hw->getButton("f4").lock()->release();
		break;
	case SDLK_F5:
		hw->getButton("f5").lock()->release();
		break;
	case SDLK_F6:
		hw->getButton("f6").lock()->release();
		break;
	case SDLK_i:
		hw->getButton("openwindow").lock()->release();
		break;
	case SDLK_LSHIFT:
		hw->getButton("shift").lock()->release();
		break;
	case SDLK_ESCAPE:
		hw->getButton("mainscreen").lock()->release();
		break;
	case SDLK_LEFT:
		hw->getButton("left").lock()->release();
		break;
	case SDLK_UP:
		hw->getButton("up").lock()->release();
		break;
	case SDLK_RIGHT:
		hw->getButton("right").lock()->release();
		break;
	case SDLK_DOWN:
		hw->getButton("down").lock()->release();
		break;
	case SDLK_z:
		hw->getPad(0).lock()->release();
		break;
	case SDLK_x:
		hw->getPad(1).lock()->release();
		break;
	case SDLK_c:
		hw->getPad(2).lock()->release();
		break;
	}
	mpc->getLayeredScreen().lock()->Draw();
	rootComponent->draw(cairoContext, false);
	draw();
}

/* end of keyboard handler */

/* mouse handler */

void Gui::handleMouseDown(const SDL_MouseButtonEvent& event) {
	const auto x = event.x / cairoScale;
	const auto y = event.y / cairoScale;
	//printf("%f, %f\n", x, y);
	auto c = rootComponent->findTopChild(x, y).lock();
	if (!c) c = rootComponent;
	printf("Mouse down: %s\n", c->getName().c_str());
}

/* end of mouse handler */

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
			case SDL_MOUSEBUTTONDOWN:
				handleMouseDown(event.button);
				break;
            case SDL_KEYDOWN:
                handleKeyDown(event.key);
                break;
            case SDL_KEYUP:
                handleKeyUp(event.key);
                break;
            case SDL_QUIT:
                quit = true;
                break;
        }
    }
}
/* end of one-time methods */
