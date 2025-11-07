//
// Created by 18030 on 2025/2/16.
//

#ifndef DATALIBC_BUFFEREDREADER_H
#define DATALIBC_BUFFEREDREADER_H

#include "DataLibC/Tool/ExportModuleMacro.h"
#include "Reader.h"

/// Reads text from a character-input stream, buffering characters so as to
/// provide for the efficient reading of characters, arrays, and lines.
typedef LIB_CLASS _BufferedReader BufferedReader;

/// Creates a buffering character-input stream that uses an input buffer of the specified size.
/// \param in A Reader
/// \param size Input-buffer size
/// \return A BufferedReader
API_MODULE BufferedReader* BufferedReader_CreateSize(Reader* in, int size);

/// Creates a buffering character-input stream that uses a default-sized input buffer.
/// \param in A Reader
/// \return A BufferedReader
API_MODULE BufferedReader* BufferedReader_Create(Reader* in);


#endif //DATALIBC_BUFFEREDREADER_H
