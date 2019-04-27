#include <stdlib.h>

#include <Gui.hpp>

#include <cairo.h>


#include <SDL.h>        
#include <SDL_image.h>
#include <SDL_render.h>

//#include "portaudio.h"

#include "StartUp.hpp"

#include <file/FileUtil.hpp>

#include <Mpc.hpp>
#include <audiomidi/AudioMidiServices.hpp>
#include <audio/server/ExternalAudioServer.hpp>
#include <lcdgui/LayeredScreen.hpp>

#include <math.h>

#include <RtAudio.h>

#include <audio/AudioPreferences.hpp>

#if defined(__cplusplus)
extern "C" {
#endif

using namespace moduru;
using namespace mpc;

static Mpc* mpcInstance = nullptr;


static int
rtaudio_callback(
	void			*outbuf,
	void			*inbuf,
	unsigned int		nFrames,
	double			streamtime,
	RtAudioStreamStatus	status,
	void			*userdata)
{
	float	*buf = (float*)outbuf;
	unsigned int remainFrames;

	auto as = mpcInstance->getAudioMidiServices().lock()->getExternalAudioServer();
	if (as == nullptr) return 0;
	unsigned int bufSize = (unsigned int) userdata;
	//unsigned int bufSize = 4096;
	static float* tootOut[2]{ new float[bufSize], new float[bufSize] };
	as->work(nullptr, tootOut, bufSize, 0, 2);

	int counter = 0;
	for (int i = 0; i < bufSize; i++) {
		buf[counter++] = tootOut[0][i];
		buf[counter++] = tootOut[1][i];
	}
	return 0;
}

int main(int argc, char *argv[]) {

	AudioPreferences ap = AudioPreferences();

	map<DriverType, RtAudio::Api> apis = {
		{ DriverType::ASIO, RtAudio::WINDOWS_ASIO },
		{ DriverType::WASAPI, RtAudio::WINDOWS_WASAPI },
		{ DriverType::DIRECTSOUND, RtAudio::WINDOWS_DS },
		{ DriverType::CORE_AUDIO, RtAudio::MACOSX_CORE },
		{ DriverType::PULSE_AUDIO, RtAudio::LINUX_PULSE }
	};
	
	RtAudio* audio = new RtAudio(apis[ap.getDriverType()]);
	const auto defaultInputDeviceId = audio->getDefaultInputDevice();
	const auto defaultOutputDeviceId = audio->getDefaultOutputDevice();
	const auto defaultInputDevice = audio->getDeviceInfo(defaultInputDeviceId).name;
	const auto defaultOutputDevice = audio->getDeviceInfo(defaultOutputDeviceId).name;

	mpcInstance = new Mpc();
	mpcInstance->init("rtaudio", ap.getSampleRate());
	mpcInstance->getLayeredScreen().lock()->openScreen("sequencer");
	mpcInstance->loadDemoBeat();
	mpcInstance->getAudioMidiServices().lock()->getExternalAudioServer()->resizeBuffers(ap.getBufferSize());

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

	audio->openStream(outParam, NULL, audioFormat, sampleRate, &bufSize, rtaudio_callback, (void*)bufSize);

	audio->startStream();

	auto gui = Gui(mpcInstance);
	gui.initSDL();
    gui.setUserScale(1.0f);
    gui.startLoop();
	gui.destroySDL();
	audio->stopStream();
	audio->closeStream();
	delete audio;
	delete mpcInstance;
	return 0;
}

#if defined(__cplusplus)
}
#endif