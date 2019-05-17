#include "Component.hpp"

Component::Component(const MRECT& rect, const string& name) {
	r = rect;
	this->name = name;
}

void Component::rotate(const float& angle) {
	rotation += angle;
    dirty = true;
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

const bool Component::isDirty() {
	for (auto& c : children) {
		if (c->isDirty()) return true;
	}
	return dirty;
}

void Component::setDirty(const bool b) {
    dirty = b;
}

void Component::prepareTranslate(cairo_t* context) {
	const auto l = static_cast<double>(r.L);
	const auto t = static_cast<double>(r.T);
	cairo_translate(context, l, t);
}

void Component::restoreTranslate(cairo_t* context) {
	auto l = static_cast<double>(r.L);
	auto t = static_cast<double>(r.T);
	cairo_translate(context, -l, -t);
}

void Component::prepareClip(cairo_t* context) {
	const auto l = static_cast<double>(r.L);
	const auto t = static_cast<double>(r.T);
	cairo_rectangle(context, l, t, static_cast<double>(r.W()), static_cast<double>(r.H()));
	cairo_clip(context);
}

void Component::restoreClip(cairo_t* context) {
	cairo_reset_clip(context);
}

void Component::prepareRotate(cairo_t* context) {
	if (rotation == 0.0f) return;
	cairo_translate(context, r.MW(), r.MH());
	cairo_rotate(context, rotation);
	cairo_translate(context, -r.MW() , -r.MH());
}

void Component::restoreRotate(cairo_t* context) {
	if (rotation == 0.0f) return;
	cairo_translate(context, r.MW(), r.MH());
	cairo_rotate(context, -rotation);
	cairo_translate(context, -r.MW(), -r.MH());
}


bool Component::contains(const int x, const int y) {
	return r.Contains(x, y);
}

void Component::draw(cairo_t* context, bool dirtyOnly, vector<MRECT*>& updatedRects) {
	for (int i = 0; i < children.size(); i++) {
		children[i]->draw(context, dirtyOnly, updatedRects);
	}
}
