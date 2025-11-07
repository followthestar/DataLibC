//
// Created by 18030 on 2025/2/20.
//
#include <stdlib.h>
#include "ITransfer.h"
#include "DataLibC/IO/Serialization/Transfer/JsonReader.h"
#include "DataLibC/IO/Serialization/Transfer/JsonWriter.h"

ITransfer* CreateITransfer(TransferType type, TransferDirection direction)
{
    if (direction == TransferDirection_Read)
    {
        switch (type)
        {
            case TransferType_JSON:
                return (ITransfer*) CreateJsonReader();
            case TransferType_YAML:
                break;
            case TransferType_BINARY:
                break;
        }
    }
    else if (direction == TransferDirection_Write)
    {
        switch (type)
        {
            case TransferType_JSON:
                return (ITransfer*) CreateJsonWriter();
            case TransferType_YAML:
                break;
            case TransferType_BINARY:
                break;
        }
    }
    return NULL;
}
