#include "Lcd.hpp"

Lcd::Lcd(const MRECT& rect, vector<vector<bool>>* pixels, const string& name)
	: Component(rect, name)
{
	this->pixels = pixels;
	surface = cairo_image_surface_create(CAIRO_FORMAT_RGB24, 248, 60);
	data = cairo_image_surface_get_data(surface);
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
					setPixel3F(data + offset, fgColor);
				}
				else {
					setPixel3F(data + offset, bgColor);
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
