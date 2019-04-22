#include "Group.hpp"

Group::Group(const MRECT& rect, const string& name) : Component(rect, name)
{
}

Group::~Group()
{
}

void Group::addComp(shared_ptr<Component> comp) {
	components.push_back(move(comp));
}

void Group::draw(cairo_t* context) {
	prepare(context, false);
	for (auto& c : components)
		c->draw(context);
	restore(context, false);
	dirty = false; 
}

vector<weak_ptr<Component>> Group::getComponents() {
	return vector<weak_ptr<Component>>(components.begin(), components.end());
}

vector<weak_ptr<Component>> Group::getChildren() {
	return vector<weak_ptr<Component>>(components.begin(), components.end());
}
