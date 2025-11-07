//
// Created by 18030 on 2025/9/17.
//


#include "NetError_Internal.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <winerror.h>

#include "DataLibC/DS/Allocation.h"
#include "DataLibC/Tool/LogMacro.h"

static NetErrorContext* s_NetErrorContext = NULL;

static NetError MapWsaErrorToNetError(int errorCode)
{
    switch (errorCode)
    {
        case 0:
            return NoError;

        case WSAEACCES:
        case WSAEADDRINUSE:
        case WSAEADDRNOTAVAIL:
            return BindFailed;

        case WSAECONNREFUSED:
            return ConnectFailed;

        case WSAETIMEDOUT:
            return ConnectTimeout;

        case WSAEAFNOSUPPORT:
        case WSAEPROTONOSUPPORT:
            return InvalidSocket;

        case WSAEINVAL:
        case WSAEDESTADDRREQ:
            return InvalidArgument;

        case WSAENETDOWN:
        case WSAENETUNREACH:
        case WSAEHOSTDOWN:
        case WSAEHOSTUNREACH:
            return SocketFailed;

        case WSAEMFILE:
        case WSAENOBUFS:
            return NetOutOfMemory;

        default:
            return SocketFailed;
    }
}

static const char* GetWsaErrorString(int wsaError)
{
    switch (wsaError)
    {
        case WSAEADDRINUSE:
            return "Address already in use";
        case WSAECONNREFUSED:
            return "Connection refused";
        case WSAETIMEDOUT:
            return "Connection timed out";
        case WSAEACCES:
            return "Permission denied (are you binding to port < 1024?)";
        case WSAEADDRNOTAVAIL:
            return "Cannot assign requested address";
        case WSAENETDOWN:
            return "Network is down";
        case WSAENOBUFS:
            return "No buffer space available";
        case WSAEINVAL:
            return "Invalid argument";
        default:
            return "Winsock system error";
    }
}

NetErrorContext* GetNetErrorContext()
{
    if (s_NetErrorContext == NULL)
    {
        s_NetErrorContext = (NetErrorContext*)malloc(sizeof(NetErrorContext));
        BASE_INIT(NetErrorContext, s_NetErrorContext);
    }
    return s_NetErrorContext;
}

void SetLastNetError(const NetError error, const char* message)
{
    NetErrorContext* context = GetNetErrorContext();
    context->m_LastError = error;
    context->m_LastErrorMessage = message;
    if (context->m_Callback && error != NoError)
        context->m_Callback(context->m_LastError, context->m_LastErrorMessage);
}

void SetLastNetErrorFromOS(const int errorCode)
{
    if (errorCode == 0 || errorCode == WSAEWOULDBLOCK)
    {
        SetLastNetError(NoError, NULL);
        return;
    }

    const NetError netError = MapWsaErrorToNetError(errorCode);
    const char* message = GetWsaErrorString(errorCode);

    SetLastNetError(netError, message);
}
