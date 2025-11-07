//
// Created by 18030 on 2025/9/17.
//

#ifndef NETERROR_H
#define NETERROR_H
#include "DataLibC/Tool/ExportModuleMacro.h"

typedef enum NetError
{
    NoError = 0,
    SocketFailed,
    BindFailed,
    ConnectTimeout,
    InvalidAddress,
    NetOutOfMemory,
    CloseError,
    InvalidSocket,
    InvalidPort,
    InvalidState,
    ConnectFailed,
    InvalidData
} NetError;

typedef void (* NetErrorCallback)(NetError error, const char* message);

API_MODULE bool IsNetErrorOccured();

API_MODULE NetError GetLastNetError();

API_MODULE const char* GetNetErrorMessage();

API_MODULE void SetNetErrorCallback(NetErrorCallback callback);

API_MODULE int GetLastNetErrorCode();

#endif //NETERROR_H
