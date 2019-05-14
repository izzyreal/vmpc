#pragma once

#include <SDL.h>

#include <Mpc.hpp>

class KeyDownHandler
{
public:
	KeyDownHandler(const mpc::Mpc*);
	~KeyDownHandler();

public:
	void handle(const SDL_KeyboardEvent&);

private:
	const mpc::Mpc* mpc;
};

