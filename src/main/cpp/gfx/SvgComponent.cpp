#include "SvgComponent.hpp"

SvgComponent::SvgComponent(const MRECT& rect, const string& name, const std::function<void(cairo_t*)>& svgRenderFunc) : Component(rect, name)
{
    this->svgRenderFunc = svgRenderFunc;
}


SvgComponent::~SvgComponent()
{
}

void SvgComponent::draw(cairo_t* context) {
	prepare(context, true);
	svgRenderFunc(context);
	restore(context, true);
	dirty = false;
}

void SvgComponent::rotate(const float& angle) {
	dirty = true;
}
