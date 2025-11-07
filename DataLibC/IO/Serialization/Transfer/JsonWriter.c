//
// Created by 18030 on 2025/2/20.
//

#include <assert.h>
#include <malloc.h>
#include "JsonWriter.h"
#include "Interface/ITransfer.h"
#include "DataLibC/IO/TextStream/Writer.h"

typedef ITransfer Super;


struct _JsonWriter
{
    Super super;
    Writer* out;
};

static void JsonWriter_Tranfer(ITransfer* self, const char* name, void* object, TransferCallback callback)
{


    callback(object, self);
}

static void JsonWriter_Init_Writer(ITransfer* self, void* writer, size_t startOffset, size_t endOffset)
{
    assert(writer);
    JsonWriter* jsonWriter = (JsonWriter*) self;
    jsonWriter->out = (Writer*) writer;
}


const static ITransferVTable JsonReader_VTable = {
        .InitDataCache = JsonWriter_Init_Writer,
        .Transfer = JsonWriter_Tranfer,
};

JsonWriter* CreateJsonWriter()
{
    JsonWriter* self = (JsonWriter*) malloc(sizeof(JsonWriter));
    self->super.vtable = &JsonReader_VTable;

    return self;
}