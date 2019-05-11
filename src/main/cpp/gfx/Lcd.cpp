#include "Lcd.hpp"

Lcd::Lcd(const MRECT& rect, vector<vector<bool>>* pixels, const string& name)
	: Component(rect, name)
{
	this->pixels = pixels;
	surface = cairo_image_surface_create(CAIRO_FORMAT_RGB24, 248, 60);
	data = cairo_image_surface_get_data(surface);
}

Lcd::Lcd(const MRECT& rect, const float* bgColor, const float* fgColor, vector<vector<bool>>* pixels, const string& name)
	: Lcd(rect, pixels, name)
{
	this->bgColor[0] = bgColor[0];
	this->bgColor[1] = bgColor[1];
	this->bgColor[2] = bgColor[2];
	this->fgColor[0] = fgColor[0];
	this->fgColor[1] = fgColor[1];
	this->fgColor[2] = fgColor[2];
}

void Lcd::draw(cairo_t* context, bool dirtyOnly) {
	if (!dirtyOnly || (dirtyOnly && dirty)) {
		prepareClip(context);
		prepareTranslate(context);
		for (int y = 0; y < 60; y++) {
			for (int x = 0; x < 248; x++) {
				const bool pixel = (*pixels)[x][y];
				int offset = ((y * 248) + x) * 4;
				if (pixel) {
					data[offset + 0] = fgColor[0] * 255;
					data[offset + 1] = fgColor[1] * 255;
					data[offset + 2] = fgColor[2] * 255;
				}
				else {
					data[offset + 0] = bgColor[0] * 255;
					data[offset + 1] = bgColor[1] * 255;
					data[offset + 2] = bgColor[2] * 255;
				}
			}
		}

		
		cairo_set_source_surface(context, surface, 0, 0);

		cairo_paint(context);

		restoreTranslate(context);
		restoreClip(context);
		dirty = false;
	}
	Component::draw(context, dirtyOnly);
}

Lcd::~Lcd()
{
}
