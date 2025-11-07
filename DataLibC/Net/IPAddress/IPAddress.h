//
// Created by 18030 on 2025/9/17.
//

#ifndef IPADDRESS_H
#define IPADDRESS_H

#include "DataLibC/Tool/ExportModuleMacro.h"

typedef struct IPAddress IPAddress;

API_MODULE extern const char* const LOCAL_ADDRESS;

API_MODULE IPAddress* ParseIPAddressFrom(const char* ip);


#endif //IPADDRESS_H
