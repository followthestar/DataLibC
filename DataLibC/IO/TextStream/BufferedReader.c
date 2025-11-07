//
// Created by 18030 on 2025/2/16.
//

#include <malloc.h>
#include <assert.h>
#include <memory.h>
#include "BufferedReader.h"
#include "Reader.h"
#include "Reader_InternalMacro.h"
#include "DataLibC/DS/Algorithm.h"

READER_INTERNAL_DECLARE

static const int INVALIDATED = -2;
static const int UNMARKED = -1;
static const int DEFAULT_CHAR_BUFFER_SIZE = 8192;
static const int DEFAULT_EXPECTED_LINE_LENGTH = 80;

LIB_CLASS _BufferedReader
{
    Super super;
    Reader* m_In;
    char* m_CharBlock;
    int m_CBlockSize;
    int m_NChars;
    int m_NextChar;
    int m_MarkedChar;
    int m_ReadAheadLimit; /* Valid only when markedChar > 0 */

    /** If the next character is a line feed, skip it */
    bool m_SkipLF;

    /** The skipLF flag when the mark was set */
    bool m_MarkedSkipLF;
};

static void EnsureOpen(BufferedReader* self)
{
    assert(self->m_In);
}

/** Fills the input buffer, taking the mark into account if it is valid. */
static void Fill(BufferedReader* self)
{
    /* Default no mark */
    int dst = 0;
    if (self->m_MarkedChar > UNMARKED)
    {
        int delta = self->m_NextChar - self->m_MarkedChar;
        if (delta >= self->m_ReadAheadLimit)
        {
            /* Gone past read-ahead limit: Invalidate mark */
            self->m_MarkedChar = INVALIDATED;
            self->m_ReadAheadLimit = 0;
        }
        else
        {
            if (self->m_ReadAheadLimit <= self->m_CBlockSize)
            {
                memcpy(self->m_CharBlock, self->m_CharBlock + self->m_MarkedChar, delta);
                self->m_MarkedChar = 0;
            }
            else
            {
                /* Reallocate buffer to accommodate read-ahead limit */
                char* ncb = malloc(sizeof(char) * self->m_ReadAheadLimit);
                memcpy(ncb, self->m_CharBlock + self->m_MarkedChar, delta);
                free(self->m_CharBlock);
                self->m_CharBlock = ncb;
                self->m_CBlockSize = self->m_ReadAheadLimit;
                self->m_MarkedChar = 0;
            }
            dst = delta;
            self->m_NextChar = self->m_NChars = delta;
        }
    }
    int n;
    do
    {
        n = self->m_In->vtable->Read(self->m_In, self->m_CharBlock, dst, self->m_CBlockSize - dst);
    } while (n == 0);
    if (n > 0)
    {
        self->m_NChars = dst + n;
        self->m_NextChar = dst;
    }
}

static int BufferedReader_ReadChar(BufferedReader* self)
{
    EnsureOpen(self);
    for (;;)
    {
        if (self->m_NextChar >= self->m_NChars)
        {
            Fill(self);
            if (self->m_NextChar >= self->m_NChars)
                return -1;
        }
        if (self->m_SkipLF)
        {
            self->m_SkipLF = false;
            if (self->m_CharBlock[self->m_NextChar] == '\n')
            {
                self->m_NextChar++;
                continue;
            }
        }
        return self->m_CharBlock[self->m_NextChar++];
    }
}

static bool BufferedReader_Ready(BufferedReader* self)
{
    EnsureOpen(self);
    /*
     * If newline needs to be skipped and the next char to be read
     * is a newline character, then just skip it right away.
     */
    if (self->m_SkipLF)
    {
        /* Note that in.ready() will return true if and only if the next
         * read on the stream will not block.
         */
        if (self->m_NextChar >= self->m_NChars && self->m_In->vtable->Ready(self->m_In))
        {
            Fill(self);
        }
        if (self->m_NextChar < self->m_NChars)
        {
            if (self->m_CharBlock[self->m_NextChar] == '\n')
                self->m_NextChar++;
            self->m_SkipLF = false;
        }
    }
    return (self->m_NextChar < self->m_NChars) || self->m_In->vtable->Ready(self->m_In);
}

