#include "ICMPv4_ERROR.h"
#include "ICMPv4config.h"
#include "AbstractionAPI.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/***************************************************************************************************
*
*   FUNCTION NAME: ICMPv4_ERROR_02
*
***************************************************************************************************/
/**
* @brief
*  Also ICMP messages are only sent about errors in handling fragment zero of fragmented datagrams.
*
* @return int 0: The test  was successful | 1: The test failed
*
* @note
*  This tests that ICMP error message is sent on receiving the fragment having Fragment Offset 
*  field set to zero. This test is ran when <DUTSupportsIPOptions> is TRUE.
*
* @warning
* -
*
***************************************************************************************************/
int ICMPv4_ERROR_02()
{   
    /* TESTER: Construct an ICMP Echo Request. Send an IP packet to
        <DIface-0>, containing:
        - Source Address field set to address of host-1
        - Destination Address field set to address of DUT
        - Fragment Offset field set to zero
        - Flags field, containing:
          - MF bit set to 1
        - IP Options, containing:
          - One Internet Timestamp option, containing:
            - length field set to 10
            - pointer field set to 9
            - one timestamp value
        - first half of the constructed ICMP packet */
    ICMP_Option_Packet ICMP_OP_P = CreateICMPOption();
    EditICMPOptionField(&ICMP_OP_P,IP, IP_Offset, (void *)0b0010000000000000);
    ICMP_Option_Compute_checksum(&ICMP_OP_P);
    SendICMPOption(ICMP_OP_P);
    /*TESTER: Listen (for up to <ListenTime> seconds) on <DIface-0>*/
    Packet_filter filter;
    strcpy(filter.dstAdd, (char*)ICMPv4Config.TESTER_IP);
    strcpy(filter.SrcAdd, (char*)ICMPv4Config.DUT_IP);
    ICMP_Packet ICMP_PR = ListenICMP(filter, ICMPv4Config.LISTEN_TIME);
    /* DUT : Send one ICMP Parameter Problem message */
    if (ICMP_PR.length != 0)
    {   
        if(GetICMPField(&ICMP_PR, ICMP, ICMP_pointer) == 22 )
        {
            printf("------------- int ICMPv4_ERROR_02: Test passed -------------\n");
            return 0;
        }
        else
        {
            printf("------------- int ICMPv4_ERROR_02: Test failed -------------\n");
            return 1;
        }
    }
    else
    {
        printf("------------- int ICMPv4_ERROR_02: Test failed -------------\n");
        return 1;
    }

    return 2;
}

