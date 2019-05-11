#include "Label.hpp"

Label::Label(const MRECT& rect, const string& name, const string& text)
	: Component(rect, name)
{
	this->text = text;
}

Label::Label(const MRECT& rect, const float* color, const string& name, const string& text)
	: Label(rect, name, text)
{
	this->color[0] = color[0];
	this->color[1] = color[1];
	this->color[2] = color[2];
}

void Label::draw(cairo_t* context, bool dirtyOnly) {
	if (!dirtyOnly || (dirtyOnly && dirty)) {
		prepareClip(context);
		prepareTranslate(context);
		cairo_set_source_rgb(context, color[0], color[1], color[2]);

		cairo_select_font_face(context, "Arial",
			CAIRO_FONT_SLANT_NORMAL,
			CAIRO_FONT_WEIGHT_BOLD);

		cairo_set_font_size(context, 33);
		cairo_move_to(context, 20, 30);
		cairo_show_text(context, text.c_str());

		restoreTranslate(context);
		restoreClip(context);
		dirty = false;
	}
	Component::draw(context, dirtyOnly);
}

Label::~Label()
{
}
