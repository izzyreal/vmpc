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
	storePreferences();
// Is there a way to call safeDestroy() from this class's destructor?
//	safeDestroy();
}

int AbstractAudioWrapper::getBufferSize() {
	return ap.getBufferSize();
}
int AbstractAudioWrapper::getSampleRate() {
	return ap.getSampleRate();
}
