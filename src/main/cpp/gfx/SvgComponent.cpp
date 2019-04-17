#include "SvgComponent.hpp"
#include "mpc.h"

SvgComponent::SvgComponent()
{
}


SvgComponent::~SvgComponent()
{
}

void SvgComponent::draw(cairo_t* context) {
	if (!dirty) return;
	cairo_set_source_rgb(context, 255, 255, 255);
	cairo_paint(context);
	cairo_code_mpc_render(context);
	dirty = false;
}


void SvgComponent::rotate(const float& angle) {
	dirty = true;
}
