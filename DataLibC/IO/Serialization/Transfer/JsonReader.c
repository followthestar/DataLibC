//
// Created by 18030 on 2025/2/20.
//

#include <malloc.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "JsonReader.h"
#include "Interface/ITransfer.h"
#include "DataLibC/IO/TextStream/Reader.h"
#include "DataLibC/IO/Serialization/Transfer/Internal/Json.h"
#include "DataLibC/Tool/Misc.h"

typedef ITransfer Super;


struct _JsonReader
{
    Super super;
    bool m_HasInit;
    union
    {
        const char* string;
        Reader* in;
    } m_ReadData;

    int m_CurrentVersion;

    JsonDocument m_Document;
    JsonDocument* m_ActiveDocument;
    JsonNode* m_CurrentNode;

    bool m_DidReadLastProperty;

    JsonReadHandler m_ReadHandler;
    size_t m_ReadOffset;
    size_t m_EndOffset;
};

static void JsonTransferBasic_Int32(void* data, ITransfer* transfer)
{
    *(int*) data = strtol(((JsonReader*) transfer)->m_CurrentNode->value.scalar.data, NULL, 10);
}

static void JsonTransferBasic_Float(void* data, ITransfer* transfer)
{
    *(float*) data = strtof(((JsonReader*) transfer)->m_CurrentNode->value.scalar.data, NULL);
}

static void JsonTransferBasic_String(void* data, ITransfer* transfer)
{
    char* str = (char*) data;
    strcpy(str, ((JsonReader*) transfer)->m_CurrentNode->value.scalar.data);
}

static bool JsonReadStringHandler(void* data, unsigned char* buffer, size_t size, size_t* size_read)
{
    JsonReader* read = (JsonReader*) data;

    if (read->m_ReadOffset >= read->m_EndOffset)
        return false;

    if (read->m_ReadOffset + size > read->m_EndOffset)
        size = read->m_EndOffset - read->m_ReadOffset;

    const char* readData = read->m_ReadData.string;

    memcpy(buffer, readData + read->m_ReadOffset, size);
    read->m_ReadOffset += size;
    *size_read = size;

    return true;
}

static JsonNode* GetValueForKey(JsonNode* parentNode, const char* key)
{

    if (parentNode && parentNode->type == JSON_OBJECT_NODE)
    {
        if (parentNode->value.mapping.length == 0)
            return NULL;

        for (int i = 0; i < parentNode->value.mapping.length; ++i)
        {
            JsonNode* curr = *(JsonNode**) DynamicArray_ValueAt(&parentNode->value.mapping, i);
            if (strcmp(curr->key, key) == 0)
                return curr;
        }
    }

    // case for index key
    if (parentNode && parentNode->type == JSON_ARRAY_NODE)
    {
        int index;
        if (!StringIsInteger(key, &index))
            return NULL;
        if (index >= 0 && index < parentNode->value.array.length)
            return *(JsonNode**) DynamicArray_ValueAt(&parentNode->value.array, index);
    }

    return NULL;
}

static int JsonReader_GetArrayLength(ITransfer* self, const char* name)
{
    JsonReader* this = (JsonReader*) self;
    auto node = GetValueForKey(this->m_CurrentNode, name);
    if (!node)
        return -1;
    if (node->type == JSON_ARRAY_NODE)
        return (int) node->value.array.length;
    return -1;
}

static bool JsonReadTextReaderHandler(void* data, unsigned char* buffer, size_t size, size_t* size_read)
{
    JsonReader* read = (JsonReader*) data;

    if (read->m_ReadOffset >= read->m_EndOffset)
        return false;

    if (read->m_ReadOffset + size > read->m_EndOffset)
        size = read->m_EndOffset - read->m_ReadOffset;

    int ret = read->m_ReadData.in->vtable->Read(read->m_ReadData.in, (char*) buffer, 0, (int) size);
    if (ret == -1)
        return false;
    read->m_ReadOffset += ret;
    *size_read = size;

    return true;
}

static void JsonReader_InitInternal(JsonReader* self)
{
    self->m_HasInit = false;
    self->m_Document = (JsonDocument) {};

    JsonParser parser = {};
    if (!JsonParserInitialize(&parser))
    {
        fprintf(stderr, "Could not initialize json parser!\n");
        return;
    }

    JsonParserSetInput(&parser, self->m_ReadHandler, self);
    JsonParserLoad(&parser, &self->m_Document);

    if (parser.error != JSON_NO_ERROR)
    {
        fprintf(stderr, "Unable to parse json file: [%s] at line %zu!\n", parser.problem, parser.problemMark.line);
    }

    JsonParserDelete(&parser);

    self->m_ActiveDocument = &self->m_Document;
    self->m_DidReadLastProperty = false;
    self->m_CurrentNode = JsonDocumentGetRootNode(&self->m_Document);
    self->m_HasInit = true;
}


