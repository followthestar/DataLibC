//
// Created by 18030 on 2024/12/9.
//

#include "InputState.h"
#include "InputEnum.h"
#include "DataLibC/DS/Queue/Queue.h"
#include "DataLibC/DS/List/LinkedList.h"
#include "DataLibC/Anim/Interface/IAnimated.h"
#include "windows.h"

#include <pthread.h>

typedef char KeyStates[KEY_COUNT];

static KeyStates gKeyState = { };

static Queue* gInputMsgQueue = NULL;
static LinkedList* gIAnimatedValueList = NULL;
static LinkedList* gIAnimatedDestroyList = NULL;

static pthread_mutex_t gInputMsgMtx = PTHREAD_MUTEX_INITIALIZER;

typedef struct KeyState
{
    int vkCode;
    bool state;
} KeyState;

void* KeyStateConstructor(const void* value)
{
    KeyState* s = malloc(sizeof(KeyState));
    memcpy(s, value, sizeof(KeyState));
    return s;
}

static void _update_key_state(KeyCode keyCode, bool state)
{
    if (state)
    {
        switch (gKeyState[keyCode])
        {
            case KEY_STATE_UNPRESS:
            case KEY_STATE_UP:
                gKeyState[keyCode] = KEY_STATE_DOWN_THIS_FRAME;
                break;
        }
    }
    else
    {
        switch (gKeyState[keyCode])
        {
            case KEY_STATE_PRESS:
            case KEY_STATE_DOWN:
                gKeyState[keyCode] = KEY_STATE_UP_THIS_FRAME;
                break;
        }
    }
}

float DeltaTime();

void UpdateInputState_Internal()
{
    pthread_mutex_lock(&gInputMsgMtx);
    while (!Queue_Empty(gInputMsgQueue))
    {
        KeyState* s = Queue_Front(gInputMsgQueue);
        _update_key_state(s->vkCode, s->state);
        Queue_Dequeue(gInputMsgQueue);
    }
    pthread_mutex_unlock(&gInputMsgMtx);
    for (int i = 0; i < KEY_COUNT; ++i)
    {
        if (gKeyState[i] == KEY_STATE_DOWN_THIS_FRAME)
            gKeyState[i] = KEY_STATE_DOWN;
        else if (gKeyState[i] == KEY_STATE_UP_THIS_FRAME)
            gKeyState[i] = KEY_STATE_UP;
        else if (gKeyState[i] == KEY_STATE_DOWN)
            gKeyState[i] = KEY_STATE_PRESS;
        else if (gKeyState[i] == KEY_STATE_UP)
            gKeyState[i] = KEY_STATE_UNPRESS;
    }

    auto iter2 = LinkedList_Iterator(gIAnimatedDestroyList);
    while (HasNext(iter2))
    {
        IAnimated* animated = *(IAnimated**)Next(iter2);
        LinkedList_Remove(gIAnimatedValueList, &animated);
        Remove(iter2);
        free(animated);
    }

    auto iter = LinkedList_Iterator(gIAnimatedValueList);
    while (HasNext(iter))
    {
        IAnimated** animated = Next(iter);
        const bool finished = (*animated)->vtable->UpdateValue(*animated, DeltaTime());
        if (finished)
            Remove(iter);
    }
}

KEYSTATE GetInputKeyState(int keyCode)
{
    return gKeyState[keyCode];
}

bool InitInputState_Internal()
{
    gInputMsgQueue = malloc(sizeof(Queue));
    gIAnimatedValueList = malloc(sizeof(LinkedList));
    gIAnimatedDestroyList = malloc(sizeof(LinkedList));
    bool init1 = LinkedList_Init(gIAnimatedValueList, kULLONG, NULL, NULL);
    bool init2 = LinkedList_Init(gIAnimatedDestroyList, kULLONG, NULL, NULL);
    bool init3 = Queue_Init(gInputMsgQueue, kCUSTOM, KeyStateConstructor, NULL);
    return init1 && init2 && init3;
}

void AddInputMsg_Internal(int vkCode, bool state)
{
    pthread_mutex_lock(&gInputMsgMtx);

    KeyState kMsg = { vkCode, state };
    Queue_Enqueue(gInputMsgQueue, &kMsg);

    pthread_mutex_unlock(&gInputMsgMtx);
}

void CleanInputState_Internal()
{
    pthread_mutex_destroy(&gInputMsgMtx);
    LinkedList_Clear(gIAnimatedValueList);
    free(gIAnimatedValueList);
}

void AddIAnimated_Internal(IAnimated* animated)
{
    LinkedList_AddLast(gIAnimatedValueList, &animated);
}

void AddIAnimatedDestroy_Internal(IAnimated* animated)
{
    LinkedList_AddLast(gIAnimatedDestroyList, &animated);
}
