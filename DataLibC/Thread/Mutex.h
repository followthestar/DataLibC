//
// Created by 18030 on 2025/9/29.
//

#ifndef MUTEX_H
#define MUTEX_H


#include <pthread.h>


#if defined(__GNUC__) && (defined(__MINGW32__) || defined(__MINGW64__))
typedef pthread_mutex_t PlatformMutex;
#define InitPlatformMutex(mutex) pthread_mutex_init((PlatformMutex*)(&mutex), nullptr)
#define LockPlatformMutex(mutex) pthread_mutex_lock((PlatformMutex*)(&mutex))
#define UnlockPlatformMutex(mutex) pthread_mutex_unlock((PlatformMutex*)(&mutex))
#endif


#endif

