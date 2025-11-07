//
// Created by 18030 on 2024/12/15.
//

#include "AudioState.h"
#include "AudioClip.h"
#include "AudioSource.h"
#include <xaudio2.h>
#include <stdio.h>
#include <assert.h>
#include <pthread.h>

#include "DataLibC/DS/Queue/Queue.h"


#define WAV_SIGNATURE   "RIFF"
#define WAV_CHUNKTYPE   "WAVE"
#define FLAC_SIGNATURE  "fLaC"

typedef struct TempVoiceCallback
{
    IXAudio2VoiceCallback callback;
    IXAudio2SourceVoice* pSourceVoice;
    AudioSource* source;
    bool autoClear;
} TempVoiceCallback;

static IXAudio2* gIXAudio2 = NULL;
static IXAudio2MasteringVoice* gIXAudio2MasteringVoice = NULL;

static Queue gVoiceClearQueue;

pthread_mutex_t gAudioSourceMutex = PTHREAD_MUTEX_INITIALIZER;

static void AudioVoiceQueueClear()
{
    while (!Queue_Empty(&gVoiceClearQueue))
    {
        TempVoiceCallback** pCallback = Queue_Front(&gVoiceClearQueue);
        (*pCallback)->pSourceVoice->lpVtbl->DestroyVoice((*pCallback)->pSourceVoice);
        free(*pCallback);
        Queue_Dequeue(&gVoiceClearQueue);
    }
}

#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT  ' tmf'
#define fourccWAVE 'EVAW'

XAUDIO2_BUFFER CreateBufferFromClip(const AudioClip* clip);

size_t _GetAudioSourceID(const AudioSource* source);

void _SetAudioSourceID(AudioSource* source, size_t id);

void _SetAudioSourceIsPlaying(AudioSource* source, bool value);


static void OnVoiceProcessingPassStart(IXAudio2VoiceCallback* This, UINT32 BytesRequired) { }

static void OnVoiceProcessingPassEnd(IXAudio2VoiceCallback* This) { }

static void OnStreamEnd(IXAudio2VoiceCallback* This)
{
    TempVoiceCallback* pCallback = (TempVoiceCallback*) This;

    if (pCallback->pSourceVoice)
    {
        pCallback->pSourceVoice->lpVtbl->FlushSourceBuffers(pCallback->pSourceVoice);
        if (pCallback->source)
        {
            AudioSourcePause_Internal(pCallback->source);
            auto buffer = CreateBufferFromClip(GetAudioClip(pCallback->source));
            pCallback->pSourceVoice->lpVtbl->SubmitSourceBuffer(pCallback->pSourceVoice, &buffer, 0);

        }
        if (!pCallback->autoClear)
        {
            if (IsAudioLoop(pCallback->source))
                AudioSourcePlay_Internal(pCallback->source);
            return;
        }

        pCallback->pSourceVoice->lpVtbl->Stop(pCallback->pSourceVoice, 0, XAUDIO2_COMMIT_NOW);
        pthread_mutex_lock(&gAudioSourceMutex);
        Queue_Enqueue(&gVoiceClearQueue, ULNGV((size_t) pCallback));
        pthread_mutex_unlock(&gAudioSourceMutex);
    }
}

static void OnBufferStart(IXAudio2VoiceCallback* This, void* pBufferContext) { }

static void OnBufferEnd(IXAudio2VoiceCallback* This, void* pBufferContext) { }

static void OnLoopEnd(IXAudio2VoiceCallback* This, void* pBufferContext) { }

static void OnVoiceError(IXAudio2VoiceCallback* This, void* pBuffercontext, HRESULT Error) { }

static IXAudio2VoiceCallbackVtbl TempVoiceCallbackVtbl = {
        OnVoiceProcessingPassStart, OnVoiceProcessingPassEnd, OnStreamEnd,
        OnBufferStart, OnBufferEnd, OnLoopEnd, OnVoiceError
};

static AudioFormat SwitchAudioFileFormat(const unsigned char* header)
{
    if (memcmp(header, WAV_SIGNATURE, 4) == 0)
        return WAV;
    if (memcmp(header, FLAC_SIGNATURE, 4) == 0)
        return FLAC;


    return INVALID;
}

static void CreateAudioClipFromWav(FILE* file, AudioClip* out)
{
    int chunkDataSize;
    unsigned char chunkType[4];
    unsigned char fileType[4];
    fread(chunkType, 4, 1, file);
    assert(memcmp(chunkType, WAV_SIGNATURE, 4) == 0);
    fread(&chunkDataSize, sizeof chunkDataSize, 1, file);
    fread(fileType, 4, 1, file);
    if (memcmp(fileType, WAV_CHUNKTYPE, 4) != 0)
    {
        out->format = INVALID;
        return;
    }

    fseek(file, 4, SEEK_CUR);
    fread(&chunkDataSize, sizeof chunkDataSize, 1, file);
    WAVEFORMATEXTENSIBLE* wfx = malloc(sizeof(WAVEFORMATEXTENSIBLE));
    *wfx = (WAVEFORMATEXTENSIBLE) {};
    fread(wfx, sizeof(unsigned char) * chunkDataSize, 1, file);
    fseek(file, 4, SEEK_CUR);
    fread(&chunkDataSize, sizeof chunkDataSize, 1, file);
    unsigned char* dataBuffer = malloc(sizeof(unsigned char) * chunkDataSize);
    fread(dataBuffer, sizeof(unsigned char) * chunkDataSize, 1, file);

    out->length = chunkDataSize;
    out->data = dataBuffer;
    out->formatPtr = (size_t) wfx;
}


