//
// Created by 18030 on 2025/9/28.
//

#include "UdpServer.h"

#include <assert.h>
#include <pthread.h>

#include "Socket_Internal.h"
#include "DataLibC/IO/Print.h"
#include "DataLibC/Net/Error/NetError.h"
#include "DataLibC/Net/IPAddress/IPAddress_Internal.h"
#include "DataLibC/Thread/ThreadPool.h"
#include <stdatomic.h>

#include "DataLibC/Thread/Mutex.h"

typedef struct UdpServerContext
{
    atomic_bool m_IsRunning;
    void* m_UserData;
    UdpOnReceive m_OnReceive;
} UdpServerContext;

typedef struct UdpServer
{
    Socket* m_Socket;
    ThreadPool* m_ThreadPool;
    UdpServerContext m_Context;
    PlatformMutex m_Mutex;
} UdpServer;

UdpServer* UdpServer_Create(const char* ip, const int port)
{
    UdpServer* server = malloc(sizeof(UdpServer));
    server->m_Socket = CreateSocket(IPv4, UDP);
    if (!BindSocket(server->m_Socket, ip, port))
        return nullptr;
    SetSocketIsBlock(server->m_Socket, false);
    server->m_Context.m_UserData = nullptr;
    server->m_Context.m_OnReceive = nullptr;
    server->m_ThreadPool = nullptr;
    atomic_init(&server->m_Context.m_IsRunning, false);
    InitPlatformMutex(server->m_Mutex);
    return server;
}

void UdpServer_SetOnReceiveCallback(UdpServer* server, const UdpOnReceive callback, void* userData)
{
    if (atomic_load(&server->m_Context.m_IsRunning))
        return;

    server->m_Context.m_OnReceive = callback;
    server->m_Context.m_UserData = userData;
}

static void* UdpWorker(void* args)
{
    const UdpServer* this = args;

    char buffer[1024] = { };
    int recvSize;
    char outIP[64];
    int outPort;

    while (atomic_load(&this->m_Context.m_IsRunning))
    {
        if (ReceiveFrom(this->m_Socket, buffer, sizeof(buffer), &recvSize, outIP, &outPort))
        {
            const UdpOnReceive callback = this->m_Context.m_OnReceive;
            void* userData = this->m_Context.m_UserData;
            if (callback != nullptr)
                callback(buffer, recvSize, outIP, outPort, userData);
        }
    }
    return nullptr;
}

#define MAX_THREADS 16
#define MIN_THREADS 4

bool UdpServer_Start(UdpServer* server, int threadCount)
{
    if (atomic_load(&server->m_Context.m_IsRunning))
        return false;

    if (threadCount < MIN_THREADS)
        threadCount = MIN_THREADS;

    if (threadCount > MAX_THREADS)
        threadCount = MAX_THREADS;

    server->m_ThreadPool = ThreadPool_Create(threadCount);
    if (server->m_ThreadPool == nullptr)
        return false;

    atomic_store(&server->m_Context.m_IsRunning, true);
    for (int i = 0; i < threadCount; i++)
    {
        ThreadPool_Submit(server->m_ThreadPool, UdpWorker, server);
    }

    return true;
}

void UdpServer_Stop(UdpServer* server)
{
    if (!atomic_load(&server->m_Context.m_IsRunning))
        return;
    atomic_store(&server->m_Context.m_IsRunning, false);
    ThreadPool_Destroy(server->m_ThreadPool);
}

void UdpServer_Destroy(UdpServer* server)
{
    assert(atomic_load(&server->m_Context.m_IsRunning) == false);

    DeleteSocket(server->m_Socket);
    free(server);
}
