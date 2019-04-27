#include "AudioPreferences.hpp"

AudioPreferences::AudioPreferences(const string& inputDevName, const string& outputDevName) {
	this->inputDevName = inputDevName;
	this->outputDevName = outputDevName;
}

AudioPreferences::AudioPreferences(const string& inputDevName, const string& outputDevName, const DriverType& driverType, const unsigned short& bufferSize)
	: AudioPreferences(inputDevName, outputDevName)
{
	this->driverType = driverType;
	this->bufferSize = bufferSize;
}


AudioPreferences::AudioPreferences(const string& inputDevName, const string& outputDevName, const DriverType& driverType, const unsigned short& bufferSize, const SampleFormat& sampleFormat)
	: AudioPreferences(inputDevName, outputDevName)
{
	this->driverType = driverType;
	this->bufferSize = bufferSize;
	this->sampleFormat = sampleFormat;
}


AudioPreferences::~AudioPreferences()
{
}

/**
* We attempt to return a fail-safe, commonly accessible driver type for the active platform
**/
DriverType AudioPreferences::getDefaultDriverType() {
#if defined (__APPLE__)
	return DriverType::CORE_AUDIO;
#elif defined (__linux__)
	return DriverType::PULSE_AUDIO;
#else // assume Windows
	return DriverType::DIRECTSOUND;
#endif
}

/**
* We attempt to return a fail-safe, commonly accessible driver type for the active platform
**/
const unsigned short AudioPreferences::getDefaultBufferSize() {
#if defined (__APPLE__)
	return 128;
#elif defined (__linux__)
	return 1024; // TODO - What is a sane default here?
#else // assume Windows
	return 4096; // If we will use WASAPI as default driver type, we could probably set this lower, like 512
#endif
}
