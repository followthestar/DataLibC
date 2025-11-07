//
// Created by 18030 on 2024/12/14.
//

#include "Audio.h"
#include "AudioState.h"
#include "DataLibC/Tool/LogMacro.h"
#include <stdio.h>

static AudioClip error = {INVALID, 0, 0};

bool AudioInit()
{
    return InitAudioSystem_Internal();
}

void AudioClear()
{
    CleanUpAudioSystem_Internal();
}

AudioClip CreateAudioClip(const char* filePath)
{
    FILE* file = fopen(filePath, "rb");
    if (!file)
    {
        RuntimeErrorLog(OpenFileFailed, "Failed to open sfx file at path %s!", filePath);
        return error;
    }
    AudioClip out;
    CreateAudioClip_Internal(file, &out);
    if (out.format == INVALID)
    {
        RuntimeErrorLog(InvalidFormat, "Invalid sfx file format at path %s!", filePath);
    }

    fclose(file);
    return out;
}

void PlayOneShot(const AudioClip* clip)
{
    if (clip->format == INVALID)
    {
        RuntimeErrorLog(IllegalState, "Uninitialized AudioClip!");
        return;
    }
    PlayOneShot_Internal(clip);
}
