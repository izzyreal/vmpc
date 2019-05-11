#include "Panel.hpp"

Panel::Panel(const MRECT& rect, const string& name)
	: Component(rect, name)
{
}

Panel::Panel(const MRECT& rect, const float* color, const string& name)
	: Component(rect, name)
{
	this->color[0] = color[0];
	this->color[1] = color[1];
	this->color[2] = color[2];
}

void Panel::draw(cairo_t* context, bool dirtyOnly) {
	if (!dirtyOnly || (dirtyOnly && dirty)) {
		prepareClip(context);
		cairo_set_source_rgb(context, color[0], color[1], color[2]);
		cairo_rectangle(context, r.L, r.T, r.W(), r.H());
		cairo_stroke_preserve(context);
		cairo_fill(context);
		restoreClip(context);
		dirty = false;
	}
	Component::draw(context, dirtyOnly);
}

Panel::~Panel()
{
}
