//#include "Panel.hpp"
//
//Panel::Panel(const MRECT& rect, const string& name)
//    : Component(rect, name)
//{
//}
//
//void Panel::draw(cairo_t* context, bool dirtyOnly) {
//    if (!dirtyOnly || (dirtyOnly && dirty)) {
//        prepareClip(context);
//        cairo_set_source_rgb(context, bgColor.R, bgColor.G, bgColor.B);
//        cairo_rectangle(context, r.L, r.T, r.W(), r.H());
//        cairo_stroke_preserve(context);
//        cairo_fill(context);
//        restoreClip(context);
//        dirty = false;
//    }
//    Component::draw(context, dirtyOnly);
//}
//
//Panel::~Panel()
//{
//}
