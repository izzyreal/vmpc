#include "RtAudioWrapper.hpp"

#if AUDIO_LIBRARY == RTAUDIO

#include <file/File.hpp>

#include <rapidjson/document.h>

RtAudioWrapper::RtAudioWrapper(const RtAudioCallback& callback, void* callbackData, const string& filePath)
	: AbstractAudioWrapper(callbackData, filePath)
{
	this->callback = callback;
}

RtAudioWrapper::~RtAudioWrapper()
{
	safeDestroy();
}

void RtAudioWrapper::safeDestroy() {
	if (audio != nullptr) {
		if (audio->isStreamRunning()) {
			audio->stopStream();
		}
		if (audio->isStreamOpen()) {
			audio->closeStream();
		}
		delete audio;
	}
}

void RtAudioWrapper::start() {

	safeDestroy();

	map<DriverType, RtAudio::Api> apis = {
	{ DriverType::ASIO, RtAudio::WINDOWS_ASIO },
	{ DriverType::WASAPI, RtAudio::WINDOWS_WASAPI },
	{ DriverType::DIRECTSOUND, RtAudio::WINDOWS_DS },
	{ DriverType::CORE_AUDIO, RtAudio::MACOSX_CORE }
	};

	audio = new RtAudio(apis[ap.getDriverType()]);

	unsigned int bufSize = ap.getBufferSize();
	
	// We start off with an input device ID that always works
	auto inputDevId = audio->getDefaultInputDevice();

	// But if the AudioPreferences contain an available input device name, we'll use that
	bool inputNameFound = false;
	for (int i = 0; i < audio->getDeviceCount(); i++) {
		if (audio->getDeviceInfo(i).name.compare(ap.getInputDevName()) == 0) {
			inputNameFound = true;
			inputDevId = i;
			break;
		}
	}

	if (ap.getInputDevName().compare("") == 0 || !inputNameFound)
		ap.setInputDevName(audio->getDeviceInfo(audio->getDefaultInputDevice()).name);

	// Same for output device ID
	auto outputDevId = audio->getDefaultOutputDevice();

	bool outputNameFound = false;
	for (int i = 0; i < audio->getDeviceCount(); i++) {
		if (audio->getDeviceInfo(i).name.compare(ap.getOutputDevName()) == 0) {
			outputNameFound = true;
			outputDevId = i;
			break;
		}
	}

	if (ap.getOutputDevName().compare("") == 0 | !outputNameFound)
		ap.setOutputDevName(audio->getDeviceInfo(audio->getDefaultOutputDevice()).name);

	const auto inputDevInfo = audio->getDeviceInfo(inputDevId);
	const auto maxInputChannels = inputDevInfo.inputChannels;

	auto inParam = new RtAudio::StreamParameters();

	if (maxInputChannels >= 2) {
		inParam->deviceId = inputDevId;
		// The MPC2000XL has only 2 mono input channels
		inParam->nChannels = maxInputChannels > 2 ? 2 : maxInputChannels;
	}
	else {
		// If we don't have enough mono input channels available, we don't bother setting up an input stream
		inParam = nullptr;
	}

	const auto outputDevInfo = audio->getDeviceInfo(outputDevId);
	const auto maxOutputChannels = outputDevInfo.outputChannels;

	auto outParam = new RtAudio::StreamParameters();

	if (maxOutputChannels >= 2) {
		outParam->deviceId = outputDevId;
		// The MPC2000XL has only 10 mono output channels
		outParam->nChannels = maxOutputChannels > 10 ? 10 : maxOutputChannels;
	}
	else {
		// If we don't have enough mono output channels available or sr is unsupported, we don't bother setting up an output stream
		outParam = nullptr;
	}

	// A stream is only opened and started if there is at least a valid output device established
	if (outParam != nullptr) {
		const auto sampleRate = ap.getSampleRate();
		const auto audioFormat = RTAUDIO_FLOAT32;
		LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Opening RtAudio stream with:"));
		LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Buffer size: ") << bufSize);
		RtAudio::StreamOptions options;
		options.numberOfBuffers = 1;
		options.priority = RTAUDIO_SCHEDULE_REALTIME;
		audio->openStream(outParam, inParam, audioFormat, sampleRate, &bufSize, callback, callbackData, &options);
		audio->startStream();
	}
}

void RtAudioWrapper::stopAndCloseStream() {
	audio->stopStream();
	audio->closeStream();
}

#endif