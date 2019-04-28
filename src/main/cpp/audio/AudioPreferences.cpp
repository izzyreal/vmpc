#include "AudioPreferences.hpp"

#include <SerializationException.hpp>

#include <rapidjson/filereadstream.h>
#include <rapidjson/document.h>

#include <algorithm>

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

AudioPreferences::AudioPreferences(const string& filePath) {
	FILE* fp = fopen(filePath.c_str(), "r"); // non-Windows use "r"
	char readBuffer[4096];
	FileReadStream is(fp, readBuffer, sizeof(readBuffer));
	Document d;
	d.ParseStream(is);
	fclose(fp);

	if (!d.HasMember("driverType")) {
		throw SerializationException("driverType", "not found while deserializing");
	}
	if (!d.HasMember("bufferSize")) {
		throw SerializationException("bufferSize", "not found while deserializing");
	}
	if (!d.HasMember("inputDevName")) {
		throw SerializationException("inputDevName", "not found while deserializing");
	}
	if (!d.HasMember("outputDevName")) {
		throw SerializationException("outputDevName", "not found while deserializing");
	}
	
	const auto driverTypeString = d["driverType"].GetString();
	bool driverTypeFound = false;
	
	map<DriverType, string>::iterator it1;
	for (it1 = driverTypeNames.begin(); it1 != driverTypeNames.end(); it1++) {
		if (driverTypeString == it1->second) {
			driverType = it1->first;
			driverTypeFound = true;
			break;
		}
	}

	if (!driverTypeFound) {
		throw new SerializationException("driverType", string(driverTypeString) + " not found in map AudioPreferences::driverTypeNames while deserializing");
	}

	//const auto sampleFormatString = d["sampleFormat"].GetString();
	bool sampleFormatFound = false;

	/*
	map<SampleFormat, string>::iterator it2;
	for (it2 = sampleFormatNames.begin(); it2 != sampleFormatNames.end(); it2++) {
		if (sampleFormatString == it2->second) {
			sampleFormat = it2->first;
			sampleFormatFound = true;
			break;
		}
	}z

	if (!sampleFormatFound) {
		throw new SerializationException("sampleFormat", string(sampleFormatString) + " not found in map AudioPreferences::sampleFormatNames while deserializing");
	}
	*/

	bufferSize = d["bufferSize"].GetUint();
	//inputDevName = d["inputDevName"].GetString();
	outputDevName = d["outputDevName"].GetString();
}

AudioPreferences::AudioPreferences() : AudioPreferences("", "") {
}

map<DriverType, string> AudioPreferences::driverTypeNames =
{
	{ DriverType::ASIO, "asio" },
	{ DriverType::CORE_AUDIO, "core-audio" },
	{ DriverType::DIRECTSOUND, "directsound" },
	{ DriverType::WASAPI, "wasapi" },
	{ DriverType::PULSE_AUDIO, "pulse-audio" }
};

map<SampleFormat, string> AudioPreferences::sampleFormatNames =
{
	{ SampleFormat::FLOAT32, "float32" }
};

const string AudioPreferences::serialize()
{
	StringBuffer s;
	Writer<StringBuffer> w(s);
	w.StartObject();

	w.String("inputDevName");
	w.String(inputDevName);

	w.String("outputDevName");
	w.String(outputDevName);

	w.String("bufferSize");
	w.Uint(bufferSize);

	if (driverTypeNames.find(driverType) == driverTypeNames.end()) {
		throw new SerializationException("driverType", "not found in map AudioPreferences::driverTypeNames");
	}

	w.String("driverType");
	w.String(driverTypeNames[driverType]);
	
	if (sampleFormatNames.find(sampleFormat) == sampleFormatNames.end()) {
		throw new SerializationException("sampleFormat", "not found in map AudioPreferences::sampleFormatNames");
	}

	w.EndObject();
	return s.GetString();
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
	return DriverType::WASAPI;
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
	return 512; // If we will use WASAPI as default driver type, we could probably set this lower, like 512
#endif
}

const string& AudioPreferences::getInputDevName() {
	return inputDevName;
}

const string& AudioPreferences::getOutputDevName() {
	return outputDevName;
}

const unsigned short AudioPreferences::getBufferSize() {
	return bufferSize;
}

const unsigned short AudioPreferences::getSampleRate() {
	return sampleRate;
}

const DriverType& AudioPreferences::getDriverType() {
	return driverType;
}
