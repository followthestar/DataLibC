//
// Created by 18030 on 2025/9/18.
//

#ifndef IPADDRESS_INTERNAL_H
#define IPADDRESS_INTERNAL_H

#include "DataLibC/Net/Socket/Socket.h"
#include "DataLibC/Tool/ExportModuleMacro.h"
#include "DataLibC/Tool/TypeMacro.h"

typedef LIB_STRUCT struct IPAddress
{
    union
    {
        /// IPv4
        UInt32 m_Address;

        /// IPv6
        UInt32 m_Scope;
    };

    const char* m_OriAddress;
    UInt16 m_Segments[8];
    IpVersion m_Version;
} IPAddress;

#endif //IPADDRESS_INTERNAL_H
