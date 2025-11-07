//
// Created by 18030 on 2025/2/14.
//

#ifndef DATALIBC_ITRANSFER_H
#define DATALIBC_ITRANSFER_H

#include "DataLibC/Tool/ExportModuleMacro.h"
#include "DataLibC/IO/TextStream/Reader.h"

typedef LIB_INTERFACE _ITransfer ITransfer;
typedef LIB_VTABLE _ITransferVTable ITransferVTable;

typedef void (* TransferCallback)(void* object, ITransfer* transfer);

LIB_VTABLE _ITransferVTable
{
    /// Initialize the transfer object with given data cache
    /// \param self The Transfer object
    /// \param dataProvider The data provider, which is used to read/write data.
    /// \attention Must call Init() before Transfer()
    void (* InitDataCache)(ITransfer* self, void* dataCache, size_t startOffset, size_t endOffset);

    /// Initialize the transfer object with given string
    /// \param self The Transfer object
    /// \param string The target data buffer
    void (* InitString)(ITransfer* self, const char* string, size_t length);

    /// Transfer the object with given transfer callback method
    /// \param self The Transfer object
    /// \param object The object to be transfer
    /// \param callback The transfer callback method, defining the process of transferring that kind of object.
    void (* Transfer)(ITransfer* self, const char* name, void* object, TransferCallback callback);

    /// Transfer an int32 value
    /// \param self The Transfer object
    /// \param value The int32 value to be transfer
    void (* TransferInt32)(ITransfer* self, const char* name, int* value);

    /// Transfer an int64 value
    /// \param self The Transfer object
    /// \param value The int64 value to be transfer
    void (* TransferInt64)(ITransfer* self, const char* name, long long* value);

    /// Transfer a float value
    /// \param self The Transfer object
    /// \param value The float value to be transfer
    void (* TransferFloat)(ITransfer* self, const char* name, float* value);

    /// Transfer a string value
    /// \param self The Transfer object
    /// \param name The name of the string
    /// \param value The string buffer to be filled
    void (* TransferString)(ITransfer* self, const char* name, char* value);

    /// Transfer an array with custom transfer callback method
    /// \param self The Transfer object
    /// \param name The name of the array
    /// \param array The array to be transfer. It can be whatever you want, just match the transfer callback method.
    /// \param callback The transfer callback method, defining the process of transferring that kind of object.
    void (* TransferArray)(ITransfer* self, const char* name, void* array, TransferCallback callback);

    /// Get the length of the array if the node with given name is an array.
    /// \param self The Transfer object
    /// \param name The name of the array
    /// \return The length of the array if the node is really an array, otherwise -1.
    int (* GetArrayLength)(ITransfer* self, const char* name);
};

LIB_INTERFACE _ITransfer
{
    const ITransferVTable* vtable;
};

typedef enum TransferDirection
{
    TransferDirection_Read,
    TransferDirection_Write
} TransferDirection;

typedef enum TransferType
{
    TransferType_JSON,
    TransferType_YAML,
    TransferType_BINARY,
} TransferType;

API_MODULE ITransfer* CreateITransfer(TransferType type, TransferDirection direction);

#endif //DATALIBC_ITRANSFER_H
