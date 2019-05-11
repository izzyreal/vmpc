#include "AbstractAudioWrapper.hpp"

#include <file/File.hpp>

#include <rapidjson/document.h>

AbstractAudioWrapper::AbstractAudioWrapper(void* callbackData, const string& filePath)
{
	this->callbackData = callbackData;
	this->ap = ap;
	this->filePath = filePath;
	loadPreferences();
}

void AbstractAudioWrapper::loadPreferences() {
	LOG4CPLUS_INFO_STR(logger, LOG4CPLUS_STRING_TO_TSTRING("Trying to load audio preferences from " + filePath + "..."));
	try {
		ap = AudioPreferences(filePath);
	}
	catch (const SerializationException& e) {
		const char* msg = e.what();
		printf("%s\n", msg);
		ap = AudioPreferences();
	}
}

void AbstractAudioWrapper::storePreferences() {
	moduru::file::File f(filePath, nullptr);
	f.create();
	f.openWrite();
	for (auto c : ap.serialize())
		f.writeByte(c);
	f.close();
}

AbstractAudioWrapper::~AbstractAudioWrapper()
{
}

int AbstractAudioWrapper::getBufferSize() {
	return ap.getBufferSize();
}
int AbstractAudioWrapper::getSampleRate() {
	return ap.getSampleRate();
}