bool InitAudioSystem_Internal()
{
    HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    if (FAILED(hr))
        return false;

    hr = XAudio2Create(&gIXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
    if (FAILED(hr))
        return false;

    hr = gIXAudio2->lpVtbl->CreateMasteringVoice(gIXAudio2, &gIXAudio2MasteringVoice,
                                                 0, 0, 0, NULL, NULL,
                                                 AudioCategory_GameMedia);


    if (FAILED(hr) || !Queue_Init(&gVoiceClearQueue, kULLONG, NULL, NULL))
        return false;
    return true;
}

void CleanUpAudioSystem_Internal()
{
    pthread_mutex_lock(&gAudioSourceMutex);
    AudioVoiceQueueClear();
    pthread_mutex_unlock(&gAudioSourceMutex);

    gIXAudio2MasteringVoice->lpVtbl->DestroyVoice(gIXAudio2MasteringVoice);
    gIXAudio2->lpVtbl->Release(gIXAudio2);

    CoUninitialize();
    pthread_mutex_destroy(&gAudioSourceMutex);

}

void CreateAudioClip_Internal(FILE* file, AudioClip* out)
{
    unsigned char header[4];
    size_t bytesRead = fread(header, 1, 4, file);
    if (bytesRead < 4)
    {
        out->format = INVALID;
        return;
    }
    fseek(file, 0, SEEK_SET);
    switch (SwitchAudioFileFormat(header))
    {
        case INVALID:
            out->format = INVALID;
            return;
        case WAV:
            CreateAudioClipFromWav(file, out);
            break;
        case MAP3:

            break;
        case FLAC:

            break;
    }

}

IXAudio2SourceVoice* CreateSourceFromClip(const AudioClip* clip, AudioSource* source, bool autoClear)
{
    IXAudio2SourceVoice* audio;

    TempVoiceCallback* callback = malloc(sizeof(TempVoiceCallback));
    callback->callback.lpVtbl = &TempVoiceCallbackVtbl;
    callback->autoClear = autoClear;
    callback->source = source;
    gIXAudio2->lpVtbl->CreateSourceVoice(gIXAudio2, &audio, (WAVEFORMATEX*) clip->formatPtr, 0,
                                         XAUDIO2_DEFAULT_FREQ_RATIO, (IXAudio2VoiceCallback*) callback, 0, 0);
    callback->pSourceVoice = audio;
    return audio;
}

void PlayOneShot_Internal(const AudioClip* clip)
{
    XAUDIO2_BUFFER buffer = {0};

    buffer.AudioBytes = clip->length;
    buffer.pAudioData = clip->data;
    buffer.Flags = XAUDIO2_END_OF_STREAM;

    IXAudio2SourceVoice* temp = CreateSourceFromClip(clip, NULL, true);

    temp->lpVtbl->SubmitSourceBuffer(temp, &buffer, 0);
    temp->lpVtbl->Start(temp, 0, XAUDIO2_COMMIT_NOW);

    if (pthread_mutex_trylock(&gAudioSourceMutex))
        return;
    AudioVoiceQueueClear();
    pthread_mutex_unlock(&gAudioSourceMutex);

}


XAUDIO2_BUFFER CreateBufferFromClip(const AudioClip* clip)
{
    XAUDIO2_BUFFER buffer = {0};

    buffer.AudioBytes = clip->length;
    buffer.pAudioData = clip->data;
    buffer.Flags = XAUDIO2_END_OF_STREAM;

    return buffer;
}

void SetClip_Internal(AudioSource* source, const AudioClip* clip)
{
    size_t sourceId = _GetAudioSourceID(source);
    if (sourceId != 0)
    {
        AudioSourcePause_Internal(source);
        ((IXAudio2SourceVoice*) sourceId)->lpVtbl->DestroyVoice((IXAudio2SourceVoice*) sourceId);
    }
    _SetAudioSourceID(source, (size_t) CreateSourceFromClip(clip, source, false));
    IXAudio2SourceVoice* curr = (IXAudio2SourceVoice*) _GetAudioSourceID(source);
    auto buffer = CreateBufferFromClip(clip);
    curr->lpVtbl->SubmitSourceBuffer(curr, &buffer, 0);
}

void AudioSourcePlay_Internal(AudioSource* source)
{
    IXAudio2SourceVoice* curr = (IXAudio2SourceVoice*) _GetAudioSourceID(source);
    if (IsAudioPlaying(source))
        return;

    curr->lpVtbl->Start(curr, 0, XAUDIO2_COMMIT_NOW);
    _SetAudioSourceIsPlaying(source, true);
}

void AudioSourcePause_Internal(AudioSource* source)
{
    IXAudio2SourceVoice* curr = (IXAudio2SourceVoice*) _GetAudioSourceID(source);
    if (!IsAudioPlaying(source))
        return;
    curr->lpVtbl->Stop(curr, 0, XAUDIO2_COMMIT_NOW);
    _SetAudioSourceIsPlaying(source, false);
}

