//
// Created by 18030 on 2024/11/16.
//

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "String.h"
#include "DataLibC/DS/Algorithm.h"
#include "DataLibC/Tool/LogMacro.h"

static bool _String_ExpandCapacity(String* str)
{
    size_t newCapacity = str->m_Capacity * 2 - 1;
    char* tempBuf = realloc(str->m_Buffer, newCapacity);
    if (tempBuf)
    {
        str->m_Buffer = tempBuf;
        str->m_Capacity = newCapacity;
        PLUS_ASSIGN(str, m_AllocationCount, 1);
        memset(str->m_Buffer + str->length, 0, newCapacity - str->length);
        return true;
    }
    return false;
}

static bool _String_AppendInternal(String* str, const char* const src, size_t addL)
{
    while (str->length + addL >= str->m_Capacity)
    {
        if (!_String_ExpandCapacity(str))
            return false;
    }
    memcpy(str->m_Buffer + str->length, src, addL);
    str->length += addL;
    str->m_Dirty = true;
    return true;
}

static Npos _String_FindInternal(String* str, const char* const pattern, size_t length)
{
    int next[length];
    memset(next, 0, sizeof(int) * length);
    for (int i = 1, j = 0; i < length; i++)
    {
        while (j && pattern[i] != pattern[j])
            j = next[j - 1];
        if (pattern[i] == pattern[j])
            next[i] = ++j;
    }
    for (int i = 0, j = 0; i < str->length; i++)
    {
        while (j && str->m_Buffer[i] != pattern[j])
            j = next[j - 1];

        if (str->m_Buffer[i] == pattern[j])
        {
            if (++j == length)
                return i - length + 1;
        }
    }
    return npos;
}

static size_t _String_CountInternal(String* str, const char* const pattern, size_t length)
{
    int count = 0;
    int next[length];
    memset(next, 0, sizeof(int) * length);
    for (int i = 1, j = 0; i < length; i++)
    {
        while (j && pattern[i] != pattern[j])
            j = next[j - 1];
        if (pattern[i] == pattern[j])
            next[i] = ++j;
    }
    for (int i = 0, j = 0; i < str->length; i++)
    {
        while (j && str->m_Buffer[i] != pattern[j])
            j = next[j - 1];

        if (str->m_Buffer[i] == pattern[j])
        {
            if (++j == length)
            {
                count++;
                j = next[j - 1];
            }
        }
    }
    return count;
}

bool String_Init(String* str, size_t size, const char* const elemPtr)
{
    assert(str);
    INIT_ASSIGN(str, m_AllocationCount, 0);
    str->length = elemPtr ? strlen(elemPtr) : 0;
    size_t maxL = max(str->length, size);
    str->m_Capacity = NextPowerTwo(maxL) + 1;
    if (str->m_Capacity < 8)
        str->m_Capacity = 8;

    str->m_Buffer = malloc(sizeof(char) * str->m_Capacity);
    if (!str->m_Buffer)
    {
        RuntimeErrorLog(MallocFailed, "Failed to init a String memory!");
        return false;
    }
    PLUS_ASSIGN(str, m_AllocationCount, 1);
    memset(str->m_Buffer, 0, str->m_Capacity);
    memcpy(str->m_Buffer, elemPtr, str->length);


    str->m_Dirty = true;
    return true;
}

const char* String_ToString(const String* str)
{
    assert(str);
    return str->m_Buffer;
}

String* String_Append(String* str, const char* const elemPtr)
{
    assert(str);
    size_t addL = strlen(elemPtr);
    if (!_String_AppendInternal(str, elemPtr, addL))
        RuntimeErrorLog(MallocFailed, "Failed to append data!");
    return str;
}

String* String_AppendChar(String* str, char ch)
{
    assert(str);
    if (!_String_AppendInternal(str, &ch, 1))
        RuntimeErrorLog(MallocFailed, "Failed to append data!");
    return str;
}

char String_CharAt(String* str, size_t index)
{
    assert(str);
    if (index >= str->length)
    {
        RuntimeErrorLog(IndexOutOfRange, "Index must be in range of [0, %zu) !", str->length);
        return '\0';
    }
    return str->m_Buffer[index];
}

String* String_AppendString(String* dst, const String* src)
{
    assert(dst);
    if (!src)
        return dst;
    if (!_String_AppendInternal(dst, src->m_Buffer, src->length))
        RuntimeErrorLog(MallocFailed, "Failed to append data!");
    return dst;
}

Npos String_FindChar(String* str, char ch)
{
    assert(str);
    for (size_t i = 0; i < str->length; i++)
        if (str->m_Buffer[i] == ch)
            return i;
    return npos;
}

Npos String_Find(String* str, const char* pattern)
{
    assert(str);
    if (!pattern)
        return -1;
    size_t patternL = strlen(pattern);
    if (patternL == 1)
        return String_FindChar(str, pattern[0]);
    return _String_FindInternal(str, pattern, patternL);
}

