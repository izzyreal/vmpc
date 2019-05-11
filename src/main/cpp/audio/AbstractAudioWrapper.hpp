#pragma once
#include "AudioWrapperConfig.hpp"

#include "AudioPreferences.hpp"

#include <log4cplus/log4cplus.h>

class AbstractAudioWrapper
{
protected:
	AbstractAudioWrapper(void* callbackData, const string& filePath);
	~AbstractAudioWrapper();

public:
	virtual void start() = 0;
	virtual void stop() {}
	virtual const int getInputCount() { return 0; }
	virtual const int getStereoOutputCount() { return 0; }

public:
	void storePreferences();
	void loadPreferences();
	int getBufferSize();
	int getSampleRate();

protected:
	const log4cplus::Logger logger = log4cplus::Logger::getRoot();
	void* callbackData;
	AudioPreferences ap;
	string filePath = "";

};
