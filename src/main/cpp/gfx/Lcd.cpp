#include "Lcd.hpp"

Lcd::Lcd(const MRECT& rect, vector<vector<bool>>* pixels, const string& name)
	: Component(rect, name)
{
	this->pixels = pixels;
	surface = cairo_image_surface_create(CAIRO_FORMAT_RGB24, 248 * 2, 60 * 2);
	data = cairo_image_surface_get_data(surface);
}

void Lcd::draw(cairo_t* context, bool dirtyOnly) {
	if (!dirtyOnly || (dirtyOnly && dirty)) {
		prepareClip(context);
		prepareTranslate(context);
		const float lightnessFactor = 2.0f;
		MColorF fgColor2 = { 1.0f, fgColor.R * lightnessFactor, fgColor.G * lightnessFactor, fgColor.B * lightnessFactor };
		for (int y = 0; y < 60; y++) {
			for (int x = 0; x < 248; x++) {
				const bool pixel = (*pixels)[x][y];
				int offset1 = ((y * 496 * 2) + (x * 2)) * 4;
				int offset2 = ((y * 496 * 2) + (x * 2) + 1) * 4;
				int offset3 = ((y * 496 * 2) + 496 + (x * 2)) * 4;
				int offset4 = ((y * 496 * 2) + 496 + (x * 2) + 1) * 4;
				if (pixel) {
					setPixel3F(data + offset1, fgColor);
					setPixel3F(data + offset2, fgColor2);
					setPixel3F(data + offset3, fgColor2);
					setPixel3F(data + offset4, fgColor2);
				}
				else {
					setPixel3F(data + offset1, bgColor);
					setPixel3F(data + offset2, bgColor);
					setPixel3F(data + offset3, bgColor);
					setPixel3F(data + offset4, bgColor);
				}
			}
		}
		
		cairo_scale(context, 0.5f, 0.5f);
		cairo_set_source_surface(context, surface, 0, 0);
		cairo_paint(context);
		cairo_scale(context, 2.0f, 2.0f);

		restoreTranslate(context);
		restoreClip(context);
		dirty = false;
	}
	Component::draw(context, dirtyOnly);
}

Lcd::~Lcd()
{
}
