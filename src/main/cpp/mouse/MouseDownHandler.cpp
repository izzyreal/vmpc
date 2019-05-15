#include "MouseDownHandler.hpp"

#include <hardware/Hardware.hpp>
#include <hardware/HwPad.hpp>
#include <hardware/Button.hpp>
#include <hardware/DataWheel.hpp>

#include <gfx/Component.hpp>

MouseDownHandler::MouseDownHandler(mpc::Mpc* _mpc, Gui* _gui) : mpc(_mpc), gui(_gui)
{
}

MouseDownHandler::~MouseDownHandler()
{
}

void MouseDownHandler::handle(const SDL_MouseButtonEvent& event) {
	const auto x = event.x / gui->getUserScale();
	const auto y = event.y / gui->getUserScale();
	auto c = gui->findTopChild(x, y).lock();
	const auto name = c->getName();
	if (name.find("pad") == 0) {
		const string numberStr = name.substr(3);
		try {
			const auto numberInt = stoi(numberStr);
			const auto pad = mpc->getHardware().lock()->getPad(numberInt - 1).lock();
			pad->push(127);
		}
		catch (const exception& e) {
			// not a number, shouldn't happen
		}
	}
}
