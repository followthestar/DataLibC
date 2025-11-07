//
// Created by 18030 on 2025/9/28.
//

#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "DataLibC/Tool/ExportModuleMacro.h"
#include "Socket.h"

/// A TcpServer acts as a server to receive and process TCP data streams.
typedef struct TcpServer TcpServer;


/**
 * @brief What should server do when a connection is established or closed.
 * @param conn The socket object of the connection, used to response to the client
 * @param connected True if the connection is established, false if the connection is closed.
 * @param userData Extra context used in this callback
 */
typedef void (* TcpOnConnection)(Socket* conn, bool connected, void* userData);

/**
 * @brief What should server do when a message is received.
 * @param conn The socket object of the connection, used to response to the client
 * @param data The message data
 * @param size The size of the message
 * @param userData Extra context used in this callback
 */
typedef void (* TcpOnMessage)(Socket* conn, const char* data, int size, void* userData);

/**
 * @brief What should server do when an error occurs.
 * @param errorMsg The error message
 * @param userData Extra context used in this callback
 */
typedef void (* TcpOnError)(const char* errorMsg, void* userData);

/**
 * Create a TcpServer with given ip address and port id
 * @param ip IP Address
 * @param port Port ID
 * @return A new TcpServer object.
 */
API_MODULE TcpServer* TcpServer_Create(const char* ip, int port);


/**
 * @brief Start the TcpServer
 * @param server The TcpServer object
 * @param threadCount The max amount of threads to handle the incoming requests.
 * @param maxConnectionLimit The max amount of connections allowed.
 * @return
 */
API_MODULE bool TcpServer_Start(TcpServer* server, int threadCount, int maxConnectionLimit);

/**
 * Stop the TcpServer and release all connections.
 * @param server The TcpServer object
 */
API_MODULE void TcpServer_Stop(TcpServer* server);

/**
 * Release the TcpServer object.
 * @param server The TcpServer object
 */
API_MODULE void TcpServer_Destroy(TcpServer* server);

#endif //TCPSERVER_H
