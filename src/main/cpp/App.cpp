#include <stdlib.h>

#include <log4cplus/log4cplus.h>

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

#include <portaudio.h>

#include <audio/RtAudioServer.hpp>
#include <audio/AudioPreferences.hpp>

using namespace moduru;
using namespace moduru::file;
using namespace mpc;

using namespace log4cplus;

static Mpc* mpcInstance = nullptr;

struct CallbackData {
	mpc::Mpc* mpc;
};

static int pa_callback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData)
{
    auto callbackData = (CallbackData*)userData;
    auto as = callbackData->mpc->getAudioMidiServices().lock()->getExternalAudioServer();
    if (as == nullptr) return 0;
    float* in = (float*)inputBuffer;
    float *out = (float*)outputBuffer;
    auto bufSize = as->getBufferSize();
    as->work(in, out, bufSize, 2, 5);
    return 0;
}
    
int main(int argc, char *argv[]) {
	
	// First we set up the logger
	const auto logFilePath = FileUtil::joinPath(mpc::StartUp::home, "vMPC", "vmpc2000xl.log");
		
	log4cplus::Initializer initializer;
	log4cplus::BasicConfigurator config;
	config.configure();
	auto logger = log4cplus::Logger::getRoot();
	SharedAppenderPtr fileAppender(new FileAppender(LOG4CPLUS_STRING_TO_TSTRING(logFilePath), std::ios_base::app));
	fileAppender->setLayout(make_unique<TTCCLayout>());
	logger.addAppender(fileAppender);

    logger.setLogLevel(INFO_LOG_LEVEL);
//    logger.setLogLevel(TRACE_LOG_LEVEL);
    
	LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Starting VMPC2000XL..."));

	// Then we set up the audio server
	const auto preferencesFilePath = FileUtil::joinPath(mpc::StartUp::home, "vMPC", "audio_preferences.json");

	// and instantiate the MPC
	mpcInstance = new Mpc();
	CallbackData callbackData{ mpcInstance };
	RtAudioServer* audioServer = new RtAudioServer(pa_callback, (void*)&callbackData, preferencesFilePath);
	mpcInstance->init("rtaudio", audioServer->getSampleRate());
	mpcInstance->getLayeredScreen().lock()->openScreen("sequencer");
	mpcInstance->loadDemoBeat();

	// We make the MPC audio engine aware of the buffer size
	mpcInstance->getAudioMidiServices().lock()->getExternalAudioServer()->resizeBuffers(audioServer->getBufferSize());
	
	// Now the audio server can start its callbacks
	
	try {
		audioServer->start();
	}
	catch (const PaError& e) {
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("Error while starting audio engine: ") << Pa_GetErrorText(e));
	}

	// With the audio engine running, we instantiate the graphics side of things
	auto gui = Gui(mpcInstance);
	gui.initSDL();
    gui.setUserScale(1.0f);
	gui.startLoop();
	
	// If the GUI loop has stopped, we try to clean up after ourselves	
	gui.destroySDL();
	delete audioServer;
	delete mpcInstance;

	return 0;
}
