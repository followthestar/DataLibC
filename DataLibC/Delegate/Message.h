//
// Created by 18030 on 2025/6/23.
//

#ifndef MESSAGE_H
#define MESSAGE_H

#include "DataLibC/Tool/ExportModuleMacro.h"


/// @brief Every registered message call should have the same signature as the following
/// @param self Used to mark which object this message call belongs to
/// @param sender Who sends this message?
/// @param receiver Who receives this message?
/// @param eventArgs The event args of this message type
typedef void MessageEventCall(void* self, void* sender, void* receiver, void** eventArgs);

/**
 * @brief Register a message call to the message event system. If the message is already registered,
 * the call will be added to the message call list, otherwise a new message will be created.
 * @param messageName The name of this kind of message.
 * @param object The object you want to bind to this message.
 * @param messageCall The function call you want to bind to this message.
 * @return True if the message is registered successfully, otherwise false.
 */
API_MODULE bool RegisterMessage(const char* messageName, void* object, MessageEventCall* messageCall);

/**
 * @brief Unregister a message from an object which has registered a message
 * @param messageName Which message do you want to unregister?
 * @param object Which object do you want to unregister the message from?
 * @param messageCall Which message call do you want to unregister from the object?
 * @return True if the given object does unregister this message, otherwise false.
 */
API_MODULE bool UnregisterMessage(const char* messageName, void* object, MessageEventCall* messageCall);

/**
 * @brief Send a message with given message name
 * @param messageName The message event name
 * @param sender Who sends this message?
 * @param receiver Who receives this message?
 * @param eventArgs The event args of this message type
 * @return True if the message exists, otherwise false.
 */
API_MODULE bool SendMessage(const char* messageName, void* sender, void* receiver, void** eventArgs);

/**
 * @brief Remove a message event by name
 * @param messageName The name of message you want to remove
 * @return True if the message exists and is removed, otherwise false.
 */
API_MODULE bool DeleteMessage(const char* messageName);

/**
 * @brief Set a maximum amount of message event (message type, not the registered message call).
 * The default amount is 64.
 * @param count The new message event amount.
 * @return Last amount of message event.
 */
API_MODULE int SetMaxMessageEventCount(int count);


#endif //MESSAGE_H
