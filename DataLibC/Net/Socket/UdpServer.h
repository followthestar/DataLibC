//
// Created by 18030 on 2025/9/28.
//

#ifndef UDPSERVER_H
#define UDPSERVER_H

#include "DataLibC/Tool/ExportModuleMacro.h"

/// A UdpServer acts as a server to receive and process UDP datagrams.
typedef struct UdpServer UdpServer;

typedef void (* UdpOnReceive)(const char* data, int size, const char* remoteIP, int remotePort, void* userData);

/**
 * Create a UdpServer with given ip address and port id
 * @param ip IP Address
 * @param port Port ID
 * @return A new UdpServer object.
 */
API_MODULE UdpServer* UdpServer_Create(const char* ip, int port);

/**
 * Set the callback function to handle the incoming UDP packets. And the callback will be called when the packets arrive.
 * @param server The UdpServer object
 * @param callback The callback function.
 * @param userData The user data to be passed to the callback function. If you don't need extra data, just pass nullptr.
 */
API_MODULE void UdpServer_SetOnReceiveCallback(UdpServer* server, UdpOnReceive callback, void* userData);

/**
 * Start the UdpServer
 * @param server The UdpServer object
 * @param threadCount The max amount of threads to handle the incoming requests.
 * @return True if start successfully.
 */
API_MODULE bool UdpServer_Start(UdpServer* server, int threadCount);

/**
 * Stop the UdpServer and release all resources.
 * @param server The UdpServer object
 */
API_MODULE void UdpServer_Stop(UdpServer* server);

/**
 * Release the UdpServer object.
 * @param server The UdpServer object
 */
API_MODULE void UdpServer_Destroy(UdpServer* server);

#endif //UDPSERVER_H
