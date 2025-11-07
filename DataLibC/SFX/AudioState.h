//
// Created by 18030 on 2024/12/15.
//

#include <stdio.h>

#ifndef DATALIBC_AUDIOSTATE_H
#define DATALIBC_AUDIOSTATE_H

typedef struct AudioClip AudioClip;
typedef struct AudioSource AudioSource;
typedef struct IXAudio2SourceVoice IXAudio2SourceVoice;

bool InitAudioSystem_Internal();

void CleanUpAudioSystem_Internal();

void CreateAudioClip_Internal(FILE* file, AudioClip* out);

void PlayOneShot_Internal(const AudioClip* clip);

void SetClip_Internal(AudioSource* source, const AudioClip* clip);

void AudioSourcePlay_Internal(AudioSource* source);

void AudioSourcePause_Internal(AudioSource* source);

void AudioSourceResume_Internal(AudioSource* source);

#endif //DATALIBC_AUDIOSTATE_H
