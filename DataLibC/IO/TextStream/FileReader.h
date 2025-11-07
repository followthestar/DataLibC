//
// Created by 18030 on 2025/2/15.
//

#ifndef DATALIBC_FILEREADER_H
#define DATALIBC_FILEREADER_H

#include "DataLibC/Tool/ExportModuleMacro.h"
#include <stdio.h>

/// FileReader is used to read data from a file.
typedef LIB_CLASS _FileReader FileReader;

/// Create a FileReader object with a file path.
/// \param fileName The file name, or the path of the file.
/// \return A FileReader object.
API_MODULE FileReader* FileReader_Create(const char* fileName);

/// Create a FileReader object with a file.
/// \param file The input file.
/// \return A FileReader object.
API_MODULE FileReader* FileReader_CreateWithFile(FILE* file);

#endif //DATALIBC_FILEREADER_H
