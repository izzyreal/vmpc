#pragma once

#include "Component.hpp"

class Panel 
	: public Component
{

public:
	Panel(const MRECT&, const string& name);
	~Panel();

private:
	unsigned char color[4];

public:
	void draw(cairo_t* context, bool dirtyOnly) override;

};

