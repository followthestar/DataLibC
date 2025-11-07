//
// Created by 18030 on 2025/9/17.
//

#include "Socket.h"

#include <stddef.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "Socket_Internal.h"
#include "DataLibC/IO/Print.h"
#include "DataLibC/Net/Error/NetError_Internal.h"
#include "DataLibC/Net/IPAddress/IPAddress_Internal.h"

static int s_AddressFamilyMap[] = { AF_INET, AF_INET6 };
static int s_ProtocolMap[] = { SOCK_STREAM, SOCK_DGRAM };

static bool CheckPortNumber(int port)
{
    return port >= 0 && port < 65536;
}

static bool s_HasInit = false;
static bool s_HasCleanup = false;

#ifdef LIB_WIN
#define MAX_WORKER_THREADS 4
// IOCP
static HANDLE g_Iocp = NULL;
static int g_ThreadCount = 0;
#endif

static bool InitIOCP()
{
    if (g_Iocp != NULL)
        return true;

    g_Iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL,
                                    0, 0);
    if (g_Iocp == NULL)
    {
        SetLastNetErrorFromOS(WSAGetLastError());
        return false;
    }

    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    g_ThreadCount = (int)sysInfo.dwNumberOfProcessors;
    if (g_ThreadCount > MAX_WORKER_THREADS)
        g_ThreadCount = MAX_WORKER_THREADS;

    //TODO ?
    return true;
}


static void CleanupService()
{
    if (s_HasInit && !s_HasCleanup)
    {
        WSACleanup();
        s_HasCleanup = true;
        print_any("Clean up net resources...");
    }
}

static void CheckServiceInit()
{
    if (!s_HasInit)
    {
        WSADATA wsaData;
        int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (result != 0)
        {
            print_any("OS net service is failed to start up!", result);
            return;
        }
        s_HasInit = true;
        atexit(CleanupService);
    }
}


static bool GenerateHandle(Socket* this, const IpVersion version, const TransportProtocol type)
{
    CheckServiceInit();

    memset(this, 0, sizeof(Socket));
    this->m_IpVersion = version;
    this->m_TransportProtocol = type;
    this->m_Fd = INVALID_SOCKET;

    const int af = s_AddressFamilyMap[version];
    const int protocol = s_ProtocolMap[type];

    const SocketHandle fd = socket(af, protocol, 0);
    if (fd == INVALID_SOCKET)
        return false;
    this->m_Fd = fd;


    return true;
}

Socket* CreateSocket(const IpVersion version, const TransportProtocol type)
{
    Socket* socket = malloc(sizeof(Socket));
    if (socket == NULL)
    {
        SetLastNetError(NetOutOfMemory, "Out of memory when creating socket!");
        return NULL;
    }
    if (!GenerateHandle(socket, version, type))
    {
        SetLastNetErrorFromOS(WSAGetLastError());
        DeleteSocket(socket);
        return NULL;
    }

    socket->m_HasClose = false;
    socket->m_IsBlocking = true;
    socket->m_SendTimeout = 0;
    socket->m_RecvTimeout = 0;
    socket->m_CachedPort = 0;
    socket->m_CachedAddress = nullptr;
    socket->m_State = 0;
    return socket;
}

static bool BindOrConnectCheck(const Socket* this, const char* ip, const int port)
{
    if (this == NULL)
    {
        SetLastNetError(InvalidSocket, "The socket you passed is NULL!");
        return false;
    }

    if (ip == NULL)
    {
        SetLastNetError(InvalidAddress, "The IP address is NULL!");
        return false;
    }

    if (!CheckPortNumber(port))
    {
        SetLastNetError(InvalidPort, "The port must be in range [0, 65535]");
        return false;
    }

    if (this->m_HasClose || this->m_Fd == INVALID_SOCKET)
    {
        SetLastNetError(InvalidSocket, "The socket has closed!");
        return false;
    }
    return true;
}

