#pragma once
#include <cairo-svg.h>

#include <gui/BasicStructs.hpp>

#include <memory>
#include <vector>

using namespace std;

class Component {
protected:
	MRECT r;
	bool dirty = true;

public:
	Component(const MRECT&);

public:
	virtual void draw(cairo_t* context) = 0;

public:
	virtual void rotate(const float& angle) {}
    
public:
    bool isDirty() { return dirty; }
};

class Group : Component{
public:
	Group(const MRECT& rect) : Component(rect) {}
private:
	vector<std::weak_ptr<Component>> group;
public:
    void draw(cairo_t* context) override { for (auto& el : group) el.lock()->draw(context); dirty = false; }
};
