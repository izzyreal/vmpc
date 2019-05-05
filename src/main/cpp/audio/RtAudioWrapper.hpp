#pragma once
#include "AbstractAudioWrapper.hpp"

#if AUDIO_LIBRARY == RTAUDIO
#include "RtAudio.h"

class RtAudioWrapper : public AbstractAudioWrapper
{
public:
	RtAudioWrapper(const RtAudioCallback& callback, void* callbackData, const string& filePath);
	~RtAudioWrapper();

protected:
	void stopAndCloseStream() override;
	void safeDestroy() override;

public:
	void start() override;

private:
	RtAudioCallback callback;
	RtAudio* audio = nullptr;
};
#endif
