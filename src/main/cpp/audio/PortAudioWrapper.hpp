#pragma once
#include "AbstractAudioWrapper.hpp"

#if AUDIO_LIBRARY == PORTAUDIO

#include "portaudio.h"

class PortAudioWrapper : public AbstractAudioWrapper
{
public:
	PortAudioWrapper(PaStreamCallback* callback, void* callbackData, const string& filePath);
	~PortAudioWrapper();

private:
	void stopAndCloseStream();
	void safeDestroy();
    void logError(const PaError& e);
    
public:
	void start() override;

	PaStreamCallback* callback;
    PaStream* stream = nullptr;
};

#endif