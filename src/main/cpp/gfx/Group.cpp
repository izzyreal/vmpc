#include "Group.hpp"

Group::Group(const MRECT& rect) : Component(rect)
{
}

Group::~Group()
{
}

void Group::addComp(shared_ptr<Component> comp) {
	group.push_back(move(comp));
}

void Group::draw(cairo_t* context) {
	prepare(context, false);
	for (auto& el : group)
		el->draw(context);
	restore(context, false);
	dirty = false; 
}