static int ReadInternal(BufferedReader* self, char buf[], int off, int len)
{
    if (self->m_NextChar >= self->m_NChars)
    {
        /* If the requested length is at least as large as the buffer, and
           if there is no mark/reset activity, and if line feeds are not
           being skipped, do not bother to copy the characters into the
           local buffer.  In this way buffered streams will cascade
           harmlessly. */
        if (len >= self->m_CBlockSize && self->m_MarkedChar <= UNMARKED && !self->m_SkipLF)
        {
            return self->m_In->vtable->Read(self->m_In, buf, off, len);
        }
        Fill(self);
    }
    if (self->m_NextChar >= self->m_NChars)
        return -1;
    if (self->m_SkipLF)
    {
        self->m_SkipLF = false;
        if (self->m_CharBlock[self->m_NextChar] == '\n')
        {
            self->m_NextChar++;
            if (self->m_NextChar >= self->m_NChars)
                Fill(self);
            if (self->m_NextChar >= self->m_NChars)
                return -1;
        }
    }
    int n = min(len, self->m_NChars - self->m_NextChar);
    memcpy(buf + off, self->m_CharBlock + self->m_NextChar, n);
    self->m_NextChar += n;
    return n;
}

static int BufferedReader_Read(BufferedReader* self, char buf[], int off, int len)
{
    EnsureOpen(self);
    if (len == 0)
        return 0;

    int n = ReadInternal(self, buf, off, len);
    if (n <= 0) return n;
    while ((n < len) && self->m_In->vtable->Ready(self->m_In))
    {
        int n1 = ReadInternal(self, buf, off + n, len - n);
        if (n1 <= 0) break;
        n += n1;
    }
    return n;
}

static int BufferedReader_Skip(BufferedReader* self, int n)
{
    assert(n >= 0);
    EnsureOpen(self);
    int r = n;
    while (r > 0)
    {
        if (self->m_NextChar >= self->m_NChars)
            Fill(self);
        if (self->m_NextChar >= self->m_NChars) /* EOF */
            break;
        if (self->m_SkipLF)
        {
            self->m_SkipLF = false;
            if (self->m_CharBlock[self->m_NextChar] == '\n')
            {
                self->m_NextChar++;
            }
        }
        int d = self->m_NChars - self->m_NextChar;
        if (r <= d)
        {
            self->m_NextChar += r;
            r = 0;
            break;
        }
        else
        {
            r -= d;
            self->m_NextChar = self->m_NChars;
        }
    }
    return n - r;
}

static void BufferedReader_Mark(BufferedReader* self, int readAheadLimit)
{
    assert(readAheadLimit >= 0);
    EnsureOpen(self);
    self->m_ReadAheadLimit = readAheadLimit;
    self->m_MarkedChar = self->m_NextChar;
    self->m_MarkedSkipLF = self->m_SkipLF;
}

static void BufferedReader_Reset(BufferedReader* self)
{
    EnsureOpen(self);
    assert(self->m_MarkedChar >= 0);
    self->m_NextChar = self->m_MarkedChar;
    self->m_SkipLF = self->m_MarkedSkipLF;
}

static void BufferedReader_Close(BufferedReader* self)
{
    if (self->m_In == NULL)
        return;
    self->m_In->vtable->Close(self->m_In);
    self->m_In = NULL;
    free(self->m_CharBlock);
    self->m_CharBlock = NULL;
    SUPERCLOSE
    free(self);
}

static const ReaderVTable BufferedReader_VTable = {
        .ReadChar = (ReadCharFunc) BufferedReader_ReadChar,
        .Read = (ReadFunc) BufferedReader_Read,
        .Skip = (SkipFunc) BufferedReader_Skip,
        .Ready = (ReadyFunc) BufferedReader_Ready,
        .Reset = (ResetFunc) BufferedReader_Reset,
        .Mark = (MarkFunc) BufferedReader_Mark,
        .Close = (CloseFunc) BufferedReader_Close
};


BufferedReader* BufferedReader_CreateSize(Reader* in, int size)
{
    assert(in);
    assert(size > 0);
    BufferedReader* reader = (BufferedReader*) malloc(sizeof(BufferedReader));
    SUPERINIT
    reader->super.vtable = &BufferedReader_VTable;
    reader->m_In = in;
    reader->m_CharBlock = malloc(sizeof(char) * size);
    reader->m_CBlockSize = size;
    reader->m_NChars = 0;
    reader->m_NextChar = 0;
    reader->m_MarkedChar = UNMARKED;
    reader->m_ReadAheadLimit = 0;
    reader->m_SkipLF = false;
    reader->m_MarkedSkipLF = false;
    return reader;
}

BufferedReader* BufferedReader_Create(Reader* in)
{
    return BufferedReader_CreateSize(in, DEFAULT_CHAR_BUFFER_SIZE);
}
