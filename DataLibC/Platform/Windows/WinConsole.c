//
// Created by 18030 on 2025/10/11.
//

#include "DataLibC/Tool/ExportModuleMacro.h"

#ifdef LIB_WIN
#include <windows.h>

static void EnableVirtualTerminalProcessing()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE)
        return;

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode))
        return;

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}

HANDLE WinCheckConsoleValid()
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode;
    const bool isValid = h != INVALID_HANDLE_VALUE && GetConsoleMode(h, &mode);
    if (isValid)
        EnableVirtualTerminalProcessing();
    else
        return nullptr;
    return h;
}


#endif
