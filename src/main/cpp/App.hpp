#pragma once

#include <memory>

using namespace std;

class AudioPreferencesDialog {
};

class RtAudioAudioPreferencesDialog : public AudioPreferencesDialog {
public:
};

class AudioPreferencesDialogFactory {
public:
	static shared_ptr<AudioPreferencesDialog> createAudioPreferencesDialog
};
