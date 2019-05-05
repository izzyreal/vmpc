#pragma once
#include "AbstractAudioWrapper.hpp"

#if AUDIO_LIBRARY == PORTAUDIO

#include "portaudio.h"

class PortAudioWrapper
{
public:
	PortAudioWrapper(PaStreamCallback* callback, void* callbackData, const string& filePath);
	~PortAudioWrapper();

private:
	void stopAndCloseStream();
	void safeDestroy();
    void logError(const PaError& e);
    
public:
	void start();
	void storePreferences();
	void loadPreferences();
	int getBufferSize();
	int getSampleRate();

	log4cplus::Logger logger = log4cplus::Logger::getRoot();
	PaStreamCallback* callback;
	void* callbackData;
    PaStream* stream = nullptr;
	AudioPreferences ap;
	string filePath = "";
};

#endif