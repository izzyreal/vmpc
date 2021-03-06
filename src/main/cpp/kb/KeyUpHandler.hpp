#pragma once

#include <SDL.h>

#include <Mpc.hpp>

class KeyUpHandler
{
public:
	KeyUpHandler(mpc::Mpc*);
	~KeyUpHandler();

public:
	void handle(const SDL_KeyboardEvent&);

private:
	mpc::Mpc* mpc;
};

