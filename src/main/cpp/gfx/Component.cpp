#include "Component.hpp"

Component::Component(const MRECT& rect, const string& name) {
	r = rect;
	this->name = name;
}


void Component::addChild(shared_ptr<Component> comp) {
	children.push_back(move(comp));
}

weak_ptr<Component> Component::findTopChild(const int x, const int y) {
	for (int i = children.size() - 1; i >= 0; i--) {
		const auto c = children[i];
		auto candidate = c->findTopChild(x, y).lock();
		if (candidate) return candidate;
		if (c->contains(x, y)) return c;
	}
	return {};
}

bool Component::isDirty() {
	for (auto& c : children) {
		if (c->isDirty()) return true;
	}
	return dirty;
}

void Component::prepare(cairo_t* context) {
	auto l = static_cast<double>(r.L);
	auto t = static_cast<double>(r.T);
	cairo_rectangle(context, l, t, static_cast<double>(r.W()), static_cast<double>(r.H()));
	cairo_clip(context);
	cairo_translate(context, l, t);
}

void Component::restore(cairo_t* context) {
	auto l = static_cast<double>(r.L);
	auto t = static_cast<double>(r.T);
	cairo_translate(context, -l, -t);
	cairo_reset_clip(context);
}

bool Component::contains(const int x, const int y) {
	return r.Contains(x, y);
}

void Component::draw(cairo_t* context, bool dirtyOnly) {
	for (int i = children.size() - 1; i >= 0; i--) {
		children[i]->draw(context, dirtyOnly);
	}
}
