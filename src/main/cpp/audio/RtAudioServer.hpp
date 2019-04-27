#pragma once
#include "AudioPreferences.hpp"
#include "RtAudio.h"

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

private:
	RtAudioCallback callback;
	RtAudio* audio = nullptr;
	AudioPreferences ap;
	string filePath = "";
};

