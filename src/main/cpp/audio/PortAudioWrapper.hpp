#pragma once
#include "AbstractAudioWrapper.hpp"
#include "AudioPreferences.hpp"
#include "portaudio.h"

#include <log4cplus/log4cplus.h>

class PortAudioWrapper : public AbstractAudioWrapper
{
public:
	PortAudioWrapper(PaStreamCallback* callback, void* callbackData, const string& filePath);
	~PortAudioWrapper();

protected:
	void logError(const PaError& e);

public:
	void start() override;

	PaStreamCallback* callback;
	PaStream* stream = nullptr;
};
