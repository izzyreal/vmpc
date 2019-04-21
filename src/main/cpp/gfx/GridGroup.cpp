#include "GridGroup.hpp"

GridGroup::GridGroup(const MRECT& rect, shared_ptr<Component> component) : Group(rect)
{
	this->component = move(component);
	
}

GridGroup::~GridGroup()
{
}
