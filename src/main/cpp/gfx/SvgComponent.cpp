#include "SvgComponent.hpp"

SvgComponent::SvgComponent(const MRECT& rect, const std::function<void(cairo_t*)>& svgRenderFunc) : Component(rect)
{
    this->svgRenderFunc = svgRenderFunc;
}


SvgComponent::~SvgComponent()
{
}

void SvgComponent::draw(cairo_t* context) {	
	cairo_rectangle(context, (double) r.L, (double) r.T, (double) r.W(), (double) r.H());
	cairo_clip_preserve(context);
    svgRenderFunc(context);
	cairo_reset_clip(context);
    dirty = false;
}


void SvgComponent::rotate(const float& angle) {
	dirty = true;
}
