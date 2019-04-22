#include <stdlib.h>

#include <Gui.hpp>

#include <cairo.h>


#include <SDL.h>        
#include <SDL_image.h>
#include <SDL_render.h>

#include "portaudio.h"

#include "StartUp.hpp"

#include <file/FileUtil.hpp>

#include <Mpc.hpp>
#include <audiomidi/AudioMidiServices.hpp>
#include <audio/server/RtAudioServer.hpp>
#include <lcdgui/LayeredScreen.hpp>

using namespace moduru;
using namespace mpc;

static Mpc* mpcInstance = nullptr;

static int paTestCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData)
{
	auto as = mpcInstance->getAudioMidiServices().lock()->getRtAudioServer();
	if (as == nullptr) return 0;
	float left[512 * 8];
	float right[512 * 8];
	float* tootOut[2]{ left, right };
	as->work(nullptr, tootOut, 512 * 8, 0, 2);

	float *out = (float*)outputBuffer;
	float* in = (float*)inputBuffer;

	for (int i = 0; i < framesPerBuffer; i++)
	{
		*out++ = tootOut[0][i];
		*out++ = tootOut[1][i];
	}
	return 0;
}

int main(int argc, char *argv[]) {

	mpcInstance = new Mpc();
	mpcInstance->init("rtaudio", 44100);
	mpcInstance->getLayeredScreen().lock()->openScreen("sequencer");
	mpcInstance->loadDemoBeat();
	mpcInstance->getAudioMidiServices().lock()->getRtAudioServer()->resizeBuffers(512 * 8);


	PaStreamParameters inputParameters, outputParameters;
	PaStream *stream = NULL;
	PaError err;

	err = Pa_Initialize();
	if (err != paNoError) printf("initialize error\n");

	outputParameters.hostApiSpecificStreamInfo = NULL;
	outputParameters.channelCount = 2;
	outputParameters.sampleFormat = paFloat32;
	outputParameters.suggestedLatency = 0;
	outputParameters.device = Pa_GetHostApiInfo(Pa_HostApiTypeIdToHostApiIndex(paDirectSound))->defaultOutputDevice;
	err = Pa_OpenStream(&stream, nullptr, &outputParameters, 44100, 512 * 8, paNoFlag, paTestCallback, nullptr);
	if (err != paNoError) printf("openstream error\n");

	err = Pa_StartStream(stream);
	if (err != paNoError) printf("startstream error\n");


	auto gui = Gui(mpcInstance);
	gui.initSDL();
    gui.setUserScale(1.0f);
    gui.startLoop();
	gui.destroySDL();
	return 0;
}
