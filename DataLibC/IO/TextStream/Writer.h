//
// Created by 18030 on 2025/2/20.
//

#ifndef DATALIBC_WRITER_H
#define DATALIBC_WRITER_H

#include "DataLibC/Tool/ExportModuleMacro.h"

///
typedef ABSTRACT LIB_CLASS _Writer Writer;

typedef LIB_VTABLE _WriterVTable WriterVTable;

LIB_VTABLE _WriterVTable
{
    int (* Write)(Writer* self, char buf[]);
};

ABSTRACT LIB_CLASS _Writer
{
    const WriterVTable* vtable;
};

#endif //DATALIBC_WRITER_H
