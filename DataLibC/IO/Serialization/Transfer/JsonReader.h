//
// Created by 18030 on 2025/2/20.
//

#ifndef DATALIBC_JSONREADER_H
#define DATALIBC_JSONREADER_H

#include "DataLibC/Tool/ExportModuleMacro.h"
#include "DataLibC/IO/TextStream/Reader.h"

/// Used to read json data from an input text stream.
typedef LIB_CLASS _JsonReader JsonReader;


API_MODULE JsonReader* CreateJsonReader();

#endif //DATALIBC_JSONREADER_H
