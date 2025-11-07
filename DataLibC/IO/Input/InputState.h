//
// Created by 18030 on 2024/12/9.
//

#ifndef TEST_INPUTSTATE_H
#define TEST_INPUTSTATE_H

#define KEY_STATE_UP_THIS_FRAME (-2)
#define KEY_STATE_UP (-1)
#define KEY_STATE_UNPRESS 0
#define KEY_STATE_DOWN_THIS_FRAME 1
#define KEY_STATE_DOWN 2
#define KEY_STATE_PRESS 3

typedef char KEYSTATE;
typedef enum KeyCode KeyCode;

typedef struct _IAnimated IAnimated;

void UpdateInputState_Internal();

bool InitInputState_Internal();

void CleanInputState_Internal();

void AddInputMsg_Internal(int vkCode, bool state);

KEYSTATE GetInputKeyState(int keyCode);

void AddIAnimated_Internal(IAnimated* animated);

void RemoveIAnimated_Internal(IAnimated* animated);

#endif //TEST_INPUTSTATE_H
