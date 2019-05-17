#pragma once
#include <cairo.h>

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
	MColorF bgColor;
	MColorF fgColor;
	float rotation = 0.0f; // radians

protected:
	bool dirty = true;

public:
	Component(const MRECT&, const string& name);

protected:
	void prepareTranslate(cairo_t* context);
	void restoreTranslate(cairo_t* context);
	void prepareClip(cairo_t* context);
	void restoreClip(cairo_t* context);
	void prepareRotate(cairo_t* context);
	void restoreRotate(cairo_t* context);

public:
	virtual void draw(cairo_t* context, bool dirtyOnly);

public:
	//virtual vector<weak_ptr<Component>> getChildren() { return children; }
	void rotate(const float& angle);
    
public:
	void addChild(shared_ptr<Component> comp);
	weak_ptr<Component> findTopChild(const int x, const int y);
	const bool isDirty();
    void setDirty(const bool);
	bool contains(int x, int y);
	string getName() { return name; }

public:
	void setBgColor(const MColorF& color) { bgColor = color; }
	void setFgColor(const MColorF& color) { fgColor = color; }

};
