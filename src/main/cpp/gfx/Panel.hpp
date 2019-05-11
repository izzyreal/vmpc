#pragma once

#include "Component.hpp"

class Panel 
	: public Component
{

public:
	Panel(const MRECT&, const string& name);
	Panel(const MRECT&, const float* color, const string& name);
	~Panel();

private:
	float color[3]{ 1.0f, 1.0f, 1.0f };

public:
	void draw(cairo_t* context, bool dirtyOnly) override;

};