/***************************************************************************************************
*
*   FUNCTION NAME: ICMPv4_ERROR_03
*
***************************************************************************************************/
/**
* @brief
*  Also ICMP messages are only sent about errors in handling fragment zero of fragmented datagrams.
*
* @return int 0: The test  was successful | 1: The test failed
*
* @note
*  This tests that ICMP error message is not sent on receiving non zero fragment i.e Fragment Offset
*  field set to non-zero value. This test is ran when <DUTSupportsIPOptions> is TRUE.
*
* @warning
* -
*
***************************************************************************************************/
int ICMPv4_ERROR_03()
{
    /* TESTER: Construct an ICMP Echo Request. Send an IP packet to
        <DIface-0>, containing:
        - Source Address field set to address of host-1
        - Destination Address field set to address of DUT
        - Fragment Offset field set to zero
        - Flags field, containing:
          - MF bit set to 1
        - IP Options, containing:
          - One Internet Timestamp option, containing:
            - length field set to 12
            - pointer field set to 9
            - one timestamp value
        - first half of the constructed ICMP packet */
    ICMP_Option_Packet ICMP_OP_P = CreateICMPOption();
    EditICMPOptionField(&ICMP_OP_P,IP, IP_Offset, (void *)0b0010000000000000);
    EditICMPOptionField(&ICMP_OP_P,IP_Options, ipt_len, (void *)12);
    ICMP_Option_Compute_checksum(&ICMP_OP_P);
    EditICMPOptionField(&ICMP_OP_P,ICMP, ICMP_checksum, (void *)0xfcce);
    SendICMPOption(ICMP_OP_P);
    /* TESTER: Send an IP packet to <DIface-0>, containing:
        - Source Address field set to address of host-1
        - Destination Address field set to address of DUT
        - Fragment Offset field set to data size sent in first 
          IP packet in unit of 8-octets
        - Flags first, containing:
          - MF bit set to zero
        - IP Options, containing:
          - One Internet Timestamp option, containing:
            - length field set to 10
            - pointer field set to 9
            - one timestamp value
        - last half of the constructed ICMP packet */
    IP_OPTION_Packet IP_OP_P = CreateIPoption();
    SendIPOption(IP_OP_P);
    /*TESTER: Listen (for up to <ListenTime> seconds) on <DIface-0>*/
    Packet_filter filter;
    strcpy(filter.dstAdd, (char*)ICMPv4Config.TESTER_IP);
    strcpy(filter.SrcAdd, (char*)ICMPv4Config.DUT_IP);
    ICMP_Packet ICMP_PR = ListenICMP(filter, ICMPv4Config.LISTEN_TIME);
    /* DUT: Do not send ICMP Parameter Problem messages */
    if (ICMP_PR.length != 0)
    {   
        if(GetICMPField(&ICMP_PR, ICMP, ICMP_type) == 12 )
        {
            printf("------------- int ICMPv4_ERROR_03: Test fialed -------------\n");
            return 1;
        }
        else
        {
            printf("------------- int ICMPv4_ERROR_03: Test passed -------------\n");
            return 0;
        }
    }
    else
    {
        printf("------------- int ICMPv4_ERROR_03: Test passed -------------\n");
        return 0;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: ICMPv4_ERROR_04
*
***************************************************************************************************/
/**
* @brief
*  An ICMP error message MUST NOT be sent as the result of receiving a datagram destined to an IP
*  broadcast.
*
* @return int 0: The test  was successful | 1: The test failed
*
* @note
*  This test is ran when <DUTSupportsIPOptions> is TRUE.
*
* @warning
* -
*
***************************************************************************************************/
int ICMPv4_ERROR_04()
{   
    /*  TESTER: Send an ICMP Echo Request to <DIface-0>, containing:
        - IP Source Address field set to address of host-1
        - IP Destination Address field set to <broadcast-address>
        - IP Options, containing:
          - One Internet Timestamp option, containing:
            - length field set to 10
            - pointer field set to 9
            - one timestamp value */
    ICMP_Option_Packet ICMP_OP_P = CreateICMPOption();
    EditICMPOptionField(&ICMP_OP_P,IP,IP_DstAddress,(void*)ICMPv4Config.BROADCAST_ADDRESS);
    SendICMPOption(ICMP_OP_P);
    /* TESTER: Listen (for up to <ListenTime> seconds) on <DIface-0> */
    Packet_filter filter;
    strcpy(filter.dstAdd, (char*)ICMPv4Config.TESTER_IP);
    strcpy(filter.SrcAdd, (char*)ICMPv4Config.DUT_IP);
    ICMP_Packet ICMP_PR = ListenICMP(filter, ICMPv4Config.LISTEN_TIME);
    /* DUT: Do not send ICMP Parameter Problem Message */
    if (ICMP_PR.length == 0)
    {
        printf("------------- ICMPv4_ERROR_04: Test passed -------------\n");
        return 0;
    }
    else
    {   
        if (GetICMPField(&ICMP_PR, ICMP, ICMP_type) == 12)
        {
            printf("------------- ICMPv4_ERROR_04: Test failed ------------\n");
            return 1;
        }
        else
        {
            printf("------------- ICMPv4_ERROR_04: Test passed -------------\n");
            return 0;
        }
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: ICMPv4_ERROR_05
*
***************************************************************************************************/
/**
* @brief
*  If an ICMP message of unknown type is received, it MUST be silently discarded.
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
int ICMPv4_ERROR_05()
{
    // 1. TESTER: Send an ICMP Message containing an invalid ICMP type
    ICMP_Packet ICMP_P = CreateICMP();
    EditICMPField(&ICMP_P, ICMP, ICMP_type, (void *)(intptr_t)ICMPv4Config.INVALID_ICMP_TYPE);
    ICMP_Compute_checksum(&ICMP_P);
    SendICMP(ICMP_P);

    // 2. TESTER: Listen for response
    Packet_filter filter;
    strcpy(filter.dstAdd, (char*)ICMPv4Config.TESTER_IP);
    strcpy(filter.SrcAdd, (char*)ICMPv4Config.DUT_IP);
    ICMP_Packet ICMP_PR = ListenICMP(filter, ICMPv4Config.LISTEN_TIME);

    // 3. DUT: Do not send any ICMP Message
    if (ICMP_PR.length == 0)
    {
        printf("------------- ICMPv4_ERROR_05: Test passed -------------\n");
        return 0;
    }
    else
    {
        printf("------------- ICMPv4_ERROR_05: Test failed -> DUT sent a response -------------\n");
        return 1;
    }
}
