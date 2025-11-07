//
// Created by 18030 on 2024/12/9.
//

#ifndef TEST_INPUT_H
#define TEST_INPUT_H

#include "DataLibC/Tool/ExportModuleMacro.h"
#include "DataLibC/LA/Vector/Vector2.h"


typedef enum MouseButton MouseButton;
typedef enum KeyCode KeyCode;

/// \brief Used to initialize input system
/// \attention Remember calling this before your logical loop
/// \param hasMsgProcess Is there any other message processing function in your program? True means
/// message processing in DataLibC will be disabled
/// \return True if succeed, otherwise false
API_MODULE bool InputInit(bool hasMsgProcess);

/// \brief Used to update global input state
/// \attention Remember calling this at the beginning of your logical loop
API_MODULE void InputUpdate();

/// Used to clean global input state
/// \attention Remember calling this before your program finished completely
API_MODULE void InputTerminate();

/// Get a key state
/// \param keyCode The key enum value
/// \return True if the key is being pressed
API_MODULE bool GetKey(KeyCode keyCode);

/// Get a key state
/// \param keyCode The key enum value
/// \return True if pressing down the key during your logical frame
API_MODULE bool GetKeyDown(KeyCode keyCode);

/// Get a key state
/// \param keyCode The key enum value
/// \return True if releasing the key during your logical frame
API_MODULE bool GetKeyUp(KeyCode keyCode);

/// Get a mouse button state
/// \param mouseButton The button enum value
/// \return True if the button is being pressed
API_MODULE bool GetMouseButton(MouseButton mouseButton);

/// Get a mouse button state
/// \param mouseButton The button enum value
/// \return True if pressing down the button duriing your logical frame
API_MODULE bool GetMouseButtonDown(MouseButton mouseButton);

/// Get a mouse button state
/// \param mouseButton The button enum value
/// \return True if releasing the button during your logical frame
API_MODULE bool GetMouseButtonUp(MouseButton mouseButton);

/// Get your mouse cursor position
/// \return The position in pixel coordinates
API_MODULE Vector2 GetMousePosition();

/// Get your mouse cursor position delta value
/// \return The delta value between last and current frame in pixel coordinates
API_MODULE Vector2 GetMousePositionDelta();

/// Get delta seconds between current and last frame
API_MODULE float DeltaTime();

/// Get the time elapsed since the input system initialization
/// \return Current time
API_MODULE float GetCurrTime();

#endif //TEST_INPUT_H
