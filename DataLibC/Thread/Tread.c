//
// Created by 18030 on 2025/10/20.
//

#include "Tread.h"

#if LIB_WIN
#include <windows.h>
#elif LIB_LINUX
#include <unistd.h>
#endif


void Thread_Sleep(const float seconds)
{
#if LIB_WIN
    Sleep((int)(seconds * 1000));
#elif LIB_LINUX
    usleep(seconds * 1000);
#else
#error "Unknown platform"
#endif
}
