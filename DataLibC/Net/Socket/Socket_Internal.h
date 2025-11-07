//
// Created by 18030 on 2025/9/17.
//

#ifndef SOCKET_INTERNAL_H
#define SOCKET_INTERNAL_H

#include "Socket.h"
#include "DataLibC/Net/IPAddress/IPAddress.h"
#include "DataLibC/Tool/ExportModuleMacro.h"
#if LIB_WIN
#include <winsock.h>

typedef SOCKET SocketHandle;
#endif


typedef struct Socket
{
    SocketHandle m_Fd;
    IpVersion m_IpVersion;
    TransportProtocol m_TransportProtocol;

    IPAddress* m_CachedAddress;
    int m_CachedPort;
    SocketState m_State;

    int m_SendTimeout;
    int m_RecvTimeout;


    bool m_IsBlocking;
    bool m_HasClose;
} Socket;

#endif //SOCKET_INTERNAL_H
