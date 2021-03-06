#pragma once

#include <SDL.h>

#include <Mpc.hpp>
#include <Gui.hpp>

class KeyDownHandler
{
public:
	KeyDownHandler(mpc::Mpc*, Gui*);
	~KeyDownHandler();

public:
	void handle(const SDL_KeyboardEvent&);

private:
	mpc::Mpc* mpc;
	Gui* gui;
};

