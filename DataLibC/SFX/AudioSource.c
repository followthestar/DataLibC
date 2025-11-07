//
// Created by 18030 on 2024/12/18.
//

#include <assert.h>
#include <malloc.h>
#include "AudioSource.h"
#include "AudioClip.h"
#include "AudioState.h"
#include "DataLibC/Tool/LogMacro.h"

typedef struct AudioSource
{
    size_t sourceID;
    AudioClip clip;
    bool isPlaying;
    bool loop;
} AudioSource;

size_t _GetAudioSourceID(const AudioSource* source)
{
    return source->sourceID;
}

void _SetAudioSourceID(AudioSource* source, size_t id)
{
    source->sourceID = id;
}

void _SetAudioSourceIsPlaying(AudioSource* source, bool value)
{
    source->isPlaying = value;
}

AudioSource* CreateAudioSource()
{
    AudioSource* newSource = malloc(sizeof(AudioSource));
    newSource->sourceID = 0;
    newSource->isPlaying = false;
    newSource->loop = false;
    return newSource;
}

bool SetAudioClip(AudioSource* source, const AudioClip* clip)
{
    if (!clip)
        return false;
    source->clip = *clip;
    SetClip_Internal(source, clip);
    return true;
}

void SetAudioPlay(AudioSource* source)
{
    if (source->sourceID == 0)
    {
        RuntimeErrorLog(InvalidParameterType, "Please set a audio clip first!");
        return;
    }
    AudioSourcePlay_Internal(source);
}

const AudioClip* GetAudioClip(const AudioSource* source)
{
    return &source->clip;
}

bool IsAudioPlaying(const AudioSource* source)
{
    return source->isPlaying;
}

void SetAudioPause(AudioSource* source)
{
    if (source->sourceID == 0)
    {
        RuntimeErrorLog(InvalidParameterType, "Please set a audio clip first!");
        return;
    }
    AudioSourcePause_Internal(source);
}

void SetAudioLoop(AudioSource* source, bool loop)
{
    source->loop = loop;
}

bool IsAudioLoop(const AudioSource* source)
{
    return source->loop;
}

