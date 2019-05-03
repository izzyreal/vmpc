#include "RtAudioServer.hpp"

#include <file/File.hpp>

#include <rapidjson/document.h>

RtAudioServer::RtAudioServer(PaStreamCallback* callback, void* callbackData, const string& filePath)
{
    this->callback = callback;
	this->callbackData = callbackData;
	this->filePath = filePath;
	loadPreferences();
}

void RtAudioServer::loadPreferences() {
	try {
		ap = AudioPreferences(filePath);
	}
	catch (const SerializationException& e) {
		const char* msg = e.what();
		printf("%s\n", msg);
		ap = AudioPreferences();
	}
}

void RtAudioServer::storePreferences() {
	moduru::file::File f(filePath, nullptr);
	f.create();
	f.openWrite();
	for (auto c : ap.serialize())
		f.writeByte(c);
	f.close();
}

RtAudioServer::~RtAudioServer()
{
	storePreferences();
	safeDestroy();
}

void RtAudioServer::safeDestroy() {
	if (stream != nullptr) {
		if (!Pa_IsStreamStopped(stream)) {
            Pa_StopStream(stream);
		}
        Pa_CloseStream(stream);
        stream = nullptr;
	}
}

void RtAudioServer::start() {

	safeDestroy();
    
	map<DriverType, PaHostApiTypeId> apis = {
        { DriverType::ASIO, paASIO },
        { DriverType::WASAPI, paWASAPI },
        { DriverType::DIRECTSOUND, paDirectSound },
        { DriverType::CORE_AUDIO, paCoreAudio }
	};

	//audio = new RtAudio(apis[ap.getDriverType()]);
    Pa_Initialize();
    auto info = Pa_GetHostApiInfo(Pa_HostApiTypeIdToHostApiIndex(apis[ap.getDriverType()]));
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

	if (ap.getInputDevName().compare("") == 0 || !inputNameFound)
		ap.setInputDevName(Pa_GetDeviceInfo(info->defaultInputDevice)->name);

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

    
    /* Now we're ready to set up some streams */
    
    auto inParam = new PaStreamParameters();

    const auto inputDevInfo = Pa_GetDeviceInfo(inputDevId);
	const auto maxInputChannels = inputDevInfo->maxInputChannels;

	if (maxInputChannels >= 2) {
		inParam->device = inputDevId;
		// The MPC2000XL has only 2 mono input channels
		inParam->channelCount = maxInputChannels > 2 ? 2 : maxInputChannels;
	}
	else {
		// If we don't have enough mono input channels available, we don't bother setting up an input stream
        inParam = nullptr;
	}

	const auto outputDevInfo = Pa_GetDeviceInfo(outputDevId);
    const auto maxOutputChannels = outputDevInfo->maxOutputChannels;

    auto outParam = new PaStreamParameters();

	if (maxOutputChannels >= 2) {
		outParam->device = outputDevId;
		// The MPC2000XL has only 10 mono output channels
		outParam->channelCount = maxOutputChannels > 10 ? 10 : maxOutputChannels;
	}
	else {
		// If we don't have enough mono output channels available, we don't bother setting up an output stream
		outParam = nullptr;
	}

	// A stream is only opened and started if there is at least a valid output device established
	if (outParam != nullptr) {
		const auto sampleRate = ap.getSampleRate();
		//const auto audioFormat = RTAUDIO_FLOAT32;
		LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Opening portaudio stream..."));
		LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Buffer size: ") << bufSize);
		//RtAudio::StreamOptions options;
		//options.numberOfBuffers = 1;
		//options.priority = RTAUDIO_SCHEDULE_REALTIME;
		//audio->openStream(outParam, inParam, audioFormat, sampleRate, &bufSize, callback, callbackData, &options);
		//audio->startStream();
        Pa_OpenStream(&stream, inParam, outParam, sampleRate, bufSize, paNoFlag, callback, nullptr);
        Pa_StartStream(stream);
	}
}

void RtAudioServer::stopAndCloseStream() {
//    audio->stopStream();
//    audio->closeStream();
    safeDestroy();
}

int RtAudioServer::getBufferSize() {
	return ap.getBufferSize();
}
int RtAudioServer::getSampleRate() {
	return ap.getSampleRate();
}
