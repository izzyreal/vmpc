#pragma once
#include "AudioPreferences.hpp"
#include "RtAudio.h"

class RtAudioServer
{
public:
	RtAudioServer(const RtAudioCallback& callback, AudioPreferences& ap);
	~RtAudioServer();

private:
	void stopAndCloseStream();
	void safeDestroy();

public:
	void loadAudioPreferences(AudioPreferences& ap);

private:
	RtAudioCallback callback;
	RtAudio* audio = nullptr;
};