Npos String_FindString(String* str, String* pattern)
{
    assert(str);
    if (!pattern)
        return -1;
    if (pattern->length == 1)
        return String_FindChar(str, pattern->m_Buffer[0]);
    return _String_FindInternal(str, pattern->m_Buffer, pattern->length);
}

Npos String_FindLastOfChar(String* str, char ch)
{
    assert(str);
    for (size_t i = str->length; i > 0; i--)
        if (str->m_Buffer[i - 1] == ch)
            return i;
    return npos;
}

size_t String_CountOfChar(String* str, char ch)
{
    assert(str);
    size_t count = 0;
    for (int i = 0; i < str->length; i++)
        if (str->m_Buffer[i] == ch)
            count++;
    return count;
}

bool String_Contains(String* str, const char* s)
{
    return String_Find(str, s) != npos;
}

size_t String_Count(String* str, const char* pattern)
{
    assert(str);
    if (!pattern)
        return 0;
    size_t patternL = strlen(pattern);
    if (patternL == 1)
        return String_CountOfChar(str, pattern[0]);
    return _String_CountInternal(str, pattern, patternL);
}

size_t String_CountString(String* str, String* pattern)
{
    assert(str);
    if (!pattern)
        return 0;
    if (pattern->length == 1)
        return String_CountOfChar(str, pattern->m_Buffer[0]);
    return _String_CountInternal(str, pattern->m_Buffer, pattern->length);
}

String* String_SubString(String* str, size_t start, size_t length)
{
    assert(str);
    if (start >= str->length)
    {
        RuntimeErrorLog(IndexOutOfRange, "Index out of range! It should be smaller than [%zu]", str->length);
        return NULL;
    }
    String* newString = malloc(sizeof(String));
    size_t len = min(length, str->length - start);
    String_Init(newString, len, NULL);
    newString->length = len;
    memcpy(newString->m_Buffer, str->m_Buffer + start, len);
    return newString;
}

void String_ReplaceFrom(String* str, size_t pos, const char* replace) { }

void String_ReplaceString(String* str, const char* oldStr, const char* newStr, bool replaceAll) { }

void String_Erase(String* str, const size_t start, const size_t length)
{
    assert(str);
    if (start >= str->length || start + length > str->length)
    {
        RuntimeErrorLog(IndexOutOfRange, "Index out of range! It should be smaller than [%zu]", str->length);
        return;
    }

    const size_t chars_to_move = str->length - (start + length) + 1;
    memmove(str->m_Buffer + start, str->m_Buffer + start + length, chars_to_move);
    str->length -= length;
    str->m_Dirty = true;
}

int String_SplitByChar(String* str, const char delimiter, char* out[], int size)
{
    assert(str);
    String_AppendChar(str, delimiter);
    int start = 0;
    int outSize = 0;
    for (int i = 0; i < str->length; i++)
    {
        const char ch = String_CharAt(str, i);
        if (ch == delimiter)
        {
            if (outSize == size)
                return outSize;
            int len = i - start;
            if (len > 0)
            {
                char* sub = out[outSize];
                memcpy(sub, str->m_Buffer + start, len);
                sub[len] = '\0';
                outSize++;
            }
            start = i + 1;
        }
    }
    String_Erase(str, str->length - 1, 1);
    return outSize;
}

bool String_ReadFromFile(String* str, FILE* file)
{
    assert(str);

    if (!file)
        return false;

    String_Clear(str);
    if (!String_Init(str, 8, "\0"))
        return false;

    char buffer[65];
    size_t bytesRead;
    while ((bytesRead = fread(buffer, sizeof(char), 64, file)) > 0)
    {
        buffer[bytesRead] = '\0';
        String_Append(str, buffer);
        if (bytesRead < 64)
            break;
    }
    return true;
}

void String_Clear(String* str)
{
    assert(str);
    str->length = 0;
    str->m_Dirty = true;
}

bool IsStringStartWith(const char* str, const char* subString)
{
    if (subString == NULL || str == NULL)
        return false;
    auto s = strstr(str, subString);
    if (s == NULL)
        return false;
    return true;
}

void String_Release(String* str)
{
    assert(str);
    if (str->m_Buffer)
    {
        free(str->m_Buffer);
        str->m_Buffer = NULL;
    }
    str->length = 0;
    PLUS_ASSIGN(str, m_AllocationCount, 0);
    str->m_Capacity = 0;
    str->m_Dirty = false;
}

bool String_Equals(const String* str, const char* elemPtr)
{
    assert(str);
    if (str->m_Buffer == NULL || elemPtr == NULL)
        return false;
    return strcmp(str->m_Buffer, elemPtr) == 0;
}

bool String_EqualsString(const String* str, const String* other)
{
    assert(str && other);
    return String_Equals(str, other->m_Buffer);
}
