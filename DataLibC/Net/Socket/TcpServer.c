//
// Created by 18030 on 2025/9/28.
//

#include "TcpServer.h"
#include <stdatomic.h>

#include "DataLibC/Thread/Mutex.h"
#include "DataLibC/Thread/ThreadPool.h"

typedef struct TcpServerContext
{
    atomic_bool m_IsRunning;
    void* m_UserData;
    TcpOnConnection m_OnConnection;
    TcpOnMessage m_OnMessage;
    TcpOnError m_OnError;
} TcpServerContext;

typedef struct TcpServer
{
    Socket* m_Listener;
    ThreadPool* m_ThreadPool;
    TcpServerContext m_Context;
    PlatformMutex m_Mutex;
} TcpServer;

TcpServer* TcpServer_Create(const char* ip, int port) { }
