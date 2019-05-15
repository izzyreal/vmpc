#pragma once

#include <SDL.h>

#include <Mpc.hpp>
#include <Gui.hpp>

class MouseUpHandler
{
public:
	MouseUpHandler(mpc::Mpc*, Gui*);
	~MouseUpHandler();

public:
	void handle(const SDL_MouseButtonEvent&);

private:
	mpc::Mpc* mpc;
	Gui* gui;
};

