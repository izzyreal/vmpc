#pragma once
#include <cairo-svg.h>

#include <gui/BasicStructs.hpp>

#include <memory>
#include <vector>
#include <string>

using namespace std;

class Component {
private:
	string name = "";
	vector<shared_ptr<Component>> children;

protected:
	MRECT r;
	bool dirty = true;

public:
	Component(const MRECT&, const string& name);

protected:
	void prepare(cairo_t* context, const bool& clip);
	void restore(cairo_t* context, const bool& clip);

public:
	virtual void draw(cairo_t* context) {
		for (auto& c : children) {
			if (c->isDirty()) {
				c->draw(context);
			}
		}
	}

public:
	//virtual vector<weak_ptr<Component>> getChildren() { return children; }
	//virtual void rotate(const float& angle) {}
    
public:
	void addChild(shared_ptr<Component> comp);
	weak_ptr<Component> findTopChild(const int x, const int y);
    bool isDirty() { return dirty; }
	bool contains(int x, int y);
	string getName() { return name; }
};

/*
class Group : Component{
public:
	Group(const MRECT& rect) : Component(rect) {}
private:
	vector<std::weak_ptr<Component>> group;
public:
    void draw(cairo_t* context) override { for (auto& el : group) el.lock()->draw(context); dirty = false; }
};
*/