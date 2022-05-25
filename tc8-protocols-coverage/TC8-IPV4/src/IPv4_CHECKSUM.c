#include "AbstractionAPI.h"
#include "IPv4_CHECKSUM.h"
#include "IPv4config.h"
#include <stdio.h>
#include <string.h>

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_CHECKSUM_02
*
***************************************************************************************************/
/**
* @brief
*  If the header checksum fails, the internet datagram is discarded at once by the entity which 
*  detects the error.
*
* @return int 0: The test was successful | 1: The test failed 
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int IPv4_CHECKSUM_02()
{
    // 1. TESTER: Send an ICMPv4 Echo Request with Header Checksum indicating <invalidChecksum>
    ICMP_Packet ICMP_P = CreateICMP();
    ICMP_Compute_checksum(&ICMP_P);
    EditICMPField(&ICMP_P, ICMP, ICMP_checksum, (void *)(intptr_t)IPv4Config.INVALID_CHECKSUM);
    SendICMP(ICMP_P);

    // 2. TESTER: Listen for a response
    Packet_filter filter;
    strcpy(filter.dstAdd, (char*)IPv4Config.TESTER_IP);
    strcpy(filter.SrcAdd, (char*)IPv4Config.DUT_IP);
    ICMP_Packet ICMP_PR = ListenICMP(filter, IPv4Config.LISTEN_TIME);

    // 3. DUT: Does not send an ICMPv4 Echo Reply
    if(ICMP_PR.length == 0)
    {
        printf("------------- IPv4_CHECKSUM_02: Test passed -------------\n");
    }
    else
    {
        printf("------------- IPv4_CHECKSUM_02: Test failed -> DUT sent a response ------------- \n");
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_CHECKSUM_05
*
***************************************************************************************************/
/**
* @brief
*  The checksum field is the 16 bit one's complement of the one's complement sum of all 16 bit words
*  in the header. For purposes of computing the checksum, the value of the checksum field is zero.
*
* @return int 0: The test was successful | 1: The test failed  
*
* @note
*  Here we send an Echo Request with checksum calculated according to rfc. DUT receives this Echo 
*  Request, verifies the Echo Request and then sends Echo Reply. We then verify that DUT uses the 
*  checksum calculation method according to rfc.
*
* @warning
* -
*
***************************************************************************************************/
int IPv4_CHECKSUM_05() 
{
    // 1. TESTER: Send an ICMP Echo Request
    ICMP_Packet ICMP_P = CreateICMP();
    ICMP_Compute_checksum(&ICMP_P);
    SendICMP(ICMP_P);

    // 2. TESTER: wait for a reply
    Packet_filter filter;
    strcpy(filter.dstAdd, (char*)IPv4Config.TESTER_IP);
    strcpy(filter.SrcAdd, (char*)IPv4Config.DUT_IP);
    ICMP_Packet ICMP_PR1 = ListenICMP(filter, IPv4Config.LISTEN_TIME);

    // 3. TESTER: Verify that the received ICMP Echo Reply contains: - IP Checksum field to "16 bit one's complement of the one's complement sum of all 16 bit words in the header"
    ICMP_Packet Check_ICMP_PR = ICMP_PR1;
    EditICMPField(&Check_ICMP_PR, IP, IP_HeaderChecksum, (void *)0x6d00);
    ICMP_Compute_checksum(&Check_ICMP_PR);
    if((int)GetICMPField(&ICMP_PR1, IP, IP_HeaderChecksum) == (int)GetICMPField(&Check_ICMP_PR, IP, IP_HeaderChecksum))
    {
        printf("------------- IPv4_CHECKSUM_05: Test passed -------------\n");
        return 0;
    }
    else
    {
        printf("------------- IPv4_CHECKSUM_05: Test failed -> bad checksum -------------\n");
        return 1;
    }
}
