#include "Lcd.hpp"

Lcd::Lcd(const MRECT& rect, vector<vector<bool>>* pixels, const string& name)
	: Component(rect, name)
{
	this->pixels = pixels;
}

Lcd::Lcd(const MRECT& rect, const float* color, vector<vector<bool>>* pixels, const string& name)
	: Lcd(rect, pixels, name)
{
	this->color[0] = color[0];
	this->color[1] = color[1];
	this->color[2] = color[2];
}

void Lcd::draw(cairo_t* context, bool dirtyOnly) {
	if (!dirtyOnly || (dirtyOnly && dirty)) {
		prepareClip(context);
		
		cairo_set_source_rgb(context, color[0], color[1], color[2]);
		cairo_rectangle(context, r.L, r.T, r.W(), r.H());
		cairo_stroke_preserve(context);
		cairo_fill(context);

		surface = cairo_image_surface_create(CAIRO_FORMAT_RGB24, 248, 60);
		data = cairo_image_surface_get_data(surface);
		for (int y = 0; y < 60; y++) {
			for (int x = 0; x < 248; x++) {
				const bool pixel = (*pixels)[x][y];
				int offset = ((y * 248) + x) * 4;
				if (pixel) {
					data[offset + 0] = 0;
					data[offset + 1] = 0;
					data[offset + 2] = 0;
				}
				else {
					data[offset + 0] = 255;
					data[offset + 1] = 255;
					data[offset + 2] = 255;
				}
			}
		}

		
		cairo_set_source_surface(context, surface, 0, 0);

		cairo_paint(context);

		restoreClip(context);
		dirty = false;
	}
	Component::draw(context, dirtyOnly);
}

Lcd::~Lcd()
{
}
