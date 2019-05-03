#pragma once
#include "AudioPreferences.hpp"
#include "portaudio.h"

#include <log4cplus/log4cplus.h>

class RtAudioServer
{
public:
	RtAudioServer(PaStreamCallback* callback, void* callbackData, const string& filePath);
	~RtAudioServer();

private:
	void stopAndCloseStream();
	void safeDestroy();

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

