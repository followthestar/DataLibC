//
// Created by 18030 on 2025/9/17.
//

#include "NetError.h"

#if LIB_WIN
#include <winsock2.h>
#endif

#include "NetError_Internal.h"


bool IsNetErrorOccured()
{
    return GetNetErrorContext()->m_LastError != NoError;
}

NetError GetLastNetError()
{
    return GetNetErrorContext()->m_LastError;
}

const char* GetNetErrorMessage()
{
    return GetNetErrorContext()->m_LastErrorMessage;
}

void SetNetErrorCallback(const NetErrorCallback callback)
{
    GetNetErrorContext()->m_Callback = callback;
}

int GetLastNetErrorCode()
{
#if LIB_WIN
    return WSAGetLastError();
#else
    return errno;
#endif
}
