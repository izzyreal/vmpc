#pragma once

#include "Component.hpp"

#include <string>

using namespace std;

class Label 
	: public Component
{

public:
	Label(const MRECT&, const string& name, const string& text);
	Label(const MRECT&, const float* color, const string& name, const string& text);
	~Label();

private:
	float color[3]{ 1.0f, 1.0f, 1.0f };
	string text;

public:
	void draw(cairo_t* context, bool dirtyOnly) override;

};

