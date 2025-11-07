//
// Created by 18030 on 2025/6/23.
//

#include "Message.h"

#include <assert.h>
#include <stdlib.h>

#include "DataLibC/DS/List/DynamicArray.h"
#include "DataLibC/DS/Map/TreeMap.h"
#include "DataLibC/DS/Pair/Pair.h"
#include "DataLibC/Tool/LogMacro.h"

static constexpr int MAX_MESSAGE_EVENT_COUNT = 64;

static struct MSGCenter
{
    bool hasInit;
    int maxMessageCount;
    TreeMap messageMap;

    struct CallerArray
    {
        DynamicArray objects;
        DynamicArray messageCalls;
    };
}* messageEventSystem = NULL;

typedef struct CallerArray CallerArray;

static void* _CallerArrayCtor(const void*)
{
    CallerArray* self = malloc(sizeof(CallerArray));
    DynamicArray_Init(&self->objects, 0, kPOINTER, 0);
    DynamicArray_Init(&self->messageCalls, 0, kPOINTER, 0);
    return self;
}

static void _CallerArrayDtor(void* array)
{
    CallerArray* self = array;
    DynamicArray_Clear(&self->objects);
    DynamicArray_Clear(&self->messageCalls);
}

static void _OnMaxMessageEventCountChanged(int lastAmount)
{
    // TODO
}


static void _MessageCenterInitCheck()
{
    if (!messageEventSystem)
    {
        messageEventSystem = malloc(sizeof(struct MSGCenter));
        TreeMap_Init(&messageEventSystem->messageMap, kSTRING, kCUSTOM, _CallerArrayCtor, _CallerArrayDtor, NULL);
        messageEventSystem->maxMessageCount = MAX_MESSAGE_EVENT_COUNT;
        messageEventSystem->hasInit = true;
    }
}

bool RegisterMessage(const char* messageName, void* object, MessageEventCall* messageCall)
{
    _MessageCenterInitCheck();
    Pair* p = TreeMap_ValueAt(&messageEventSystem->messageMap, messageName);
    if (!p)
    {
        TreeMap_Insert(&messageEventSystem->messageMap, messageName, &(CallerArray){ });
        p = TreeMap_ValueAt(&messageEventSystem->messageMap, messageName);
    }
    CallerArray* arr = Second(*p);
    DynamicArray_Add(&arr->objects, &object);
    DynamicArray_Add(&arr->messageCalls, &messageCall);
    return true;
}

bool SendMessage(const char* messageName, void* sender, void* receiver, void** eventArgs)
{
    Pair* p = TreeMap_ValueAt(&messageEventSystem->messageMap, messageName);
    if (!p)
    {
        RuntimeErrorLog(NoSuchValue, "The message event you send is not exists!");
        return false;
    }
    CallerArray* arr = Second(*p);
    assert(arr->objects.length == arr->messageCalls.length);
    const auto size = arr->objects.length;
    for (int i = 0; i < size; i++)
    {
        void* obj = *(void**)DynamicArray_ValueAt(&arr->objects, i);
        MessageEventCall* call = *(MessageEventCall**)DynamicArray_ValueAt(&arr->messageCalls, i);
        call(obj, sender, receiver, eventArgs);
    }
    return true;
}

int SetMaxMessageEventCount(int count)
{
    _MessageCenterInitCheck();
    int lastAmount = messageEventSystem->maxMessageCount;
    if (count < 0)
    {
        RuntimeErrorLog(InvalidArgument, "The argument [count]: %d is "
                        "negative. Default setting it to %d!", count, MAX_MESSAGE_EVENT_COUNT);
        count = MAX_MESSAGE_EVENT_COUNT;
    }
    messageEventSystem->maxMessageCount = count;
    _OnMaxMessageEventCountChanged(lastAmount);
    return lastAmount;
}

