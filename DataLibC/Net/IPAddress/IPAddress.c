//
// Created by 18030 on 2025/9/17.
//

#include "IPAddress.h"

#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "IPAddress_Internal.h"
#include "DataLibC/DS/Allocation.h"
#include "DataLibC/DS/String/String.h"
#include "DataLibC/Tool/Magic.h"

const char* const LOCAL_ADDRESS = "127.0.0.1";

static bool VerifySegment(const char* seg, unsigned* outVal)
{
    if (seg == NULL || *seg == '\0')
        return false;

    char ch;
    int length = 0;
    foreach(ch in seg)
    {
        if (!isdigit(ch))
            return false;
        length++;
    }

    if (length > 3)
        return false;

    // this means the segment looks like 017
    if (seg[0] == '0' && seg[1] != '\0')
        return false;

    const long val = strtol(seg, NULL, 10);
    *outVal = val;
    return val >= 0 && val <= 255;
}

static bool ParseIPv4(char* ipSeg[], IPAddress* out)
{
    assert(ipSeg);
    out->m_Version = IPv4;
    out->m_Address = 0;
    unsigned segVal;
    for (int i = 0; i < 4; i++)
    {
        if (!VerifySegment(ipSeg[i], &segVal))
            return false;
        out->m_Address = out->m_Address | segVal << (8 * (3 - i));
    }
    return true;
}

static bool ParseIPv6(const char* ip, IPAddress* out) { }

IPAddress* ParseIPAddressFrom(const char* ip)
{
    if (ip == NULL)
        return NULL;
    String ipStr;
    String_Init(&ipStr, 0, ip);
    const bool isIPv6 = String_Contains(&ipStr, ":");

    IPAddress* ipAddr = BASE_NEW(IPAddress);
    if (ipAddr == NULL)
        return NULL;
    bool success = false;
    if (isIPv6)
    {
        success = ParseIPv6(ip, ipAddr);
    }
    else
    {
        char ipSeg[4][4] = { '\0' };
        char* out[] = { ipSeg[0], ipSeg[1], ipSeg[2], ipSeg[3] };
        const int size = String_SplitByChar(&ipStr, '.', out, 4);

        success = size == 4 && ParseIPv4(out, ipAddr);
    }
    ipAddr->m_OriAddress = strdup(ip);
    String_Release(&ipStr);
    if (!success)
    {
        BASE_DELETE(IPAddress, ipAddr);
        return NULL;
    }

    //TODO


    return ipAddr;
}
