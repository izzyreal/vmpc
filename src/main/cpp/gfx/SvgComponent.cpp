#include "SvgComponent.hpp"

#include <log4cplus/log4cplus.h>

SvgComponent::SvgComponent(const MRECT& rect, const string& name, const std::function<void(cairo_t*)>& svgRenderFunc) : Component(rect, name)
{
    this->svgRenderFunc = svgRenderFunc;
}


SvgComponent::~SvgComponent()
{
}

void SvgComponent::draw(cairo_t* context, bool dirtyOnly) {
	
	if (!dirtyOnly || (dirtyOnly && dirty)) {
	
		auto logger = log4cplus::Logger::getRoot();
		LOG4CPLUS_TRACE(logger, LOG4CPLUS_TEXT("Drawing SvgComponent ") << LOG4CPLUS_STRING_TO_TSTRING(name));

		prepareClip(context);
		prepareRotate(context);
		prepareTranslate(context);
		svgRenderFunc(context);
		restoreTranslate(context);
		restoreRotate(context);
		restoreClip(context);
		dirty = false;
	}

	Component::draw(context, dirtyOnly);
}
