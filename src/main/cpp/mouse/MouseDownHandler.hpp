#pragma once

#include <SDL.h>

#include <Mpc.hpp>
#include <Gui.hpp>

class MouseDownHandler
{
public:
	MouseDownHandler(mpc::Mpc*, Gui*);
	~MouseDownHandler();

public:
	void handle(const SDL_MouseButtonEvent&);

private:
	mpc::Mpc* mpc;
	Gui* gui;
};

