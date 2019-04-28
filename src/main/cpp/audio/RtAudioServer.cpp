#include "RtAudioServer.hpp"

#include <file/File.hpp>

#include <rapidjson/document.h>

RtAudioServer::RtAudioServer(const RtAudioCallback& callback, const string& filePath)
{
	this->callback = callback;
	this->ap = ap;
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

void RtAudioServer::start() {

	safeDestroy();

	map<DriverType, RtAudio::Api> apis = {
	{ DriverType::ASIO, RtAudio::WINDOWS_ASIO },
	{ DriverType::WASAPI, RtAudio::WINDOWS_WASAPI },
	{ DriverType::DIRECTSOUND, RtAudio::WINDOWS_DS },
	{ DriverType::CORE_AUDIO, RtAudio::MACOSX_CORE },
	{ DriverType::PULSE_AUDIO, RtAudio::LINUX_PULSE }
	};

	audio = new RtAudio(apis[ap.getDriverType()]);
	const auto defaultInputDeviceId = audio->getDefaultInputDevice();
	const auto defaultOutputDeviceId = audio->getDefaultOutputDevice();
	const auto defaultInputDevice = audio->getDeviceInfo(defaultInputDeviceId).name;
	const auto defaultOutputDevice = audio->getDeviceInfo(defaultOutputDeviceId).name;

	unsigned int bufSize = ap.getBufferSize();
	auto devId = audio->getDefaultOutputDevice();

	for (int i = 0; i < audio->getDeviceCount(); i++) {
		if (audio->getDeviceInfo(i).name.compare(ap.getOutputDevName()) == 0) {
			devId = i;
			break;
		}
	}

	RtAudio::StreamParameters *outParam = new RtAudio::StreamParameters();

	outParam->deviceId = devId;
	outParam->nChannels = 2;

	const auto sampleRate = ap.getSampleRate();
	const auto audioFormat = RTAUDIO_FLOAT32;
	LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Opening RtAudio stream with:"));
	LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Buffer size: ") << bufSize);
	audio->openStream(outParam, NULL, audioFormat, sampleRate, &bufSize, callback, (void*)bufSize);
	audio->startStream();
}

void RtAudioServer::stopAndCloseStream() {
	audio->stopStream();
	audio->closeStream();
}

int RtAudioServer::getBufferSize() {
	return ap.getBufferSize();
}
int RtAudioServer::getSampleRate() {
	return ap.getSampleRate();
}