static bool CheckIsAddressMatch(const Socket* this, const IPAddress* ipAddr)
{
    if (ipAddr == NULL)
    {
        SetLastNetError(InvalidAddress, "The IP address is illegal!");
        return false;
    }

    if (this->m_IpVersion != ipAddr->m_Version)
    {
        SetLastNetError(InvalidAddress, "Unmatched IP version!");
        return false;
    }
    return true;
}

bool BindSocket(Socket* this, const char* ip, const int port)
{
    if (!BindOrConnectCheck(this, ip, port))
        return false;

    IPAddress* ipAddr = ParseIPAddressFrom(ip);
    if (!CheckIsAddressMatch(this, ipAddr))
        return false;

    if (this->m_IpVersion == IPv4)
    {
        struct sockaddr_in addr = { };
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        const int res = inet_pton(AF_INET, ip, &addr.sin_addr);
        if (res == 0)
        {
            SetLastNetError(InvalidAddress, "Invalid IPv4 address!");
            return false;
        }

        if (bind(this->m_Fd, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
        {
            SetLastNetErrorFromOS(WSAGetLastError());
            return false;
        }
        this->m_CachedAddress = ipAddr;
    }
    else if (this->m_IpVersion == IPv6)
    {
        //TODO
    }
    else
    {
        SetLastNetError(InvalidState, "The socket has an impossible value in member m_IpVersion!");
        return false;
    }
    this->m_CachedPort = port;
    return true;
}

bool ConnectSocket(Socket* this, const char* ip, const int port)
{
    if (!BindOrConnectCheck(this, ip, port))
        return false;

    const IPAddress* ipAddr = ParseIPAddressFrom(ip);
    if (!CheckIsAddressMatch(this, ipAddr))
        return false;

    if (this->m_IpVersion == IPv4)
    {
        struct sockaddr_in addr = { };
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        const int res = inet_pton(AF_INET, ip, &addr.sin_addr);
        if (res == 0)
        {
            SetLastNetError(InvalidAddress, "Invalid IPv4 address!");
            return false;
        }

        if (connect(this->m_Fd, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
        {
            SetLastNetErrorFromOS(WSAGetLastError());
            return false;
        }
    }
    else if (this->m_IpVersion == IPv6)
    {
        //TODO
    }
    else
    {
        SetLastNetError(InvalidState, "The socket has an impossible value in member m_IpVersion!");
        return false;
    }

    [[maybe_unused]] auto _1 = GetSocketIpAddress(this);
    [[maybe_unused]] auto _2 = GetSocketPort(this);

    this->m_State |= SS_Connected;
    return true;
}

bool ListenSocket(Socket* this, const int backlog)
{
    if (this == nullptr || this->m_Fd == INVALID_SOCKET)
    {
        SetLastNetError(InvalidSocket, "The socket you passed is invalid! At calling [ListenSocket].");
        return false;
    }

    if (this->m_TransportProtocol != TCP)
    {
        SetLastNetError(InvalidSocket, "Listen is only supported on TCP sockets.");
        return false;
    }

    if (listen(this->m_Fd, backlog) != 0)
    {
        const int osError = WSAGetLastError();
        if (osError == WSAEINVAL || osError == EINVAL)
        {
            SetLastNetError(InvalidState, "Socket must be bound before calling ListenSocket.");
        }
        else
        {
            SetLastNetErrorFromOS(osError);
        }
        return false;
    }
    this->m_State |= SS_Listening;
    return true;
}

Socket* AcceptSocket(const Socket* this)
{
    if (this == nullptr || this->m_Fd == INVALID_SOCKET)
    {
        SetLastNetError(InvalidSocket, "Listener socket is null or invalid.");
        return nullptr;
    }

    if (this->m_TransportProtocol != TCP)
    {
        SetLastNetError(InvalidSocket, "Accept is only valid on TCP listening sockets.");
        return nullptr;
    }

    struct sockaddr_storage clientAddr;
    socklen_t addrLen = sizeof(clientAddr);

    const SOCKET responseFD = accept(this->m_Fd, (struct sockaddr*)&clientAddr, &addrLen);
    if (responseFD == INVALID_SOCKET)
    {
        SetLastNetErrorFromOS(WSAGetLastError());
        return nullptr;
    }

    Socket* acceptedSocket = malloc(sizeof(Socket));
    if (acceptedSocket == nullptr)
    {
        closesocket(responseFD);
        SetLastNetError(NetOutOfMemory, "Failed to allocate accepted socket.");
        return nullptr;
    }

    acceptedSocket->m_Fd = responseFD;
    acceptedSocket->m_TransportProtocol = TCP;

    if (clientAddr.ss_family == AF_INET)
        acceptedSocket->m_IpVersion = IPv4;
    else if (clientAddr.ss_family == AF_INET6)
        acceptedSocket->m_IpVersion = IPv6;
    else
    {
        //TODO: Maybe we need to handle this case? I'm not sure...
        SetLastNetError(InvalidState, "Unknown address family from getsockname!");
    }

    acceptedSocket->m_HasClose = false;
    acceptedSocket->m_IsBlocking = true;
    acceptedSocket->m_SendTimeout = 0;
    acceptedSocket->m_RecvTimeout = 0;
    acceptedSocket->m_CachedAddress = nullptr;
    acceptedSocket->m_CachedPort = 0;
    acceptedSocket->m_State = SS_Ready;
    GetSocketIpAddress(acceptedSocket);
    GetSocketPort(acceptedSocket);
    return acceptedSocket;
}

bool SendTo(const Socket* this, const char* data, const int size, const char* ip, const int port)
{
    if (this == NULL || this->m_Fd == INVALID_SOCKET)
    {
        SetLastNetError(InvalidSocket, "The socket you passed is invalid! At calling [SendTo].");
        return false;
    }

    if (data == NULL || size <= 0)
    {
        SetLastNetError(InvalidData, "The data you passed is invalid! At calling [SendTo].");
        return false;
    }

    if (ip == NULL || !CheckPortNumber(port))
    {
        SetLastNetError(InvalidAddress, "The IP address or port id is invalid!");
        return false;
    }

    if (this->m_TransportProtocol != UDP)
    {
        SetLastNetError(InvalidState, "Function [SendTo] requires a UDP socket!");
        return false;
    }

    struct sockaddr_in destAddr = { 0 };
    destAddr.sin_family = AF_INET;
    destAddr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip, &destAddr.sin_addr) <= 0)
    {
        char buf[64] = { };
        sprintf(buf, "Invalid IPv4 address: %s", ip);
        SetLastNetError(InvalidAddress, buf);
        return false;
    }

    const ssize_t sent = sendto(this->m_Fd, data, size, 0,
                                (struct sockaddr*)&destAddr, sizeof(destAddr));

    if (sent == -1)
    {
        SetLastNetErrorFromOS(WSAGetLastError());
        return false;
    }

    return true;
}

bool ReceiveFrom(const Socket* this, char* buffer, const int maxSize, int* recvSize, char* outIP, int* outPort)
{
    if (recvSize != NULL)
        *recvSize = 0;
    if (this == NULL || this->m_Fd == INVALID_SOCKET)
    {
        SetLastNetError(InvalidSocket, "The socket you passed is invalid! At calling [ReceiveFrom].");
        return false;
    }

    if (buffer == NULL || maxSize <= 0)
    {
        SetLastNetError(InvalidData, "The buffer you passed is invalid! At calling [ReceiveFrom].");
        return false;
    }

    struct sockaddr_in addrFrom = { };
    int addrLen = sizeof(addrFrom);

    const int bytes = recvfrom(this->m_Fd, buffer, maxSize - 1, 0, (struct sockaddr*)&addrFrom, &addrLen);

    if (bytes == SOCKET_ERROR)
    {
        SetLastNetErrorFromOS(WSAGetLastError());
        return false;
    }

    if (recvSize != NULL)
        *recvSize = bytes;
    buffer[bytes] = '\0';

    if (outPort != NULL)
        *outPort = ntohs(addrFrom.sin_port);

    if (outIP != NULL)
    {
        const char* ipStr = inet_ntop(AF_INET, &addrFrom.sin_addr, outIP, INET_ADDRSTRLEN);
        if (ipStr == NULL)
        {
            SetLastNetError(NetOutOfMemory, "The buffer is too small to store the extracted ip address!");
            outIP[0] = '\0';
        }
    }

    return true;
}

bool SendSocket(const Socket* this, const char* data, const int size)
{
    if (this == nullptr || this->m_Fd == INVALID_SOCKET)
    {
        SetLastNetError(InvalidSocket, "Socket is null or invalid.");
        return false;
    }

    if (size < 0)
    {
        SetLastNetError(InvalidSocket, "Size must be >= 0.");
        return false;
    }

    if (size == 0)
        return true;

    if (data == nullptr)
    {
        SetLastNetError(InvalidData, "Data is null but size > 0.");
        return false;
    }

    if (this->m_TransportProtocol != TCP)
    {
        SetLastNetError(InvalidState, "Send is only valid on TCP sockets.");
        return false;
    }

    const int sent = send(this->m_Fd, data, size, 0);
    if (sent == SOCKET_ERROR)
    {
        SetLastNetErrorFromOS(WSAGetLastError());
        return false;
    }

    if (sent != size)
    {
        SetLastNetError(InvalidState, "Failed to send all data in one call.");
        return false;
    }

    return true;
}

bool ReceiveSocket(const Socket* this, char* buffer, const int maxSize, int* recvSize)
{
    if (recvSize)
        *recvSize = 0;

    if (this == nullptr || this->m_Fd == INVALID_SOCKET)
    {
        SetLastNetError(InvalidSocket, "Socket is null or invalid.");
        return false;
    }

    if (maxSize <= 0)
    {
        SetLastNetError(InvalidData, "maxSize must be > 0.");
        return false;
    }

    if (buffer == nullptr)
    {
        SetLastNetError(InvalidData, "Buffer is null.");
        return false;
    }

    if (this->m_TransportProtocol != TCP)
    {
        SetLastNetError(InvalidState, "Receive is only valid on TCP sockets.");
        return false;
    }

    const int received = recv(this->m_Fd, buffer, maxSize, 0);
    if (received == SOCKET_ERROR)
    {
        SetLastNetErrorFromOS(WSAGetLastError());
        return false;
    }

    if (recvSize)
        *recvSize = received;

    return true;
}

void SetSocketIsBlock(const Socket* this, const bool isBlock)
{
    u_long mode = isBlock ? 0 : 1;
    ioctlsocket(this->m_Fd, FIONBIO, &mode);
}

bool IsSocketUDP(const Socket* this)
{
    return this->m_TransportProtocol == UDP;
}

TransportProtocol GetSocketProtocol(const Socket* this)
{
    return this->m_TransportProtocol;
}

IPAddress* GetSocketIpAddress(Socket* this)
{
    if (this == nullptr || this->m_Fd == INVALID_SOCKET)
        return nullptr;
    if (this->m_CachedAddress != nullptr)
        return this->m_CachedAddress;

    struct sockaddr_storage addr = { };
    socklen_t len = sizeof(addr);

    if (getsockname(this->m_Fd, (struct sockaddr*)&addr, &len) != 0)
    {
        SetLastNetErrorFromOS(WSAGetLastError());
        return nullptr;
    }

    char ipStr[INET6_ADDRSTRLEN];
    const char* ipCStr;

    if (addr.ss_family == AF_INET)
    {
        const auto sin = (const struct sockaddr_in*)&addr;
        ipCStr = inet_ntop(AF_INET, &sin->sin_addr, ipStr, sizeof(ipStr));
    }
    else if (addr.ss_family == AF_INET6)
    {
        const auto sin6 = (const struct sockaddr_in6*)&addr;
        ipCStr = inet_ntop(AF_INET6, &sin6->sin6_addr, ipStr, sizeof(ipStr));
    }
    else
    {
        SetLastNetError(InvalidState, "Unknown address family from getsockname!");
        return nullptr;
    }

    if (ipCStr == nullptr)
    {
        SetLastNetErrorFromOS(WSAGetLastError());
        return nullptr;
    }

    IPAddress* ipAddr = ParseIPAddressFrom(ipCStr);
    if (ipAddr == nullptr)
    {
        SetLastNetError(InvalidAddress, "Failed to parse IP from getsockname result!");
        return nullptr;
    }

    this->m_CachedAddress = ipAddr;
    return this->m_CachedAddress;
}

int GetSocketPort(Socket* this)
{
    if (this == nullptr || this->m_Fd == INVALID_SOCKET)
        return -1;

    if (this->m_CachedPort != 0 && this->m_CachedPort != -1)
        return this->m_CachedPort;

    struct sockaddr_storage addr;
    socklen_t len = sizeof(addr);

    if (getsockname(this->m_Fd, (struct sockaddr*)&addr, &len) != 0)
    {
        SetLastNetErrorFromOS(WSAGetLastError());
        return -1;
    }

    int port = -1;
    if (addr.ss_family == AF_INET)
    {
        const auto sin = (const struct sockaddr_in*)&addr;
        port = ntohs(sin->sin_port);
    }
    else if (addr.ss_family == AF_INET6)
    {
        const auto sin6 = (const struct sockaddr_in6*)&addr;
        port = ntohs(sin6->sin6_port);
    }
    else
    {
        SetLastNetError(InvalidState, "Unknown address family from getsockname!");
    }
    this->m_CachedPort = port;
    return port;
}

bool GetPeerSocketAddress(const Socket* this, char* outIP, int* outPort)
{
    if (this == nullptr || this->m_Fd == INVALID_SOCKET)
    {
        SetLastNetError(InvalidSocket, "Socket is null or invalid.");
        return false;
    }

    if (this->m_TransportProtocol != TCP)
    {
        SetLastNetError(InvalidSocket, "Peer address is only available for TCP sockets.");
        return false;
    }

    struct sockaddr_storage addr;
    socklen_t addrLen = sizeof(addr);

    if (getpeername(this->m_Fd, (struct sockaddr*)&addr, &addrLen) != 0)
    {
        const int osError = GetLastNetErrorCode();

        if (osError == WSAENOTCONN || osError == ENOTCONN)
            SetLastNetError(InvalidState, "Socket is not connected.");
        else
            SetLastNetErrorFromOS(osError);

        return false;
    }

    if (outIP != nullptr || outPort != nullptr)
    {
        if (addr.ss_family == AF_INET)
        {
            const auto s4 = (struct sockaddr_in*)&addr;
            if (outIP)
                inet_ntop(AF_INET, &s4->sin_addr, outIP, INET_ADDRSTRLEN);
            if (outPort)
                *outPort = ntohs(s4->sin_port);
        }
        else if (addr.ss_family == AF_INET6)
        {
            const auto s6 = (struct sockaddr_in6*)&addr;
            if (outIP)
                inet_ntop(AF_INET6, &s6->sin6_addr, outIP, INET6_ADDRSTRLEN);

            if (outPort)
                *outPort = ntohs(s6->sin6_port);
        }
        else
        {
            SetLastNetError(InvalidState, "Unknown address family.");
            return false;
        }
    }

    return true;
}

void DeleteSocket(Socket* socket)
{
    if (socket == NULL)
        return;

    if (!socket->m_HasClose && socket->m_Fd != INVALID_SOCKET)
    {
        closesocket(socket->m_Fd);
        socket->m_Fd = INVALID_SOCKET;
        socket->m_HasClose = true;
    }

    free((void*)socket->m_CachedAddress->m_OriAddress);
    free(socket->m_CachedAddress);
    free(socket);
}
