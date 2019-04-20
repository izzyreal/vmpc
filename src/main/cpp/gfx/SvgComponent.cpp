#include "SvgComponent.hpp"

SvgComponent::SvgComponent(const std::function<void(cairo_t*)>& svgRenderFunc)
{
    this->svgRenderFunc = svgRenderFunc;
}


SvgComponent::~SvgComponent()
{
}

void SvgComponent::draw(cairo_t* context) {
//	cairo_set_source_rgb(context, 255, 255, 255);
//	cairo_paint(context);
    svgRenderFunc(context);
    dirty = false;
}


void SvgComponent::rotate(const float& angle) {
	dirty = true;
}
