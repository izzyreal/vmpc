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
	virtual void draw(cairo_t* context) = 0;

public:
	virtual void rotate(const float& angle) {}
};

class Group : Component{
private:
	vector<std::weak_ptr<Component>> group;
public:
	void draw(cairo_t* context) override { for (auto& el : group) el.lock()->draw(context); }
};
