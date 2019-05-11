#pragma once
#include "AbstractAudioWrapper.hpp"

#if AUDIO_LIBRARY == RTAUDIO

#include "RtAudio.h"
#include <memory>

using namespace std;

class RtAudioWrapper : public AbstractAudioWrapper
{
public:
	RtAudioWrapper(const RtAudioCallback& callback, void* callbackData, const string& filePath);
	~RtAudioWrapper();

public:
	void start() override;
	void stop() override;

	const int getInputCount() override;
	const int getStereoOutputCount() override;

private:
	RtAudioCallback callback;
	shared_ptr<RtAudio> audio;
	RtAudio::StreamParameters inParam;
	RtAudio::StreamParameters outParam;

};
#endif
