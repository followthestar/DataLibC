//
// Created by 18030 on 2025/2/14.
//

#ifndef DATALIBC_STRINGREADER_H
#define DATALIBC_STRINGREADER_H

#include "DataLibC/Tool/ExportModuleMacro.h"

/// \brief StringReader is used to read from a string.
typedef LIB_CLASS _StringReader StringReader;

/// \brief Create a StringReader.
/// \param str The original char array.
/// \return A StringReader object.
API_MODULE StringReader* StringReader_Create(const char* str);

API_MODULE int StringReader_ReadAll(StringReader* self, char buf[]);

#endif //DATALIBC_STRINGREADER_H
