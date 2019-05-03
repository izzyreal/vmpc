/**
* Encapsulates all the parameters needed to open an audio stream
**/

#pragma once

#ifndef AUDIOPREFERENCES
#define AUDIOPREFERENCES

#include <Serializable.hpp>

#include <string>
#include <map>

using namespace std;

// We enumerate some of the most common driver types
enum DriverType {
	ASIO, CORE_AUDIO, DIRECTSOUND, WASAPI
};

// New sample formats can be added when required
enum SampleFormat {
	FLOAT32
};

class AudioPreferences : public moduru::Serializable {

public:
	AudioPreferences(const string& inputDevName, const string& outputDevName);
	AudioPreferences(const string& inputDevName, const string& outputDevName, const DriverType& driverType, const unsigned short& bufferSize);
	AudioPreferences(const string& inputDevName, const string& outputDevName, const DriverType& driverType, const unsigned short& bufferSize, const SampleFormat& sampleFormat);
	AudioPreferences(const string& settingsJson);
	AudioPreferences();

public:
	const string serialize() override;

private:
	// Maps are used for (de)serialization
	static map<DriverType, string> driverTypeNames;
	static map<SampleFormat, string> sampleFormatNames;

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
	unsigned short sampleRate = 44100;

private:
	DriverType getDefaultDriverType();
	const unsigned short getDefaultBufferSize();

public:
	const string& getInputDevName();
	const string& getOutputDevName();
	void setInputDevName(const string&);
	void setOutputDevName(const string&);

	const unsigned short getBufferSize();
	const unsigned short getSampleRate();
	const DriverType& getDriverType();
};

#endif
