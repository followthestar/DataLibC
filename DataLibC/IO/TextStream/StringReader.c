//
// Created by 18030 on 2025/2/14.
//

#include <malloc.h>
#include <string.h>
#include <assert.h>
#include "StringReader.h"
#include "Reader.h"
#include "DataLibC/DS/Algorithm.h"
#include "Reader_InternalMacro.h"

READER_INTERNAL_DECLARE

LIB_CLASS _StringReader
{
    Super super;
    const char* m_Src;
    int m_Length;
    int m_Next;
};

static void EnsureOpen(StringReader* self)
{
    assert(self->m_Src);
}

static int StringReader_ReadChar(StringReader* self)
{
    EnsureOpen(self);
    if (self->m_Next >= self->m_Length)
        return -1;
    return self->m_Src[self->m_Next++];
}

static int StringReader_Read(StringReader* self, char buf[], int offset, int len)
{
    EnsureOpen(self);
    if (len == 0)
        return 0;
    if (len == -1)
        return StringReader_ReadAll(self, buf);
    if (self->m_Next >= self->m_Length)
        return -1;
    int n = min(self->m_Length - self->m_Next, len);
    strncpy(buf + offset, self->m_Src + self->m_Next, n);
    self->m_Next += n;
    return n;
}

static int StringReader_Skip(StringReader* self, int n)
{
    EnsureOpen(self);
    if (self->m_Next >= self->m_Length)
        return 0;
    // Bound skip by beginning and end of the source
    long r = min(self->m_Length - self->m_Next, n);
    r = max(-self->m_Next, r);
    self->m_Next += (int) r;
    return r;
}

static bool StringReader_Ready(StringReader* self)
{
    EnsureOpen(self);
    return true;
}

static void StringReader_Close(StringReader* self)
{
    self->m_Src = NULL;
    SUPERCLOSE
    free(self);
}

static ReaderVTable StringReader_VTable = {
        .ReadChar = (ReadCharFunc) StringReader_ReadChar,
        .Read = (ReadFunc) StringReader_Read,
        .Skip = (SkipFunc) StringReader_Skip,
        .Ready = (ReadyFunc) StringReader_Ready,
        .Close = (CloseFunc) StringReader_Close
};


StringReader* StringReader_Create(const char* str)
{
    StringReader* reader = (StringReader*) malloc(sizeof(StringReader));
    SUPERINIT
    reader->super.vtable = &StringReader_VTable;
    reader->m_Src = str;
    reader->m_Length = (int) strlen(str);
    reader->m_Next = 0;
    return reader;
}


int StringReader_ReadAll(StringReader* self, char buf[])
{
    EnsureOpen(self);
    int n = self->m_Length - self->m_Next;
    if (n <= 0)
        return -1;
    strncpy(buf, self->m_Src + self->m_Next, n);
    self->m_Next += n;
    return n;
}