//
// Created by 18030 on 2025/10/11.

#ifndef CONSOLE_H
#define CONSOLE_H

#include "DataLibC/DS/Pair/Pair.h"
#include "DataLibC/Tool/ExportModuleMacro.h"
#include "DataLibC/Tool/Color32.h"


/**
 * @brief Call this method to initialize the console context.
 * @return If a valid console is found, return true. Otherwise, return false.
 * @attention This method must be called before any other console methods. If there is no valid console or this method
 * is not called, the console methods will not work.
 */
API_MODULE bool Console_InitContext();

/**
 * @brief Output a formatted string to the console
 * @param fmt The format string
 * @param ... The arguments
 */
API_MODULE void Console_Write(const char* fmt, ...);

/**
 * @brief Output a formatted string to the console and append a new line
 * @param fmt The format string
 * @param ... The arguments
 */
API_MODULE void Console_WriteLine(const char* fmt, ...);

/**
 * @brief Output a formatted string to the console at a specific position
 * @param x The x coordinate
 * @param y The y coordinate
 * @param fmt The format string
 * @param ... The arguments
 */
API_MODULE void Console_WriteAt(int x, int y, const char* fmt, ...);

/**
 * @brief Output a formatted string to the console at a specific position and append a new line
 * @param x The x coordinate
 * @param y The y coordinate
 * @param fmt The format string
 * @param ... The arguments
 */
API_MODULE void Console_WriteAtW(int x, int y, const wchar_t* fmt, ...);

/**
 * @brief Set the width of the console
 * @param width The width of the console
 */
API_MODULE void Console_SetWidth(int width);

/**
 * @brief Set the height of the console
 * @param height The height of the console
 */
API_MODULE void Console_SetHeight(int height);

/**
 * @brief Set the width and height of the console
 * @param width The width of the console
 * @param height The height of the console
 */
API_MODULE void Console_SetWindowSize(int width, int height);

/**
 * @brief Get the width of the console
 * @return The width of the console
 */
API_MODULE int Console_GetWidth();

/**
 * @brief Get the height of the console
 * @return The height of the console
 */
API_MODULE int Console_GetHeight();

/**
 * @brief Get the width and height of the console window
 * @return The pair <width, height> of the console window
 */
API_MODULE IntPair Console_GetWindowSize();

/**
 * @brief Clear the console and reset the cursor to the original position
 */
API_MODULE void Console_Clear();

/**
 * @brief Clear the console in a specific range
 * @param x The x coordinate of the top left corner
 * @param y The y coordinate of the top left corner
 * @param width The width of the range
 * @param height The height of the range
 * @attention The index starts from 0, not 1. That means (0, 0) is the top left character)
 */
API_MODULE void Console_ClearRange(int x, int y, int width, int height);

/**
 * @brief Set console foreground color (font color).
 * @param color The RGBA format color in [0, 1]
 */
API_MODULE void Console_SetFGColor(Color32 color);

/**
 * @brief Set console background color.
 * @param color The RGBA format color in [0, 1]
 */
API_MODULE void Console_SetBGColor(Color32 color);

/**
 * @brief Reset console style to default (Foreground and Background)
 */
API_MODULE void Console_ResetStyle();

/**
 * @brief Set console cursor position in window coordinate system
 * @param x The x coordinate
 * @param y The y coordinate
 * @attention The index starts from 0, not 1. That means (0, 0) is the top left character)
 */
API_MODULE void Console_SetCursorPos(int x, int y);

/**
 * @brief Hide console cursor
 */
API_MODULE void Console_HideCursor();

/**
 * @brief Show console cursor
 */
API_MODULE void Console_ShowCursor();

/**
 * @brief Set console font size
 * @param size The font size
 */
API_MODULE void Console_SetFontSize(int size);


#endif //CONSOLE_H
