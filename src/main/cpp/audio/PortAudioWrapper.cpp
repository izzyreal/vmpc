#include "PortAudioWrapper.hpp"

#include <file/File.hpp>

#include <rapidjson/document.h>

PortAudioWrapper::PortAudioWrapper(PaStreamCallback* callback, void* callbackData, const string& filePath) 
	: AbstractAudioWrapper(callbackData, filePath)
{
	this->callback = callback;
}

PortAudioWrapper::~PortAudioWrapper()
{
	storePreferences();
	safeDestroy();
}

void PortAudioWrapper::safeDestroy() {
	if (stream != nullptr) {
		if (!Pa_IsStreamStopped(stream)) {
			Pa_StopStream(stream);
		}
		Pa_CloseStream(stream);
		stream = nullptr;
	}
}

void PortAudioWrapper::start() {

	safeDestroy();

	map<DriverType, PaHostApiTypeId> apis = {
		{ DriverType::ASIO, paASIO },
		{ DriverType::WASAPI, paWASAPI },
		{ DriverType::DIRECTSOUND, paDirectSound },
		{ DriverType::CORE_AUDIO, paCoreAudio }
	};

	const auto error = Pa_Initialize();

	if (error) {
		const auto msg = Pa_GetErrorText(error);
		LOG4CPLUS_ERROR(logger, msg);
	}

	const auto paApiTypeId = apis[ap.getDriverType()];

	LOG4CPLUS_TRACE(logger, LOG4CPLUS_TEXT("driverType : ") << (int)ap.getDriverType());
	LOG4CPLUS_TRACE(logger, LOG4CPLUS_TEXT("paApiTypeId : ") << (int)paApiTypeId);

	const PaHostApiIndex hostApiIndex = Pa_HostApiTypeIdToHostApiIndex(paApiTypeId);

	if (hostApiIndex < 0) {
		const auto msg = Pa_GetErrorText(hostApiIndex);
		LOG4CPLUS_ERROR(logger, msg);
	}

	LOG4CPLUS_TRACE(logger, LOG4CPLUS_TEXT("hostApiIndex : ") << (int)hostApiIndex);

	const auto info = Pa_GetHostApiInfo(hostApiIndex);
	if (info == NULL) {
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("api info is NULL!"));
	}

	unsigned int bufSize = ap.getBufferSize();

	// We start off with an input device ID that always works
	auto inputDevId = info->defaultInputDevice;

	// But if the AudioPreferences contain an available input device name, we'll use that
	bool inputNameFound = false;
	for (int i = 0; i < Pa_GetDeviceCount(); i++) {

		if (strcmp(Pa_GetDeviceInfo(i)->name, ap.getInputDevName().c_str()) == 0) {
			inputNameFound = true;
			inputDevId = i;
			break;
		}
	}

	if (ap.getInputDevName().compare("") == 0 || !inputNameFound) {
		LOG4CPLUS_TRACE(logger, LOG4CPLUS_TEXT("info defaultInputDevice : ") << (int)info->defaultInputDevice);
		auto candidate = Pa_GetDeviceInfo(info->defaultInputDevice);
		ap.setInputDevName(Pa_GetDeviceInfo(info->defaultInputDevice)->name);
	}

	// Same for output device ID
	auto outputDevId = info->defaultOutputDevice;

	bool outputNameFound = false;
	for (int i = 0; i < Pa_GetDeviceCount(); i++) {

		if (strcmp(Pa_GetDeviceInfo(i)->name, ap.getOutputDevName().c_str()) == 0) {
			outputNameFound = true;
			outputDevId = i;
			break;
		}
	}

	if (ap.getOutputDevName().compare("") == 0 || !outputNameFound)
		ap.setOutputDevName(Pa_GetDeviceInfo(info->defaultOutputDevice)->name);

	// Now we're ready to set up some streams

	auto inParam = new PaStreamParameters();

	const auto inputDevInfo = Pa_GetDeviceInfo(inputDevId);
	const auto maxInputChannels = inputDevInfo->maxInputChannels;

	if (maxInputChannels >= 2) {
		inParam->device = inputDevId;
		// The MPC2000XL has only 2 mono input channels
		inParam->channelCount = maxInputChannels > 2 ? 2 : maxInputChannels;
		inParam->hostApiSpecificStreamInfo = NULL;
		inParam->sampleFormat = paFloat32;
		inParam->suggestedLatency = Pa_GetDeviceInfo(inputDevId)->defaultLowInputLatency;
	}
	else {
		// If we don't have enough mono input channels available, we don't bother setting up an input stream
		inParam = nullptr;
	}

	const auto outputDevInfo = Pa_GetDeviceInfo(outputDevId);
	const auto maxOutputChannels = outputDevInfo->maxOutputChannels;
	LOG4CPLUS_TRACE(logger, LOG4CPLUS_TEXT("pa output dev max channels : ") << (int)maxOutputChannels);

	auto outParam = new PaStreamParameters();

	if (maxOutputChannels >= 2) {
		outParam->device = outputDevId;
		// The MPC2000XL has only 10 mono output channels
		outParam->channelCount = maxOutputChannels > 10 ? 10 : maxOutputChannels;
		outParam->hostApiSpecificStreamInfo = NULL;
		outParam->sampleFormat = paFloat32;
		outParam->suggestedLatency = Pa_GetDeviceInfo(outputDevId)->defaultLowOutputLatency;
	}
	else {
		// If we don't have enough mono output channels available, we don't bother setting up an output stream
		outParam = nullptr;
	}

	// A stream is only opened and started if there is at least a valid output device established
	if (outParam != nullptr) {
		const auto sampleRate = ap.getSampleRate();
		LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Opening portaudio stream..."));
		LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Sample rate: ") << sampleRate);
		LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Buffer size: ") << bufSize);
		//logError(Pa_OpenStream(&stream, inParam, outParam, sampleRate, bufSize, paNoFlag, callback, callbackData));
		logError(Pa_OpenStream(&stream, nullptr, outParam, sampleRate, bufSize, paNoFlag, callback, callbackData));
		logError(Pa_StartStream(stream));
	}
}

void PortAudioWrapper::logError(const PaError &e) {
	if (e != paNoError) {
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("portaudio error: "));
		auto msg = Pa_GetErrorText(e);
		LOG4CPLUS_ERROR_STR(logger, LOG4CPLUS_STRING_TO_TSTRING(msg));
	}
}

void PortAudioWrapper::stopAndCloseStream() {
	//    audio->stopStream();
	//    audio->closeStream();
	safeDestroy();
}
