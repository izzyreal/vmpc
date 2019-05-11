#include "RtAudioWrapper.hpp"

#if AUDIO_LIBRARY == RTAUDIO

#include <file/File.hpp>

#include <rapidjson/document.h>

#include <System.hpp>

RtAudioWrapper::RtAudioWrapper(const RtAudioCallback& callback, void* callbackData, const string& filePath)
	: AbstractAudioWrapper(callbackData, filePath)
{
	this->callback = callback;
}

RtAudioWrapper::~RtAudioWrapper()
{
}

void RtAudioWrapper::prepare() {

	map<DriverType, RtAudio::Api> apis = {
	{ DriverType::ASIO, RtAudio::WINDOWS_ASIO },
	{ DriverType::WASAPI, RtAudio::WINDOWS_WASAPI },
	{ DriverType::DIRECTSOUND, RtAudio::WINDOWS_DS },
	{ DriverType::CORE_AUDIO, RtAudio::MACOSX_CORE }
	};

	audio = make_shared<RtAudio>(apis[ap.getDriverType()]);
	
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

	if (maxInputChannels >= 2) {
		inParam = make_shared<RtAudio::StreamParameters>();
		inParam->deviceId = inputDevId;
		// The MPC2000XL has only 2 mono input channels
		inParam->nChannels = maxInputChannels > 2 ? 2 : maxInputChannels;
	}

	const auto outputDevInfo = audio->getDeviceInfo(outputDevId);
	const auto maxOutputChannels = outputDevInfo.outputChannels;

	if (maxOutputChannels >= 2) {
		outParam = make_shared<RtAudio::StreamParameters>();
		outParam->deviceId = outputDevId;
		// The MPC2000XL has only 10 mono output channels
		outParam->nChannels = maxOutputChannels > 10 ? 10 : maxOutputChannels;
	}
}

void RtAudioWrapper::start() {

	unsigned int bufSize = ap.getBufferSize();

	// A stream is only opened and started if there is at least a valid output device established
	if (outParam) {
		const auto sampleRate = ap.getSampleRate();
		const auto audioFormat = RTAUDIO_FLOAT32;

		LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Opening RtAudio stream..."));
		LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Buffer size     : ") << bufSize);
		LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Output dev id   : ") << outParam->deviceId);
		LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Output dev name : ") << LOG4CPLUS_C_STR_TO_TSTRING(audio->getDeviceInfo(outParam->deviceId).name));

		if (inParam) {
			LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Input dev id    : ") << inParam->deviceId);
			LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Input dev name  : ") << LOG4CPLUS_C_STR_TO_TSTRING(audio->getDeviceInfo(inParam->deviceId).name));
		}

		RtAudio::StreamOptions options;
		options.numberOfBuffers = 1;
		options.priority = RTAUDIO_SCHEDULE_REALTIME;
		audio->openStream(outParam.get(), inParam.get(), audioFormat, sampleRate, &bufSize, callback, callbackData, &options);
		audio->startStream();
	}
}

void RtAudioWrapper::stop() {
	LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Stopping RtAudioWrapper..."));
	try {
		// Stop the stream
		if (audio->isStreamRunning()) {
			LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Trying to stop RtAudioStream..."));
			audio->stopStream();
			LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("RtAudioStream stopped"));
		} else {
			LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("RtAudioStream is not running, nothing to stop"));
		}
	}
	catch (const RtAudioError &error) {
		auto msg = error.getMessage();
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("Error while stopping RtAudioStream"));
		LOG4CPLUS_ERROR_STR(logger, LOG4CPLUS_STRING_TO_TSTRING(msg));
	}

	try {
		// Close the stream
		if (audio->isStreamOpen()) {
			LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Trying to close RtAudioStream..."));
			audio->closeStream();
			LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("RtAudioStream closed"));
		}
		else {
			LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("RtAudioStream is not open, nothing to close"));
		}
	}
	catch (const std::exception& error) {
		auto msg = error.what();
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("Error while closing RtAudioStream"));
		LOG4CPLUS_ERROR_STR(logger, LOG4CPLUS_STRING_TO_TSTRING(msg));
	}
	catch (const RtAudioError &error) {
		auto msg = error.getMessage();
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("Error while closing RtAudioStream"));
		LOG4CPLUS_ERROR_STR(logger, LOG4CPLUS_STRING_TO_TSTRING(msg));
	}
}

const int RtAudioWrapper::getInputCount() {
	if (inParam) {
		return inParam->nChannels;
	}
	return 0;
}

const int RtAudioWrapper::getStereoOutputCount() {
	if (outParam) {
		return outParam->nChannels;
	}
	return 0;
}

#endif