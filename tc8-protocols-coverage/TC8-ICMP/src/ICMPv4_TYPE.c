#include "AbstractionAPI.h"
#include "ICMPv4_TYPE.h"
#include "ICMPv4config.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/***************************************************************************************************
*
*   FUNCTION NAME: ICMPv4_TYPE_04
*
***************************************************************************************************/
/**
* @brief
*  If fragment zero is not available then no time exceeded need be sent at all.
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
int ICMPv4_TYPE_04()
{
    // 1. TESTER: Construct an ICMP Echo Request containing Fragment Offset field
    // set to the last half of the constructed ICMP packet in unit of 8-octets
    char lastFragment[] = "ICMPv4_TYPE_04";
    IP_Packet IP_P = CreateIP();
    EditIPField(&IP_P, PAYLAOD, PAYLOAD_data, (void *)lastFragment);
    EditIPField(&IP_P, IP, IP_Offset, (void *)0b00000000000001);
    IP_Compute_checksum(&IP_P);
    SendIP(IP_P);

    // 2. TESTER: Wait for 15s
    sleep(ICMPv4Config.FRAGMENT_REASSEMBLY_TIMEOUT);

    // 3. TESTER: Listen for a response
    Packet_filter filter;
    strcpy(filter.dstAdd, (char*)ICMPv4Config.TESTER_IP);
    strcpy(filter.SrcAdd, (char*)ICMPv4Config.DUT_IP);
    ICMP_Packet ICMP_PR = ListenICMP(filter, ICMPv4Config.LISTEN_TIME);

    // 4. DUT: Do not send ICMP Time Exceeded message
    if (ICMP_PR.length == 0)
    {
        printf("------------- ICMPv4_TYPE_04: Test passed -------------\n");
        return 0;
    }
    else if (GetICMPField(&ICMP_PR, ICMP, ICMP_type) == 11)
    {
        printf("------------- ICMPv4_TYPE_04: Test failed -> DUT sent an ICMP Time Exceeded message -------------\n");
        return 1;
    }
    else
    {
        printf("------------- ICMPv4_TYPE_04: Test failed -------------\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: ICMPv4_TYPE_05
*
***************************************************************************************************/
/**
* @brief
*  If the gateway or host processing a datagram finds a problem with the header parameters such 
*  that it cannot complete processing the datagram it must discard the datagram.
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
int ICMPv4_TYPE_05()
{   
    /* 1. TESTER: Send an ICMP Echo Request to <DIface-0>, containing:
        - IP Source Address field set to address of host-1
        - IP Destination Address field set to address of DUT
        - IP Options, containing:
          - One Internet Timestamp option, containing:
            - length field set to 10
            - pointer field set to 9
            - one timestamp value */
    ICMP_Option_Packet ICMP_OP_P = CreateICMPOption();

    SendICMPOption(ICMP_OP_P);

    /* 2. TESTER: Listen (for upto <ListenTime> seconds) on <DIface-0> */
    Packet_filter filter;
    strcpy(filter.dstAdd, (char*)ICMPv4Config.TESTER_IP);
    strcpy(filter.SrcAdd, (char*)ICMPv4Config.DUT_IP);
    ICMP_Packet ICMP_PR = ListenICMP(filter, ICMPv4Config.LISTEN_TIME);

    // 3. Discard the ICMP Echo Request and do not send ICMP Echo Reply
    if (ICMP_PR.length != 0)
    {
        printf("------------- ICMPv4_TYPE_05: Test failed -> DUT responding -------------\n");
        return 1;
    }
    else
    {
        printf("------------- ICMPv4_TYPE_05: Test passed -------------\n");
        return 0;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: ICMPv4_TYPE_08
*
***************************************************************************************************/
/**
* @brief
*  The data received in the echo message must be returned in the echo reply message.
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
int ICMPv4_TYPE_08()
{
    // 1. TESTER: Construct an ICMP Echo Request
    char payload[] = "ECU NETWORK VALIDATION TEST";
    ICMP_Packet ICMP_P = CreateICMP();
    EditICMPField(&ICMP_P, PAYLAOD, PAYLOAD_data, (void *)payload);
    EditICMPField(&ICMP_P, IP, IP_TotalLength, (void *)55);
    ICMP_Compute_checksum(&ICMP_P);
    SendICMP(ICMP_P);

    // 2. TESTER: Listen for a response
    Packet_filter filter;
    strcpy(filter.dstAdd, (char*)ICMPv4Config.TESTER_IP);
    strcpy(filter.SrcAdd, (char*)ICMPv4Config.DUT_IP);
    ICMP_Packet ICMP_PR = ListenICMP(filter, ICMPv4Config.LISTEN_TIME);

    // 3. DUT: Send an ICMP reply containing the payload
    if (ICMP_PR.length == 0)
    {
        printf("------------- ICMPv4_TYPE_08: Test failed -> DUT not responding -------------\n");
        return 1;
    }
    else if (!strcmp((char *)GetICMPField(&ICMP_PR, PAYLAOD, PAYLOAD_data), payload))
    {
        printf("------------- ICMPv4_TYPE_08: Test passed -------------\n");
        return 0;
    }
    else
    {
        printf("------------- ICMPv4_TYPE_08: Test failed -------------\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: ICMPv4_TYPE_09
*
***************************************************************************************************/
/**
* @brief
*  The identifier and sequence number may be used by the echo sender to aid in matching the replies
*  with the echo requests.
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
int ICMPv4_TYPE_09()
{
    // 1. TESTER: Construct an ICMP Echo Request
    ICMP_Packet ICMP_P = CreateICMP();
    EditICMPField(&ICMP_P, ICMP, ICMP_identifier, (void *)(intptr_t)ICMPv4Config.ICMP_IDENTIFIER);
    EditICMPField(&ICMP_P, ICMP, ICMP_sequence, (void *)(intptr_t)ICMPv4Config.ICMP_SEQUENCE_NUMBER);
    ICMP_Compute_checksum(&ICMP_P);

    // 2. TESTER: Listen for a response
    Packet_filter filter;
    strcpy(filter.dstAdd, (char*)ICMPv4Config.TESTER_IP);
    strcpy(filter.SrcAdd, (char*)ICMPv4Config.DUT_IP);
    SendICMP(ICMP_P);
    ICMP_Packet ICMP_PR = ListenICMP(filter, ICMPv4Config.LISTEN_TIME);

    // 3. TESTER: Verify that the received ICMP Echo Reply contains the same id and sequence as the ICMP request
    if (ICMP_PR.length == 0)
    {
        printf("------------- ICMPv4_TYPE_09: Test failed -> DUT not responding -------------\n");
        return 1;
    }
    else if ((int)GetICMPField(&ICMP_PR, ICMP, ICMP_identifier) == ICMPv4Config.ICMP_IDENTIFIER && (int)GetICMPField(&ICMP_PR, ICMP, ICMP_sequence) == ICMPv4Config.ICMP_SEQUENCE_NUMBER)
    {
        printf("------------- ICMPv4_TYPE_09: Test passed -------------\n");
        return 0;
    }
    else
    {
        printf("------------- ICMPv4_TYPE_09: Test failed -------------\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: ICMPv4_TYPE_10
*
***************************************************************************************************/
/**
* @brief
*  The checksum is the 16-bit one's complement of the one's complement sum of the ICMP message
*  starting with the ICMP Type. For computing the checksum, the checksum field should be zero.
*  If the total length is odd, the received data is padded with one octet of zeros for computing 
*  the checksum.
*
* @return int 0: The test  was successful | 1: The test failed 
*
* @note
*  This tests that a node does not send ICMP Echo Reply if ICMP Checksum is incorrect.
*
* @warning
* -
*
***************************************************************************************************/
int ICMPv4_TYPE_10()
{
    unsigned int correct_ICMP_checksum;
    // 1. TESTER: Construct an ICMP Echo Request containing invalidChecksum
    ICMP_Packet ICMP_P = CreateICMP();
    ICMP_Compute_checksum(&ICMP_P);
    correct_ICMP_checksum = (int)GetICMPField(&ICMP_P, ICMP, ICMP_checksum);
    EditICMPField(&ICMP_P, ICMP, ICMP_checksum, (void *)(intptr_t)ICMPv4Config.INVALID_CHECKSUM);
    SendICMP(ICMP_P);

    // 2. TESTER: Listen for a response
    Packet_filter filter;
    strcpy(filter.dstAdd, (char*)ICMPv4Config.TESTER_IP);
    strcpy(filter.SrcAdd, (char*)ICMPv4Config.DUT_IP);
    ICMP_Packet ICMP_PR = ListenICMP(filter, ICMPv4Config.LISTEN_TIME);

    // 3. DUT: Do not send ICMP Echo Reply
    if (ICMPv4Config.INVALID_CHECKSUM == correct_ICMP_checksum)
    {
        printf("------------- ICMPv4_TYPE_10: Test failed -> You set an incorrect ICMP checksum -------------\n");
        return 1;
    }
    else if (ICMP_PR.length == 0)
    {
        printf("------------- ICMPv4_TYPE_10: Test passed -------------\n");
        return 0;
    }
    else
    {
        printf("------------- ICMPv4_TYPE_10: Test failed -> DUT sent a response -------------\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: ICMPv4_TYPE_11
*
***************************************************************************************************/
/**
* @brief
*  The data received (a timestamp) in the message is returned in the reply together with an 
*  additional timestamp.
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
int ICMPv4_TYPE_11()
{
    // 1. TESTER: Construct an ICMP Timestamp Echo Request
    ICMP_Timestamp_Packet ICMP_Timestamp_P = CreateICMPTimestamp();
    EditICMPTimestampField(&ICMP_Timestamp_P, ICMP_Timestamp, ICMP_Timestamp_originate_timestamp, (void *)ICMPv4Config.ORINGIN_TIMESTAMP_VALUE);
    ICMP_Timestamp_Compute_checksum(&ICMP_Timestamp_P);
    SendICMPTimestamp(ICMP_Timestamp_P);

    // 2. TESTER: Listen for a response
    Packet_filter filter;
    strcpy(filter.dstAdd, (char*)ICMPv4Config.TESTER_IP);
    strcpy(filter.SrcAdd, (char*)ICMPv4Config.DUT_IP);
    ICMP_Timestamp_Packet ICMP_PR = ListenICMPTimestamp(filter, ICMPv4Config.LISTEN_TIME);

    // 3. DUT: Send ICMP Timestamp Reply containing the same id and sequence as the ICMP request
    if (ICMP_PR.length == 0)
    {
        printf("------------- ICMPv4_TYPE_11: Test failed -> DUT not responding -------------\n");
        return 1;
    }
    else if ((uint32_t)GetICMPTimestampField(&ICMP_PR, ICMP_Timestamp, ICMP_Timestamp_originate_timestamp) == ICMPv4Config.ORINGIN_TIMESTAMP_VALUE && (uint32_t)GetICMPTimestampField(&ICMP_PR, ICMP_Timestamp, ICMP_Timestamp_receive_timestamp) != 0 && (uint32_t)GetICMPTimestampField(&ICMP_PR, ICMP_Timestamp, ICMP_Timestamp_transmit_timestamp) != 0)
    {
        printf("------------- ICMPv4_TYPE_11: Test passed -------------\n");
        return 0;
    }
    else
    {
        printf("------------- ICMPv4_TYPE_12: Test failed -------------\n");
        return 1;
    }   
}

/***************************************************************************************************
*
*   FUNCTION NAME: ICMPv4_TYPE_12
*
***************************************************************************************************/
/**
* @brief
*  The identifier and sequence number may be used by the echo sender to aid in matching the replies
*  with the requests.
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
int ICMPv4_TYPE_12()
{
    // 1. TESTER: Construct an ICMP Timestamp Echo Request
    ICMP_Timestamp_Packet ICMP_Timestamp_P = CreateICMPTimestamp();
   
    EditICMPTimestampField(&ICMP_Timestamp_P, ICMP_Timestamp, ICMP_Timestamp_identifier, (void *)ICMPv4Config.ICMP_IDENTIFIER);
    EditICMPTimestampField(&ICMP_Timestamp_P, ICMP_Timestamp, ICMP_Timestamp_sequence, (void *)ICMPv4Config.ICMP_SEQUENCE_NUMBER);
    ICMP_Timestamp_Compute_checksum(&ICMP_Timestamp_P);
    SendICMPTimestamp(ICMP_Timestamp_P);

    // 2. TESTER: Listen for a response
    Packet_filter filter;
    strcpy(filter.dstAdd, (char*)ICMPv4Config.TESTER_IP);
    strcpy(filter.SrcAdd, (char*)ICMPv4Config.DUT_IP);
    ICMP_Timestamp_Packet ICMP_PR = ListenICMPTimestamp(filter, ICMPv4Config.LISTEN_TIME);

    // 3. DUT: Send ICMP Timestamp Reply containing the same id and sequence as the ICMP request
    if (ICMP_PR.length == 0)
    {
        printf("------------- ICMPv4_TYPE_12: Test failed -> DUT not responding -------------\n");
        return 1;
    }
    else if ((uint16_t)GetICMPTimestampField(&ICMP_PR, ICMP_Timestamp, ICMP_Timestamp_identifier) == ICMPv4Config.ICMP_IDENTIFIER && (uint16_t)GetICMPTimestampField(&ICMP_PR, ICMP_Timestamp, ICMP_Timestamp_sequence) == ICMPv4Config.ICMP_SEQUENCE_NUMBER)
    {
        printf("------------- ICMPv4_TYPE_12: Test passed -------------\n");
        return 0;
    }
    else
    {
        printf("------------- ICMPv4_TYPE_12: Test failed -------------\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: ICMPv4_TYPE_16
*
***************************************************************************************************/
/**
* @brief
*  A host SHOULD NOT implement these messages. 
*
* @return int 0: The test  was successful | 1: The test failed 
*
* @note
*  Referring to Information Request or Information Reply Message.
*
* @warning
* -
*
***************************************************************************************************/
int ICMPv4_TYPE_16()
{
    // 1. TESTER: Construct an ICMP Echo Request
    ICMP_Packet ICMP_P = CreateICMP();
    EditICMPField(&ICMP_P, IP, IP_DstAddress, (void *)"0.0.0.0");
    EditICMPField(&ICMP_P, ICMP, ICMP_identifier, (void *)(intptr_t)ICMPv4Config.ICMP_IDENTIFIER);
    EditICMPField(&ICMP_P, ICMP, ICMP_sequence, (void *)(intptr_t)ICMPv4Config.ICMP_SEQUENCE_NUMBER);
    ICMP_Compute_checksum(&ICMP_P);
    SendICMP(ICMP_P);

    // 2. TESTER: Listen for a response
    Packet_filter filter;
    strcpy(filter.dstAdd, (char*)ICMPv4Config.TESTER_IP);
    strcpy(filter.SrcAdd, (char*)ICMPv4Config.DUT_IP);
    ICMP_Packet ICMP_PR = ListenICMP(filter, ICMPv4Config.LISTEN_TIME);

    // 3. DUT: Do not send ICMP Information Reply
    if (ICMP_PR.length == 0)
    {
        printf("------------- ICMPv4_TYPE_16: Test passed -------------\n");
        return 0;
    }
    else
    {
        printf("------------- ICMPv4_TYPE_16: Test failed -> DUT sent a response -------------\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: ICMPv4_TYPE_18
*
***************************************************************************************************/
/**
* @brief
*  A host must respond to an IPv4 Packet containnig an <unsupportedProtocol>, by sending a 
*  Destination Unreachable message including the Protcol Unreachable code.
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
int ICMPv4_TYPE_18()
{
    // 1. TESTER: Construct an IP packet containg an unsupported protocol
    IP_Packet IP_P = CreateIP();
    EditIPField(&IP_P, IP, IP_Protocol, (void *)(intptr_t)ICMPv4Config.UNSUPPORTED_PROTOCOL);
    IP_Compute_checksum(&IP_P);
    SendIP(IP_P);

    // 2. TESTER: Listen for a response
    Packet_filter filter;
    strcpy(filter.dstAdd, (char*)ICMPv4Config.TESTER_IP);
    strcpy(filter.SrcAdd, (char*)ICMPv4Config.DUT_IP);
    ICMP_Packet ICMP_PR = ListenICMP(filter, ICMPv4Config.LISTEN_TIME);

    // 3. DUT: Sends ICMPv4 Destination Unreachable message indicating Protocol Unreachable
    if (ICMP_PR.length == 0)
    {
        printf("------------- ICMPv4_TYPE_18: Test failed -> DUT not responding -------------\n");
        return 1;
    }
    else if ((int)GetICMPField(&ICMP_PR, ICMP, ICMP_type) == 3 && (int)GetICMPField(&ICMP_PR, ICMP, ICMP_code) == 2)
    {
        printf("------------- ICMPv4_TYPE_18: Test passed -------------\n");
        return 0;
    }
    else
    {
        printf("------------- ICMPv4_TYPE_18: Test failed -------------\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: ICMPv4_TYPE_22
*
***************************************************************************************************/
/**
* @brief
*  A host must respond to all ICMP Echo Requests sent to it, by sending an ICMP Echo Reply back to
*  the sender of ICMP Echo Request.
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
int ICMPv4_TYPE_22()
{
    // 1. TESTER: Construct an ICMP Echo Request
    ICMP_Packet ICMP_P = CreateICMP();
    ICMP_Compute_checksum(&ICMP_P);
    SendICMP(ICMP_P);

    // 2. TESTER: Listen for a response
    Packet_filter filter;
    strcpy(filter.dstAdd, (char*)ICMPv4Config.TESTER_IP);
    strcpy(filter.SrcAdd, (char*)ICMPv4Config.DUT_IP);
    ICMP_Packet ICMP_PR = ListenICMP(filter, ICMPv4Config.LISTEN_TIME);

    // 3. DUT: Send ICMP Echo Reply
    if (ICMP_PR.length == 0)
    {
        printf("------------- ICMPv4_TYPE_22: Test failed -> DUT not responding -------------\n");
        return 1;
    }
    else if ((int)GetICMPField(&ICMP_PR, ICMP, ICMP_code) == 0 && (int)GetICMPField(&ICMP_PR, ICMP, ICMP_type) == 0)
    {
        printf("------------- ICMPv4_TYPE_22: Test passed -------------\n");
        return 0;
    }
    else
    {
        printf("------------- ICMPv4_TYPE_22: Test failed -------------\n");
        return 1;
    }
}
