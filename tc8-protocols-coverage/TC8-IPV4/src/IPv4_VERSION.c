#include "AbstractionAPI.h"
#include "IPv4_VERSION.h"
#include "IPv4config.h"
#include <stdio.h>
#include <string.h>

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_VERSION_01
*
***************************************************************************************************/
/**
* @brief
*  Ensure that when the DUT receives an IPv4 packet containing an IPv4 Header containing a Version
*  indicating a value of 4, then the DUT accepts the IPv4 Packet.
*
* @return int 0: The test  was successful | 1: The test failed
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int IPv4_VERSION_01()
{
    //1. TESTER: Send an ICMPv4 Echo Request with version 4
    ICMP_Packet ICMP_P = CreateICMP();
    EditICMPField(&ICMP_P, IP, IP_Version, (void *)4);
    ICMP_Compute_checksum(&ICMP_P);
    SendICMP(ICMP_P);

    // 2. TESTER: Listen for a response
    Packet_filter filter;
    strcpy(filter.dstAdd, (char*)IPv4Config.TESTER_IP);
    strcpy(filter.SrcAdd, (char*)IPv4Config.DUT_IP);
    ICMP_Packet ICMP_PR = ListenICMP(filter, IPv4Config.LISTEN_TIME);

    // 3. DUT: Sends an ICMPv4 Echo Reply
    if (ICMP_PR.length == 0)
    {
        printf("------------- IPv4_VERSION_01: Test failed -> DUT not responding -------------\n");
        return 1;
    }
    else if ((int)GetICMPField(&ICMP_PR, ICMP, ICMP_type) == 0 && (int)GetICMPField(&ICMP_PR, ICMP, ICMP_code) == 0)
    {
        printf("------------- IPv4_VERSION_01: Test passed -------------\n");
        return 0;
    }
    else
    {
        printf("------------- IPv4_VERSION_01: Test failed -------------\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_VERSION_03
*
***************************************************************************************************/
/**
* @brief
*  Ensure that when the DUT generates an IPv4 packet then the DUT sends an IPv4 Packet containing an
*  IPv4 Header containing a Version indicating a value of 4.
*
* @return int 0: The test  was successful | 1: The test failed 
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int IPv4_VERSION_03()
{
    //1. TESTER: Send an ICMPv4 Echo Request with version 4
    ICMP_Packet ICMP_P = CreateICMP();
    EditICMPField(&ICMP_P, IP, IP_Version, (void *)4);
    ICMP_Compute_checksum(&ICMP_P);
    SendICMP(ICMP_P);

    // 2. TESTER: Listen for a response
    Packet_filter filter;
    strcpy(filter.dstAdd, (char*)IPv4Config.TESTER_IP);
    strcpy(filter.SrcAdd, (char*)IPv4Config.DUT_IP);
    ICMP_Packet ICMP_PR = ListenICMP(filter, IPv4Config.LISTEN_TIME);

    // 3. DUT: Sends an ICMPv4 Echo Reply containing a valid version 4
    if (ICMP_PR.length == 0)
    {
        printf("------------- IPv4_VERSION_03: Test failed -> DUT not responding -------------\n");
        return 1;
    }
    else if ((int)GetICMPField(&ICMP_PR, ICMP, ICMP_type) == 0 && (int)GetICMPField(&ICMP_PR, ICMP, ICMP_code) == 0 && (int)GetICMPField(&ICMP_PR, IP, IP_Version == 4))
    {
        printf("------------- IPv4_VERSION_03: Test passed -------------\n");
        return 0;
    }
    else
    {
        printf("------------- IPv4_VERSION_03: Test failed -------------\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_VERSION_04
*
***************************************************************************************************/
/**
* @brief
*  A datagram whose version number is not 4 MUST be silently discarded.
*
* @return int 0: The test  was successful | 1: The test failed 
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int IPv4_VERSION_04() /* Doesnt send any packet*/
{
    // 1. TESTER: Send an ICMP Echo Request
    ICMP_Packet ICMP_P = CreateICMP();
    EditICMPField(&ICMP_P, IP, IP_Version, (void *)(intptr_t)IPv4Config.IP_VERSION);
    ICMP_Compute_checksum(&ICMP_P);
    SendICMP(ICMP_P);

    // 2. TESTER: Listen for a response
    Packet_filter filter;
    strcpy(filter.dstAdd, (char*)IPv4Config.TESTER_IP);
    strcpy(filter.SrcAdd, (char*)IPv4Config.DUT_IP);
    ICMP_Packet ICMP_PR = ListenICMP(filter, IPv4Config.LISTEN_TIME);

    // 3. DUT: Discard ICMP Echo Request and do not send ICMP Echo Reply
    switch (IPv4Config.IP_VERSION)
    {
    case IP_VERSION_4:
    {
        printf("------------- IPv4_VERSION_04: Test failed -> You should set an IP version different from 4 -------------\n");
        return 1;
        break;
    }

    default:
    {
        if (ICMP_PR.length == 0)
        {
            printf("------------- IPv4_VERSION_04: Test passed -------------\n");
            return 0;
        }
        else
        {
            printf("------------- IPv4_VERSION_04: Test failed -------------\n");
            return 1;
        }
        break;
    }
    break;
    }
}