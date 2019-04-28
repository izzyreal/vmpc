#pragma once
#include "AudioPreferences.hpp"
#include "RtAudio.h"

#include <log4cplus/log4cplus.h>

class RtAudioServer
{
public:
	RtAudioServer(const RtAudioCallback& callback, const string& filePath);
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

	log4cplus::Logger logger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("main"));
	RtAudioCallback callback;
	RtAudio* audio = nullptr;
	AudioPreferences ap;
	string filePath = "";
};

