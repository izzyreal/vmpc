#include "Component.hpp"

Component::Component(const MRECT& rect, const string& name) {
	r = rect;
	this->name = name;
}

void Component::prepare(cairo_t* context, const bool& clip) {
	auto l = static_cast<double>(r.L);
	auto t = static_cast<double>(r.T);
	if (clip) {
		cairo_rectangle(context, l, t, static_cast<double>(r.W()), static_cast<double>(r.H()));
		cairo_clip_preserve(context);
	}
	cairo_translate(context, l, t);
}

void Component::restore(cairo_t* context, const bool& clip) {
	auto l = static_cast<double>(r.L);
	auto t = static_cast<double>(r.T);
	cairo_translate(context, -l, -t);
	if (clip) {
		cairo_reset_clip(context);
	}
}

bool Component::contains(const int x, const int y) {
	return r.Contains(x, y);
}
