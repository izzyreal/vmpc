#pragma once
#include "AudioWrapperConfig.hpp"

#include "AudioPreferences.hpp"

#include <log4cplus/log4cplus.h>

class AbstractAudioWrapper
{
protected:
	AbstractAudioWrapper(void* callbackData, const string& filePath);
	~AbstractAudioWrapper();

protected:
	virtual void stopAndCloseStream() = 0;
	virtual void safeDestroy() = 0;

public:
	virtual void start() = 0;

public:
	void storePreferences();
	void loadPreferences();
	int getBufferSize();
	int getSampleRate();

protected:
	log4cplus::Logger logger = log4cplus::Logger::getRoot();
	void* callbackData;
	AudioPreferences ap;
	string filePath = "";

};
