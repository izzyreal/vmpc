#pragma once

#include "Component.hpp"
#include <functional>

class SvgComponent :
	public Component
{
private:
    std::function<void(cairo_t*)> svgRenderFunc;

public:
	SvgComponent(const MRECT&, const std::function<void(cairo_t*)>& svgRenderFunc);
	//SvgComponent(const cairo_rectangle_t&, const std::function<void(cairo_t*)>& svgRenderFunc);
	~SvgComponent();

public:
	void draw(cairo_t* context) override;

public:
	void rotate(const float& angle) override;
};

