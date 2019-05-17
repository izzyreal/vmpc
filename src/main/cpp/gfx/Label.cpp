//#include "Label.hpp"
//
//Label::Label(const MRECT& rect, const string& name, const string& text)
//    : Component(rect, name)
//{
//    this->text = text;
//}
//
//void Label::draw(cairo_t* context, bool dirtyOnly) {
//    if (!dirtyOnly || (dirtyOnly && dirty)) {
//        prepareClip(context);
//        prepareTranslate(context);
//        cairo_set_source_rgb(context, bgColor.R, bgColor.G, bgColor.B);
//
//        cairo_select_font_face(context, "Arial",
//            CAIRO_FONT_SLANT_NORMAL,
//            CAIRO_FONT_WEIGHT_BOLD);
//
//        cairo_set_font_size(context, 33);
//        cairo_move_to(context, 20, 30);
//        cairo_show_text(context, text.c_str());
//
//        restoreTranslate(context);
//        restoreClip(context);
//        dirty = false;
//    }
//    Component::draw(context, dirtyOnly);
//}
//
//Label::~Label()
//{
//}
