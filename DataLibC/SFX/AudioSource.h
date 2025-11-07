//
// Created by 18030 on 2024/12/18.
//

#ifndef DATALIBC_AUDIOSOURCE_H
#define DATALIBC_AUDIOSOURCE_H

#include "DataLibC/Tool/ExportModuleMacro.h"
#include <stddef.h>

typedef struct AudioClip AudioClip;
typedef struct AudioSource AudioSource;

API_MODULE AudioSource* CreateAudioSource();

API_MODULE bool SetAudioClip(AudioSource* source, const AudioClip* clip);

API_MODULE bool IsAudioPlaying(const AudioSource* source);

API_MODULE void SetAudioPause(AudioSource* source);

API_MODULE void SetAudioPlay(AudioSource* source);

API_MODULE const AudioClip* GetAudioClip(const AudioSource* source);

API_MODULE void SetAudioLoop(AudioSource* source, bool loop);

API_MODULE bool IsAudioLoop(const AudioSource* source);

#endif //DATALIBC_AUDIOSOURCE_H
