//
// Created by 18030 on 2025/10/11.
//

#include "Console.h"

#include <stdarg.h>
#include <stdio.h>

#include "DataLibC/Tool/LogMacro.h"

#ifdef LIB_WIN
#include <windows.h>
typedef HANDLE Console;
#endif

#define ESC "\x1B"
#define CSI "\x1B["

typedef struct ConsoleContext
{
    Console console;
    bool hasValidConsole;
    int cachedConsoleWidth;
    int cachedConsoleHeight;
    bool isCursorVisible;
} ConsoleContext;

static ConsoleContext gConsoleContext = {
    .hasValidConsole = false,
    .cachedConsoleWidth = 120,
    .cachedConsoleHeight = 90
};

#ifdef LIB_WIN
extern HANDLE WinCheckConsoleValid();

#endif

bool Console_InitContext()
{
#ifdef LIB_WIN
    gConsoleContext.console = WinCheckConsoleValid();
#endif
    gConsoleContext.hasValidConsole = gConsoleContext.console != nullptr;
    if (gConsoleContext.hasValidConsole)
    {
        const IntPair size = Console_GetWindowSize();
        gConsoleContext.cachedConsoleWidth = size.first;
        gConsoleContext.cachedConsoleHeight = size.second;
    }

    return gConsoleContext.hasValidConsole;
}

void Console_Write(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    fflush(stdout);
}

void Console_WriteLine(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    printf("\n");
    fflush(stdout);
}

void Console_WriteAt(const int x, const int y, const char* fmt, ...)
{
    Console_SetCursorPos(x, y);
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    fflush(stdout);
}

void Console_WriteAtW(const int x, const int y, const wchar_t* fmt, ...)
{
    Console_SetCursorPos(x, y);
    va_list args;
    va_start(args, fmt);
    vwprintf(fmt, args);
    va_end(args);
    fflush(stdout);
}

void Console_SetWidth(const int width)
{
    const SMALL_RECT windowInfo = {
        0,
        0,
        (short)(width - 1),
        (short)(gConsoleContext.cachedConsoleHeight - 1)
    };
    SetConsoleWindowInfo(gConsoleContext.console, true, &windowInfo);
    gConsoleContext.cachedConsoleWidth = width;
}

void Console_SetHeight(const int height)
{
    const SMALL_RECT windowInfo = {
        0,
        0,
        (short)(gConsoleContext.cachedConsoleWidth - 1),
        (short)(height - 1)
    };
    SetConsoleWindowInfo(gConsoleContext.console, true, &windowInfo);
    gConsoleContext.cachedConsoleHeight = height;
}

void Console_SetWindowSize(const int width, const int height)
{
    const SMALL_RECT windowInfo = { 0, 0, (short)(width - 1), (short)(height - 1) };
    const COORD bufferSize = { (short)(width), (short)(height) };
    SetConsoleScreenBufferSize(gConsoleContext.console, bufferSize);
    SetConsoleWindowInfo(gConsoleContext.console, true, &windowInfo);
    gConsoleContext.cachedConsoleWidth = width;
    gConsoleContext.cachedConsoleHeight = height;
}

int Console_GetWidth()
{
    return gConsoleContext.cachedConsoleWidth;
}

int Console_GetHeight()
{
    return gConsoleContext.cachedConsoleHeight;
}

IntPair Console_GetWindowSize()
{
    CONSOLE_SCREEN_BUFFER_INFO out = { };
    GetConsoleScreenBufferInfo(gConsoleContext.console, &out);
    return (IntPair){
        out.srWindow.Right - out.srWindow.Left + 1,
        out.srWindow.Bottom - out.srWindow.Top + 1
    };
}

void Console_Clear()
{
    printf(CSI "2J" CSI "3J" CSI "H");
}

void Console_ClearRange(const int x, const int y, const int width, const int height)
{
    for (int i = 0; i < height; i++)
    {
        Console_SetCursorPos(x, y + i);
        printf(CSI "%dX", width);
    }
}

void Console_SetFGColor(const Color32 color)
{
    printf(CSI "38;2;%d;%d;%dm", color.r, color.g, color.b);
}

void Console_SetBGColor(const Color32 color)
{
    printf(CSI "48;2;%d;%d;%dm", color.r, color.g, color.b);
}

void Console_ResetStyle()
{
    printf(CSI "0m");
}

void Console_SetCursorPos(const int x, const int y)
{
    if (x < 0 || y < 0 || x >= gConsoleContext.cachedConsoleWidth || y >= gConsoleContext.cachedConsoleHeight)
    {
        RuntimeErrorLog(InvalidArgument, "Invalid cursor position <%d, %d>", x, y);
        return;
    }
    printf(CSI "%d;%dH", y + 1, x + 1);
}

void Console_HideCursor()
{
    printf(CSI "?25l");
    gConsoleContext.isCursorVisible = false;
}

void Console_ShowCursor()
{
    printf(CSI "?25h");
    gConsoleContext.isCursorVisible = true;
}

void Console_SetFontSize(const int size)
{
    CONSOLE_FONT_INFOEX info = { };
    GetCurrentConsoleFontEx(gConsoleContext.console, false, &info);
    const float ratio = (float)info.dwFontSize.Y / (float)info.dwFontSize.X;
    info.dwFontSize.X = (short)size;
    info.dwFontSize.Y = (short)(size * ratio);
    SetCurrentConsoleFontEx(gConsoleContext.console, false, &info);
}
