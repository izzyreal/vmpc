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

#include <audio/PortAudioWrapper.hpp>
#include <audio/RtAudioWrapper.hpp>
#include <audio/AudioPreferences.hpp>
#include <audio/AudioWrapperConfig.hpp>

using namespace moduru;
using namespace moduru::file;
using namespace mpc;

using namespace log4cplus;

struct CallbackData {
	mpc::Mpc* mpc;
};

#if AUDIO_LIBRARY == PORTAUDIO

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

PortAudioWrapper instantiateAudioWrapper(CallbackData data, const string& preferencesFilePath) {

	return PortAudioWrapper(pa_callback, (void*)&data, preferencesFilePath);
}

#endif // AUDIO_LIBRARY == PORTAUDIO

#if AUDIO_LIBRARY == RTAUDIO

static int
rtaudio_callback(
	void			*outbuf,
	void			*inbuf,
	unsigned int		nFrames,
	double			streamtime,
	RtAudioStreamStatus	status,
	void			*userData)
{
	float* outbufFloat = (float*)outbuf;
	float* inbufFloat = (float*)inbuf;

	auto callbackData = (CallbackData*)userData;
	auto as = callbackData->mpc->getAudioMidiServices().lock()->getExternalAudioServer();
	if (as == nullptr) return 0;
	auto logger = log4cplus::Logger::getRoot();
	LOG4CPLUS_TRACE(logger, LOG4CPLUS_TEXT("RtAudio callback nFrames : ") << nFrames);
	auto bufSize = as->getBufferSize();
	const auto asInputCount = as->getActiveInputs().size() * 2;
	const auto asOutputCount = as->getActiveOutputs().size() * 2;
	as->work(inbufFloat, outbufFloat, bufSize, asInputCount, asOutputCount);
	return 0;
}

RtAudioWrapper instantiateAudioWrapper(CallbackData& data, const string& preferencesFilePath) {
	return RtAudioWrapper(rtaudio_callback, (void*)&data, preferencesFilePath);
}

#endif

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
    
	LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("======================"));
	LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Starting VMPC2000XL..."));
	LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("======================"));

	// Then we set up the audio server
	const auto preferencesFilePath = FileUtil::joinPath(mpc::StartUp::home, "vMPC", "audio_preferences.json");

	// and instantiate the MPC
    Mpc mpc;
	CallbackData callbackData{ &mpc };

	auto audioWrapper = instantiateAudioWrapper(callbackData, preferencesFilePath);

	audioWrapper.prepare();

	const auto inputCount = audioWrapper.getInputCount();
	const auto outputCount = audioWrapper.getStereoOutputCount();

	mpc.init(audioWrapper.getSampleRate(), inputCount / 2, outputCount / 2);
	mpc.getLayeredScreen().lock()->openScreen("sequencer");
	mpc.loadDemoBeat();
	// We make the MPC audio engine aware of the buffer size
	mpc.getAudioMidiServices().lock()->getExternalAudioServer()->resizeBuffers(audioWrapper.getBufferSize());
	
	// Now portaudio can start its callbacks
    audioWrapper.start();

	// With the audio engine running, we instantiate the graphics side of things
	auto gui = Gui(&mpc);
	gui.initSDL();
    gui.setUserScale(Gui::LARGE);

	LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("VMPC2000XL is ready to use"));

	gui.startLoop();

	LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Shutting down VMPC2000XL..."));

	// If the GUI loop has stopped, we try to clean up after ourselves

	audioWrapper.storePreferences();

	mpc.getAudioMidiServices().lock()->getExternalAudioServer()->stop();

	audioWrapper.stop();
	
	gui.destroySDL();

	LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("=================="));
	LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Shutdown completed"));
	LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("=================="));

	return 0;
}
