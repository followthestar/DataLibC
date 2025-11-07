//
// Created by 18030 on 2025/2/15.
//

#include <assert.h>
#include <malloc.h>
#include "FileReader.h"
#include "Reader.h"
#include "Reader_InternalMacro.h"

READER_INTERNAL_DECLARE

LIB_CLASS _FileReader
{
    Super super;
    FILE* m_File;
    bool m_EOF;
    const char* m_FileName;
};

static void EnsureOpen(FileReader* self)
{
    assert(self->m_File);
}

static int FileReader_Read(FileReader* self, char buf[], int offset, int len)
{
    EnsureOpen(self);
    if (self->m_EOF)
        return -1;
    if (len == 0)
        return 0;
    auto ret = fread(buf + offset, 1, len, self->m_File);
    if (ret < len)
    {
        if (feof(self->m_File))
            self->m_EOF = true;
        else if (ferror(self->m_File))
            perror("Error reading file");
    }
    return (int) ret;
}

static bool FileReader_Ready(FileReader* self)
{
    EnsureOpen(self);
    return !self->m_EOF && !ferror(self->m_File);
}

static void FileReader_Close(FileReader* self)
{
    fclose(self->m_File);
    SUPERCLOSE
    free(self);
}

extern int Reader_ReadChar(Reader* self);

extern int Reader_Skip(ReaderImpl* self, int n);

static const ReaderVTable FileReader_VTable = {
        .ReadChar = Reader_ReadChar,
        .Read = (ReadFunc) FileReader_Read,
        .Skip = (SkipFunc) Reader_Skip,
        .Ready = (ReadyFunc) FileReader_Ready,
        .Close = (CloseFunc) FileReader_Close
};


static FileReader* FileReader_InternalCreate()
{
    FileReader* reader = (FileReader*) malloc(sizeof(FileReader));
    SUPERINIT
    reader->super.vtable = &FileReader_VTable;
    reader->m_EOF = false;
    reader->m_FileName = NULL;
    return reader;
}

FileReader* FileReader_Create(const char* fileName)
{
    FileReader* reader = FileReader_InternalCreate();
    reader->m_File = fopen(fileName, "r");
    reader->m_FileName = fileName;
    return reader;
}

FileReader* FileReader_CreateWithFile(FILE* file)
{
    assert(file);
    FileReader* reader = FileReader_InternalCreate();
    reader->m_File = file;
    rewind(reader->m_File);
    return reader;
}
