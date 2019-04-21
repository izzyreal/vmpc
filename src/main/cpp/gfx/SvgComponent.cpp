#include "SvgComponent.hpp"

SvgComponent::SvgComponent(const MRECT& rect, const std::function<void(cairo_t*)>& svgRenderFunc) : Component(rect)
{
    this->svgRenderFunc = svgRenderFunc;
}


SvgComponent::~SvgComponent()
{
}

void SvgComponent::draw(cairo_t* context) {
	auto l = static_cast<double>(r.L);
	auto t = static_cast<double>(r.T);
	
	cairo_rectangle(context, l, t, static_cast<double>(r.W()), static_cast<double>(r.H()));
	cairo_clip_preserve(context);
	cairo_translate(context, l, t);
	svgRenderFunc(context);
	cairo_translate(context, -l, -t);
	cairo_reset_clip(context);
	dirty = false;
}


void SvgComponent::rotate(const float& angle) {
	dirty = true;
}