static void JsonReader_Transfer(ITransfer* self, const char* name, void* object, TransferCallback callback)
{
    JsonReader* this = (JsonReader*) self;
    this->m_DidReadLastProperty = false;

    JsonNode* parentNode = this->m_CurrentNode;
    this->m_CurrentNode = GetValueForKey(parentNode, name);

    // Cannot find node starting from the cached node, try to find it from the root node
    if (!this->m_CurrentNode)
    {
        parentNode = JsonDocumentGetRootNode(&this->m_Document);
        this->m_CurrentNode = GetValueForKey(parentNode, name);
    }

    if (this->m_CurrentNode != NULL)
    {

        callback(object, self);
        this->m_DidReadLastProperty = true;
    }
    this->m_CurrentNode = parentNode;
}

static void
JsonReader_TransferArrayAtIndex(ITransfer* self, int index, void* object, TransferCallback callback)
{
    JsonReader* this = (JsonReader*) self;
    auto parent = this->m_CurrentNode;
    this->m_CurrentNode = *(JsonNode**) DynamicArray_ValueAt(&parent->value.array, index);
    callback(object, self);
    this->m_CurrentNode = parent;
}

static void JsonReader_TransferArray(ITransfer* self, const char* name, void* array, TransferCallback callback)
{
    JsonReader* this = (JsonReader*) self;
    this->m_DidReadLastProperty = false;

    JsonNode* parentNode = this->m_CurrentNode;
    this->m_CurrentNode = GetValueForKey(parentNode, name);

    if (!this->m_CurrentNode)
    {
        parentNode = JsonDocumentGetRootNode(&this->m_Document);
        this->m_CurrentNode = GetValueForKey(parentNode, name);
    }

    if (this->m_CurrentNode != NULL && this->m_CurrentNode->type == JSON_ARRAY_NODE)
    {
        for (int i = 0; i < this->m_CurrentNode->value.array.length; ++i)
        {
            JsonReader_TransferArrayAtIndex(self, i, array, callback);
        }
    }
    this->m_CurrentNode = parentNode;
}


static void JsonReader_TransferInt32(ITransfer* self, const char* name, int* value)
{
    JsonReader_Transfer(self, name, value, JsonTransferBasic_Int32);
}

static void JsonReader_TransferFloat(ITransfer* self, const char* name, float* value)
{
    JsonReader_Transfer(self, name, value, JsonTransferBasic_Float);
}

static void JsonReader_TransferString(ITransfer* self, const char* name, char* value)
{
    JsonReader_Transfer(self, name, value, JsonTransferBasic_String);
}

static void JsonReader_Init_Char(ITransfer* self, const char* string, size_t length)
{
    assert(string);
    JsonReader* jsonReader = (JsonReader*) self;
    if (jsonReader->m_HasInit)
        return;
    jsonReader->m_ReadData.string = string;
    jsonReader->m_ReadOffset = 0;
    jsonReader->m_EndOffset = length;
    jsonReader->m_ReadHandler = JsonReadStringHandler;
    JsonReader_InitInternal(jsonReader);


}


static void JsonReader_Init_Reader(ITransfer* self, void* reader, size_t startOffset, size_t endOffset)
{
    assert(reader);
    JsonReader* jsonReader = (JsonReader*) self;
    if (jsonReader->m_HasInit)
        return;
    jsonReader->m_ReadData.in = (Reader*) reader;
    jsonReader->m_ReadOffset = startOffset;
    jsonReader->m_EndOffset = endOffset;
    jsonReader->m_ReadHandler = JsonReadTextReaderHandler;
    JsonReader_InitInternal(jsonReader);

}


const static ITransferVTable JsonReader_VTable = {
        .InitString = JsonReader_Init_Char,
        .InitDataCache = JsonReader_Init_Reader,
        .Transfer = JsonReader_Transfer,
        .TransferInt32 = JsonReader_TransferInt32,
        .TransferFloat = JsonReader_TransferFloat,
        .TransferString = JsonReader_TransferString,
        .TransferArray = JsonReader_TransferArray,
        .GetArrayLength = JsonReader_GetArrayLength,
};

JsonReader* CreateJsonReader()
{
    JsonReader* self = (JsonReader*) malloc(sizeof(JsonReader));
    self->super.vtable = &JsonReader_VTable;
    self->m_HasInit = false;
    return self;
}
