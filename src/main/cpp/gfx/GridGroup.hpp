#pragma once

#include "Group.hpp"

using namespace std;

class GridGroup :
	public Group
{
public:
	GridGroup(const MRECT&, shared_ptr<Component> component);
	~GridGroup();
private:
	shared_ptr<Component> component;
};

