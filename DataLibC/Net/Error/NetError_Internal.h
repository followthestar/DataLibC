//
// Created by 18030 on 2025/9/17.
//

#ifndef NETERROR_INTERNAL_H
#define NETERROR_INTERNAL_H
#include "NetError.h"

typedef struct NetErrorContext
{
    NetError m_LastError;
    const char* m_LastErrorMessage;
    NetErrorCallback m_Callback;
} NetErrorContext;

NetErrorContext* GetNetErrorContext();


void SetLastNetError(NetError error, const char* message);

void SetLastNetErrorFromOS(int errorCode);

#endif //NETERROR_INTERNAL_H
