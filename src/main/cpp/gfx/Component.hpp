#pragma once
#include <cairo-svg.h>

#include <gui/BasicStructs.hpp>

#include <memory>
#include <vector>
#include <string>

using namespace std;

class Component {
private:
	vector<shared_ptr<Component>> children;

protected:
	MRECT r;
	string name = "";

protected:
	bool dirty = true;

public:
	Component(const MRECT&, const string& name);

protected:
	void prepare(cairo_t* context);
	void restore(cairo_t* context);

public:
	virtual void draw(cairo_t* context, bool dirtyOnly);

public:
	//virtual vector<weak_ptr<Component>> getChildren() { return children; }
	//virtual void rotate(const float& angle) {}
    
public:
	void addChild(shared_ptr<Component> comp);
	weak_ptr<Component> findTopChild(const int x, const int y);
	bool isDirty();
	bool contains(int x, int y);
	string getName() { return name; }

};
