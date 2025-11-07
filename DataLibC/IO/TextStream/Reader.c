//
// Created by 18030 on 2025/2/15.
//

#include <malloc.h>
#include <assert.h>
#include "Reader.h"
#include "DataLibC/DS/Algorithm.h"

static const int maxSkipBufferSize = 8192;

INTERNAL struct _ReaderImpl
{
    ReaderVTable* vtable;
    Reader base;

    /** Skip buffer, null until allocated */
    char* skipBuffer;
    int skipBufferSize;
};

typedef struct _ReaderImpl ReaderImpl;

void Reader_Init(ReaderImpl* self)
{
    self->skipBuffer = NULL;
    self->skipBufferSize = 0;
}

Reader* AsReader(void* reader)
{
    return (Reader*) reader;
}

int Reader_ReadChar(Reader* self)
{
    char ch;
    if (self->vtable->Read(self, &ch, 0, 1) == -1)
        return -1;
    return ch;
}

int Reader_ReadAll(Reader* self, char* buf)
{
    int nRead = self->vtable->Read(self, buf, 0, -1);
    return nRead;
}

int Reader_Skip(ReaderImpl* self, int n)
{
    int nn = min(n, maxSkipBufferSize);
    if ((self->skipBuffer == NULL) || (self->skipBufferSize < nn))
    {
        self->skipBuffer = (char*) realloc(self->skipBuffer, sizeof(char) * nn);
        self->skipBufferSize = nn;
    }
    long r = n;
    while (r > 0)
    {
        int nc = self->vtable->Read(AsReader(self), self->skipBuffer, 0, min(r, nn));
        if (nc == -1)
            break;
        r -= nc;
    }
    return n - r;
}

bool Reader_Ready([[maybe_unused]] Reader* self)
{
    return false;
}

void Reader_Mark(Reader * self, int readAheadLimit)
{
    assert(false);
}

void Reader_Reset(Reader* self)
{
    assert(false);
}

void Reader_Close(ReaderImpl* self)
{
    if (self->skipBuffer)
        free(self->skipBuffer);
}

const ReaderVTable g_DefaultReaderVTable = {
        .ReadChar = Reader_ReadChar,
        .Read = NULL,
        .Skip = (SkipFunc) Reader_Skip,
        .Ready = Reader_Ready,
        .Reset = Reader_Reset,
        .Mark = Reader_Mark,
        .Close = (CloseFunc)Reader_Close
};


