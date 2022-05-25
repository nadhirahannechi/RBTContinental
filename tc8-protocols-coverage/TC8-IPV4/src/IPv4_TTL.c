#include "AbstractionAPI.h"
#include "IPv4_TTL.h"
#include "IPv4config.h"
#include <stdio.h>
#include <string.h>

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_TTL_01
*
***************************************************************************************************/
/**
* @brief
*  A host MUST NOT send a datagram with a Time-to-Live (TTL) value of zero.
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
int IPv4_TTL_01()
{
    // 1. TESTER: Send an ICMPv4 Echo Request packet
    ICMP_Packet ICMP_P = CreateICMP();
    ICMP_Compute_checksum(&ICMP_P);
    SendICMP(ICMP_P);
    
    // 2. TESTER: Listen for a response
	Packet_filter filter;
    strcpy(filter.dstAdd, (char*)IPv4Config.TESTER_IP);
    strcpy(filter.SrcAdd, (char*)IPv4Config.DUT_IP);
    ICMP_Packet ICMP_PR = ListenICMP(filter, IPv4Config.LISTEN_TIME);


    // 3. DUT: Sends an ICMPv4 Echo Reply packet with a TTL value greater than 0
    if(ICMP_PR.length == 0)
    {
        printf("------------- IPv4_TTL_01: Test failed -> DUT not responding -------------\n");
        return 1;
    }
    else if((int)GetICMPField(&ICMP_PR, IP, IP_Ttl) > 0)
    {
        printf("------------- IPv4_TTL_01: Test passed -------------\n");
        return 0;
    }
    else
    {
        printf("------------- IPv4_TTL_01: Test failed -------------\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_TTL_05
*
***************************************************************************************************/
/**
* @brief
*  A host MUST NOT discard a datagram just because it was received with TTL less than 2.
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
int IPv4_TTL_05()
{
    // 1. TESTER: Send an ICMP Echo Request with TTL less than 2.
    ICMP_Packet ICMP_P = CreateICMP();
    EditICMPField(&ICMP_P, IP, IP_Ttl, (void *)(intptr_t)IPv4Config.TTL);
    ICMP_Compute_checksum(&ICMP_P);
    SendICMP(ICMP_P);
    
    // 2. TESTER: Listen for a response
	Packet_filter filter;
    strcpy(filter.dstAdd, (char*)IPv4Config.TESTER_IP);
    strcpy(filter.SrcAdd, (char*)IPv4Config.DUT_IP);
    ICMP_Packet ICMP_PR = ListenICMP(filter, IPv4Config.LISTEN_TIME);


    // 3. DUT: Send ICMP Echo Reply
    if(ICMP_PR.length == 0)
    {
        printf("------------- IPv4_TTL_05: Test failed -> DUT not responding -------------\n");
        return 1;
    }
    else if((int)GetICMPField(&ICMP_PR ,ICMP, ICMP_type) == 0 && (int)GetICMPField(&ICMP_PR ,ICMP, ICMP_code) == 0)
    {
        printf("------------- IPv4_TTL_05: Test passed -------------\n");
        return 0;
    }
    else
    {
        printf("------------- IPv4_TTL_05: Test failed -------------\n");
        return 1;
    }
}
