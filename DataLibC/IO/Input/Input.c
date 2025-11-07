//
// Created by 18030 on 2024/12/9.
//

#include <windows.h>
#include <time.h>
#include "Input.h"
#include "InputEnum.h"
#include "InputState.h"
#include "DataLibC/Tool/LogMacro.h"

HHOOK gKeyboardHook = NULL;
HHOOK gMouseHook = NULL;

static Vector2 gMousePosition;
static Vector2 gMousePositionDelta;
static bool gIsFirstTimeMousePosition = true;

static bool gUseDataLibCMsg = false;

static bool gIsFirstTimeGetClock = true;
static float deltaTime = 0;
static clock_t curr = 0;

static LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    static int code;
    static bool state;
    if (nCode >= 0)
    {
        KBDLLHOOKSTRUCT* pkbhs = (KBDLLHOOKSTRUCT*) lParam;
        if (gUseDataLibCMsg)
        {
            code = (int) pkbhs->vkCode;
            state = (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN);
        }
        else
        {
            code = (int) wParam;
            state = lParam & 0x80000000;
        }
        AddInputMsg_Internal(code, state);
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

static LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode >= 0)
    {
        MSLLHOOKSTRUCT* pmsllhs = (MSLLHOOKSTRUCT*) lParam;
        gMousePosition.x = (float) pmsllhs->pt.x;
        gMousePosition.y = (float) pmsllhs->pt.y;
        switch (wParam)
        {
            case WM_LBUTTONDOWN:
                AddInputMsg_Internal(MBTN_LEFT, true);
                break;
            case WM_MBUTTONDOWN:
                AddInputMsg_Internal(MBTN_MIDDLE, true);
                break;
            case WM_RBUTTONDOWN:
                AddInputMsg_Internal(MBTN_RIGHT, true);
                break;
            case WM_LBUTTONUP:
                AddInputMsg_Internal(MBTN_LEFT, false);
                break;
            case WM_MBUTTONUP:
                AddInputMsg_Internal(MBTN_MIDDLE, false);
                break;
            case WM_RBUTTONUP:
                AddInputMsg_Internal(MBTN_RIGHT, false);
                break;
            default:

        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

bool InputInit(bool hasMsgProcess)
{
    if (hasMsgProcess)
    {
        gKeyboardHook = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, GetModuleHandle(NULL), GetCurrentThreadId());
        gMouseHook = SetWindowsHookEx(WH_MOUSE, MouseProc, GetModuleHandle(NULL), GetCurrentThreadId());
    }
    else
    {
        gKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, GetModuleHandle(NULL), 0);
        gMouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, GetModuleHandle(NULL), 0);
    }
    if (!gKeyboardHook || !gMouseHook)
        return false;
    gUseDataLibCMsg = !hasMsgProcess;
    return InitInputState_Internal();
}

void InputUpdate()
{
    static clock_t last;
    if(gIsFirstTimeGetClock)
    {
        gIsFirstTimeGetClock = false;
        curr = clock();
    }

    last = curr;
    curr = clock();
    deltaTime = (float)(curr - last) / CLOCKS_PER_SEC;

    Vector2 lastMousePosition = gMousePosition;
    if (gUseDataLibCMsg)
    {
        MSG msg;
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    UpdateInputState_Internal();
    if (gIsFirstTimeMousePosition)
    {
        gMousePositionDelta = VEC2_ZERO;
        gIsFirstTimeMousePosition = false;
    }
    else
    {
        gMousePositionDelta = VEC2(gMousePosition.x - lastMousePosition.x, gMousePosition.y - lastMousePosition.y);
    }
}

bool GetKey(KeyCode keyCode)
{
    return GetInputKeyState(keyCode) > KEY_STATE_UNPRESS;
}

bool GetKeyDown(KeyCode keyCode)
{
    return GetInputKeyState(keyCode) == KEY_STATE_DOWN;
}

bool GetKeyUp(KeyCode keyCode)
{
    return GetInputKeyState(keyCode) == KEY_STATE_UP;
}

bool GetMouseButton(MouseButton mouseButton)
{
    return GetInputKeyState(mouseButton) > KEY_STATE_UNPRESS;
}

bool GetMouseButtonDown(MouseButton mouseButton)
{
    return GetInputKeyState(mouseButton) == KEY_STATE_DOWN;
}

bool GetMouseButtonUp(MouseButton mouseButton)
{
    return GetInputKeyState(mouseButton) == KEY_STATE_UP;
}

void InputTerminate()
{
    UnhookWindowsHookEx(gKeyboardHook);
    UnhookWindowsHookEx(gMouseHook);
    CleanInputState_Internal();
}

Vector2 GetMousePosition()
{
    return gMousePosition;
}

Vector2 GetMousePositionDelta()
{
    return gMousePositionDelta;
}

float DeltaTime()
{
    return deltaTime;
}

float GetCurrTime()
{
    return 1.0f * curr / CLOCKS_PER_SEC;
}


