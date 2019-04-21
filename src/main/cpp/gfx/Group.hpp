#pragma once

#include "Component.hpp"

using namespace std;

class Group :
	public Component
{
public:
	Group(const MRECT&);
	~Group();
private:
	// The group becomes the owner of its components
	vector<shared_ptr<Component>> group;

public:
	void addComp(shared_ptr<Component> comp);

public:
	void draw(cairo_t* context) override;
};

