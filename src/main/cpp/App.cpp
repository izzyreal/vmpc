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
#include <audio/server/RtAudioServer.hpp>
#include <lcdgui/LayeredScreen.hpp>

#include <math.h>

#include <RtAudio.h>

#if defined(__cplusplus)
extern "C" {
#endif

using namespace moduru;
using namespace mpc;

static Mpc* mpcInstance = nullptr;

static float* tootOut[2]{ new float[512 * 0.125], new float[512 * 0.125] };

/*
static int paTestCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData)
{
	auto as = mpcInstance->getAudioMidiServices().lock()->getRtAudioServer();
	if (as == nullptr) return 0;
	float left[512 * 0.125];
	float right[512 * 0.125];
	float* tootOut[2]{ left, right };
	as->work(nullptr, tootOut, 512 * 0.125, 0, 2);

	float *out = (float*)outputBuffer;
	float* in = (float*)inputBuffer;

	for (int i = 0; i < framesPerBuffer; i++)
	{
		*out++ = tootOut[0][i];
		*out++ = tootOut[1][i];
	}
	return 0;
}
*/

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

	auto as = mpcInstance->getAudioMidiServices().lock()->getRtAudioServer();
	if (as == nullptr) return 0;

	as->work(nullptr, tootOut, 512 * 0.125, 0, 2);

	int counter = 0;
	for (int i = 0; i < 512 * 0.125; i++) {
		buf[counter++] = tootOut[0][i];
		buf[counter++] = tootOut[1][i];
	}
	return 0;
}

int main(int argc, char *argv[]) {


	mpcInstance = new Mpc();
	mpcInstance->init("rtaudio", 44100);
	mpcInstance->getLayeredScreen().lock()->openScreen("sequencer");
	mpcInstance->loadDemoBeat();
	mpcInstance->getAudioMidiServices().lock()->getRtAudioServer()->resizeBuffers(512 * 0.125);

	unsigned int bufsize = 512 * 0.125;
	RtAudio* audio = new RtAudio(RtAudio::WINDOWS_ASIO);
	unsigned int devId = audio->getDefaultOutputDevice();
	for (int i = 0; i < audio->getDeviceCount(); i++) {
		auto name = audio->getDeviceInfo(i).name;
		printf("Name: %s\n", name.c_str());
		if (name.find("Audio 4 DJ") != std::string::npos) {
			devId = i;
			break;
		}
	}
	RtAudio::StreamParameters *outParam = new RtAudio::StreamParameters();

	outParam->deviceId = devId;
	outParam->nChannels = 2;

	audio->openStream(outParam, NULL, RTAUDIO_FLOAT32, 44100,
		&bufsize, rtaudio_callback, NULL);

	audio->startStream();
	/*
	PaStreamParameters inputParameters, outputParameters;
	PaStream *stream = NULL;
	PaError err;

	err = Pa_Initialize();
	if (err != paNoError) printf("initialize error\n");

	for (int i = 0; i < Pa_GetHostApiCount(); i++) {
		auto name = Pa_GetHostApiInfo(i)->name;
		printf("API name: %s\n", name);
	}

	outputParameters.hostApiSpecificStreamInfo = NULL;
	outputParameters.channelCount = 2;
	outputParameters.sampleFormat = paFloat32;
	outputParameters.suggestedLatency = 0;
	outputParameters.device = Pa_GetHostApiInfo(Pa_HostApiTypeIdToHostApiIndex(paWASAPI))->defaultOutputDevice;
	err = Pa_OpenStream(&stream, nullptr, &outputParameters, 44100, 512 * 0.125, paNoFlag, paTestCallback, nullptr);
	if (err != paNoError) printf("openstream error\n");

	err = Pa_StartStream(stream);
	if (err != paNoError) printf("startstream error\n");
	*/
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