/**
* Encapsulates all the parameters needed to open an audio stream
**/

#pragma once

#ifndef AUDIOPREFERENCES
#define AUDIOPREFERENCES

#include <string>

using namespace std;

// We enumerate some of the most common driver types
enum DriverType {
	ASIO, CORE_AUDIO, DIRECTSOUND, WASAPI, PULSE_AUDIO
};

// New sample formats can be added when required
enum SampleFormat {
	FLOAT32
};

class AudioPreferences {

public:
	AudioPreferences(const string& inputDevName, const string& outputDevName);
	AudioPreferences(const string& inputDevName, const string& outputDevName, const DriverType& driverType, const unsigned short& bufferSize);
	AudioPreferences(const string& inputDevName, const string& outputDevName, const DriverType& driverType, const unsigned short& bufferSize, const SampleFormat& sampleFormat);

private:
	DriverType driverType = getDefaultDriverType();
	unsigned short bufferSize = getDefaultBufferSize(); 	// In frames

private:
	// We will open audio devices based on these names, so they will need a 100% match with
	// how the host is acquainted with them. Empty string means inactive device. For ASIO
	// the input and output device names should be the same or an error will be thrown during
	// construction.
	string inputDevName = "";
	string outputDevName = "";

private:
	SampleFormat sampleFormat = FLOAT32;

private:
	DriverType getDefaultDriverType();
	const unsigned short getDefaultBufferSize();
};

#endif