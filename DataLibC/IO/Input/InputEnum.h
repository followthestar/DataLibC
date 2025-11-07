//
// Created by 18030 on 2024/12/9.
//

#ifndef TEST_INPUTENUM_H
#define TEST_INPUTENUM_H

typedef enum KeyCode
{
    KEY_ESCAPE = 0x1B,
    KEY_SPACE = 0x20,

    KEY_0 = 0x30,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,

    KEY_A = 0x41,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_E,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_I,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_M,
    KEY_N,
    KEY_O,
    KEY_P,
    KEY_Q,
    KEY_R,
    KEY_S,
    KEY_T,
    KEY_U,
    KEY_V,
    KEY_W,
    KEY_X,
    KEY_Y,
    KEY_Z,

    KEY_F1 = 0x70,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_F6,
    KEY_F7,
    KEY_F8,
    KEY_F9,
    KEY_F10,
    KEY_F11,
    KEY_F12,

    KEY_COUNT
} KeyCode;

typedef enum MouseButton
{
    MBTN_LEFT = 0x01,
    MBTN_RIGHT,
    MBTN_MIDDLE = 0x04
} MouseButton;
#endif //TEST_INPUTENUM_H
