#pragma once

#include "Component.hpp"

class Lcd 
	: public Component
{

public:
	Lcd(const MRECT&, vector<vector<bool>>* pixels, const string& name);
	Lcd(const MRECT&, const float* bgColor, const float* fgColor, vector<vector<bool>>* pixels, const string& name);
	~Lcd();

private:
	float bgColor[3]{ 1.0f, 1.0f, 1.0f };
	float fgColor[3]{ 0.0f, 0.0f, 0.0f };
	vector<vector<bool>>* pixels;
	cairo_surface_t* surface;
	unsigned char* data;

public:
	void draw(cairo_t* context, bool dirtyOnly) override;

};

