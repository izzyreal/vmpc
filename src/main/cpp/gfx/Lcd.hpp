#pragma once

#include "Component.hpp"

class Lcd 
	: public Component
{

public:
	Lcd(const MRECT&, vector<vector<bool>>* pixels, const string& name);
	~Lcd();

private:
	vector<vector<bool>>* pixels;
	cairo_surface_t* surface;
	unsigned char* data;

public:
	void draw(cairo_t* context, bool dirtyOnly) override;

};

