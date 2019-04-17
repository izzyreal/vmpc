#pragma once

#include "Component.hpp"

class SvgComponent :
	public Component
{
public:
	SvgComponent();
	~SvgComponent();

public:
	void draw(cairo_t* context) override;

public:
	void rotate(const float& angle) override;
};

