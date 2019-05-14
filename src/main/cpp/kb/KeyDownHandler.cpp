#include "KeyDownHandler.hpp"

#include <hardware/Hardware.hpp>
#include <hardware/HwPad.hpp>
#include <hardware/Button.hpp>
#include <hardware/DataWheel.hpp>

#include <gfx/Component.hpp>

KeyDownHandler::KeyDownHandler(mpc::Mpc* _mpc, Gui* _gui) : mpc(_mpc), gui(_gui)
{
}

KeyDownHandler::~KeyDownHandler()
{
}

void KeyDownHandler::handle(const SDL_KeyboardEvent& event) {
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
	case SDLK_1:
		hw->getButton("1").lock()->push();
		break;
	case SDLK_2:
		hw->getButton("2").lock()->push();
		break;
	case SDLK_3:
		hw->getButton("3").lock()->push();
		break;
	case SDLK_4:
		hw->getButton("4").lock()->push();
		break;
	case SDLK_5:
		hw->getButton("5").lock()->push();
		break;
	case SDLK_6:
		hw->getButton("6").lock()->push();
		break;
	case SDLK_7:
		hw->getButton("7").lock()->push();
		break;
	case SDLK_8:
		hw->getButton("8").lock()->push();
		break;
	case SDLK_9:
		hw->getButton("9").lock()->push();
		break;
	case SDLK_0:
		hw->getButton("0").lock()->push();
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
			if (gui->getUserScale() == Gui::SMALL) {
				gui->setUserScale(Gui::MEDIUM);
			}
			else if (gui->getUserScale() == Gui::MEDIUM) {
				gui->setUserScale(Gui::LARGE);
			}
		}
		else {
			if (event.keysym.mod == KMOD_LSHIFT) {
				hw->getDataWheel().lock()->turn(10);
				gui->getDataWheel().lock()->rotate(0.1f);
			}
			else {
				hw->getDataWheel().lock()->turn(1);
				gui->getDataWheel().lock()->rotate(0.05f);
			}
		}
		break;
	case SDLK_MINUS:
		if (event.keysym.mod == KMOD_LCTRL) {
			if (gui->getUserScale() == Gui::LARGE) {
				gui->setUserScale(Gui::MEDIUM);
			}
			else if (gui->getUserScale() == Gui::MEDIUM) {
				gui->setUserScale(Gui::SMALL);
			}
		}
		else {
			if (event.keysym.mod == KMOD_LSHIFT) {
				hw->getDataWheel().lock()->turn(-10);
				gui->getDataWheel().lock()->rotate(-0.1f);
			}
			else {
				hw->getDataWheel().lock()->turn(-1);
				gui->getDataWheel().lock()->rotate(-0.05f);
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
	case SDLK_v:
		hw->getPad(3).lock()->push(127);
		break;
	case SDLK_a:
		hw->getPad(4).lock()->push(127);
		break;
	case SDLK_s:
		hw->getPad(5).lock()->push(127);
		break;
	case SDLK_d:
		hw->getPad(6).lock()->push(127);
		break;
	case SDLK_f:
		hw->getPad(7).lock()->push(127);
		break;
	case SDLK_b:
		hw->getPad(8).lock()->push(127);
		break;
	case SDLK_n:
		hw->getPad(9).lock()->push(127);
		break;
	case SDLK_m:
		hw->getPad(10).lock()->push(127);
		break;
	case SDLK_COMMA:
		hw->getPad(11).lock()->push(127);
		break;
	case SDLK_g:
		hw->getPad(12).lock()->push(127);
		break;
	case SDLK_h:
		hw->getPad(13).lock()->push(127);
		break;
	case SDLK_j:
		hw->getPad(14).lock()->push(127);
		break;
	case SDLK_k:
		hw->getPad(15).lock()->push(127);
		break;
	case SDLK_l:
		hw->getButton("rec").lock()->push();
		break;
	case SDLK_SEMICOLON:
		hw->getButton("overdub").lock()->push();
		break;
	case SDLK_QUOTE:
		hw->getButton("stop").lock()->push();
		break;
	case SDLK_SPACE:
		hw->getButton("play").lock()->push();
		break;
	case SDLK_BACKSLASH:
		hw->getButton("playstart").lock()->push();
		break;
	}
}
