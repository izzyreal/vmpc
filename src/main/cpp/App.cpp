#include <stdlib.h>

#include <Gui.hpp>

#include <cairo.h>


#include <SDL.h>        
#include <SDL_image.h>
#include <SDL_render.h>

#include <SerializationException.hpp>

#include "StartUp.hpp"

#include <file/FileUtil.hpp>
#include <file/File.hpp>

#include <Mpc.hpp>
#include <StartUp.hpp>
#include <audiomidi/AudioMidiServices.hpp>
#include <audio/server/ExternalAudioServer.hpp>
#include <lcdgui/LayeredScreen.hpp>

#include <math.h>

#include <RtAudio.h>

#include <audio/RtAudioServer.hpp>
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

	const auto preferencesFilePath = moduru::file::FileUtil::join_with_separator(mpc::StartUp::home, "vMPC", "audio_preferences.json");

	RtAudioServer* audioServer = new RtAudioServer(rtaudio_callback, preferencesFilePath);

	mpcInstance = new Mpc();
	mpcInstance->init("rtaudio", audioServer->getSampleRate());
	mpcInstance->getLayeredScreen().lock()->openScreen("sequencer");
	mpcInstance->loadDemoBeat();
	mpcInstance->getAudioMidiServices().lock()->getExternalAudioServer()->resizeBuffers(audioServer->getBufferSize());

	auto gui = Gui(mpcInstance);
	gui.initSDL();
    gui.setUserScale(1.0f);

	audioServer->start();

	gui.startLoop();
	gui.destroySDL();
	
	delete audioServer;
	delete mpcInstance;
	return 0;
}

#if defined(__cplusplus)
}
#endif