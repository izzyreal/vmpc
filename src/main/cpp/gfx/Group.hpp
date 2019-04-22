#pragma once

#include "Component.hpp"

using namespace std;

class Group :
	public Component
{
public:
	Group(const MRECT&, const string& name);
	~Group();
private:
	// The group becomes the owner of its components
	vector<shared_ptr<Component>> components;

public:
	void addComp(shared_ptr<Component> comp);
	vector<weak_ptr<Component>> getComponents();

public:
	void draw(cairo_t* context) override;

public:
	vector<weak_ptr<Component>> getChildren() override;
};

