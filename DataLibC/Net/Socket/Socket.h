//
// Created by 18030 on 2025/9/17.
//

#ifndef SOCKET_H
#define SOCKET_H

#include "DataLibC/Tool/ExportModuleMacro.h"
#include "DataLibC/Net/IPAddress/IPAddress.h"

typedef enum IpVersion
{
    IPv4,
    IPv6
} IpVersion;

/**
 * The transport protocol. (TCP or UDP)
 */
typedef enum TransportProtocol
{
    TCP,
    UDP
} TransportProtocol;

typedef enum SocketState
{
    SS_Ready = 1 << 0,
    SS_Listening = 1 << 1,
    SS_Connected = 1 << 2,
    SS_Closed = 1 << 3
} SocketState;

/// The core tool for inter process communication in networks.
typedef LIB_STRUCT struct Socket Socket;

/**
 * Create a socket with given address family and socket type.
 * @param version The address family (IPv4 or IPv6).
 * @param type The socket type (TCP or UDP).
 * @return The socket object. If failed, NULL is returned.
 */
API_MODULE Socket* CreateSocket(IpVersion version, TransportProtocol type);

/**
 * @brief Bind a socket to the given ip and port
 * @param this The socket object
 * @param ip Target ip address
 * @param port Target port id
 * @return True if bind succeeds, false otherwise.
 */
API_MODULE bool BindSocket(Socket* this, const char* ip, int port);

/**
 * @brief Connect a socket to the given ip and port when using TCP protocol.
 * @param this The socket object
 * @param ip Target server ip address
 * @param port Target server port id
 * @return True if connect succeeds, false otherwise.
 */
API_MODULE bool ConnectSocket(Socket* this, const char* ip, int port);

/**
 * @brief Listen a socket for incoming connections when using TCP protocol.
 * @param this The socket object
 * @param backlog The maximum length of the queue for pending connections.
 * @return True if listen succeeds, false otherwise.
 */
API_MODULE bool ListenSocket(Socket* this, int backlog);

/**
 * @brief Accept a connection from a client when using TCP protocol.
 * @param this The socket object
 * @return The socket object of the accepted connection. If failed, nullptr is returned.
 * @attention This method is always used in a server and the sockets it returns should be managed manually. If you don't
 * want to manage these objects, please use TcpServer to controll tcp service instead.
 */
API_MODULE Socket* AcceptSocket(const Socket* this);

/**
 * @brief Send data to the given ip and port through UDP protocol.
 * @param this The socket object
 * @param data The data to send
 * @param size The size of data (Exclude the null terminator).
 * @param ip Target ip address
 * @param port Target port id
 * @return True if send succeeds, false otherwise.
 */
API_MODULE bool SendTo(const Socket* this, const char* data, int size, const char* ip, int port);

/**
 * @brief Receive data from the socket through UDP protocol.
 * @param this The socket object
 * @param buffer The buffer to store the received data
 * @param maxSize The maximum size of buffer
 * @param recvSize The size of data actually received (Pass NULL to ignore the result).
 * @param outIP The ip address of sender
 * @param outPort The port id of sender
 * @return True if receive succeeds, false otherwise.
 */
API_MODULE bool ReceiveFrom(const Socket* this, char* buffer, int maxSize, int* recvSize, char* outIP, int* outPort);

/**
 * @brief Send data to the socket through TCP protocol.
 * @param this The socket object.
 * @param data The data to send.
 * @param size The size of data (!!!NOT!!! Exclude the null terminator '\0').
 * @return True if send succeeds, false otherwise.
 */
API_MODULE bool SendSocket(const Socket* this, const char* data, int size);

/**
 * @brief Receive data from the socket through TCP protocol.
 * @param this The socket object.
 * @param buffer The buffer to store received data (must be at least maxSize bytes).
 * @param maxSize Maximum number of bytes to receive.
 * @param recvSize Optional. If not null, receives the actual number of bytes read.
 * @return True if the operation succeeded (including graceful close),
 *         false if an error occurred.
 * @attention If recvSize is 0, it means the peer has closed the connection.
 */
API_MODULE bool ReceiveSocket(const Socket* this, char* buffer, int maxSize, int* recvSize);

/**
 * @brief Set the socket's blocking mode.
 * @param this The socket object
 * @param isBlock True if the socket is blocking, false otherwise.
 */
API_MODULE void SetSocketIsBlock(const Socket* this, bool isBlock);

/**
 * @brief Check whether the socket's protocol is UDP.
 * @param this The socket object
 * @return True if the socket's protocol is UDP, false otherwise.
 */
[[deprecated("Use GetSocketProtocol(const Socket*) instead.")]]
API_MODULE bool IsSocketUDP(const Socket* this);

/**
 * @brief Get the socket's protocol.
 * @param this The socket object
 * @return The socket's protocol.
 */
API_MODULE TransportProtocol GetSocketProtocol(const Socket* this);

/**
 * @brief Get the socket's ip address.
 * @param this The socket object
 * @return The socket's ip address.
 */
API_MODULE IPAddress* GetSocketIpAddress(Socket* this);

/**
 * @brief Get the socket's port id.
 * @param this The socket object
 * @return The socket's port id. If -1, the socket is not valid. If 0, the socket is not bound.
 */
API_MODULE int GetSocketPort(Socket* this);

/**
 * @brief Get the peer's ip address of the given socket. (For TCP protocol)
 * @param this The socket object
 * @param outIP The buffer to store the ip address
 * @param outPort The buffer to store the port id
 * @return True if succeeds, false otherwise.
 */
API_MODULE bool GetPeerSocketAddress(const Socket* this, char* outIP, int* outPort);

/**
 * Delete a socket and cleanup its relative resources.
 * @param socket The socket object (NULL is ignored).
 */
API_MODULE void DeleteSocket(Socket* socket);

#endif //SOCKET_H
