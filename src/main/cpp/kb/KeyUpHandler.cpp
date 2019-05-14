#include "KeyUpHandler.hpp"

#include <hardware/Hardware.hpp>
#include <hardware/HwPad.hpp>
#include <hardware/Button.hpp>
#include <hardware/DataWheel.hpp>

KeyUpHandler::KeyUpHandler(mpc::Mpc* _mpc) : mpc(_mpc)
{
}

KeyUpHandler::~KeyUpHandler()
{
}

void KeyUpHandler::handle(const SDL_KeyboardEvent& event) {
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
	case SDLK_1:
		hw->getButton("1").lock()->release();
		break;
	case SDLK_2:
		hw->getButton("2").lock()->release();
		break;
	case SDLK_3:
		hw->getButton("3").lock()->release();
		break;
	case SDLK_4:
		hw->getButton("4").lock()->release();
		break;
	case SDLK_5:
		hw->getButton("5").lock()->release();
		break;
	case SDLK_6:
		hw->getButton("6").lock()->release();
		break;
	case SDLK_7:
		hw->getButton("7").lock()->release();
		break;
	case SDLK_8:
		hw->getButton("8").lock()->release();
		break;
	case SDLK_9:
		hw->getButton("9").lock()->release();
		break;
	case SDLK_0:
		hw->getButton("0").lock()->release();
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
}
