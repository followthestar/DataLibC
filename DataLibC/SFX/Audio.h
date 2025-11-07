//
// Created by 18030 on 2024/12/14.
//

#ifndef DATALIBC_AUDIO_H
#define DATALIBC_AUDIO_H

#include "DataLibC/Tool/ExportModuleMacro.h"
#include "AudioClip.h"


/// Used to initialize audio system
/// \return True if succeed, otherwise false
API_MODULE bool AudioInit();

/// Used to clean up audio system resources
API_MODULE void AudioClear();

/// Create a AudioClip object which refers to a sfx source
/// \param filePath The path to your sfx file
/// \return A new AudioClip object
API_MODULE AudioClip CreateAudioClip(const char* filePath);

/// Play given AudioClip once. This won't interrupt the sfx being played
/// \param [in] clip The clip will be played
API_MODULE void PlayOneShot(const AudioClip* clip);


#endif //DATALIBC_AUDIO_H
