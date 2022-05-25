#include "AbstractionAPI.h"
#include "IPv4_HEADER.h"
#include "IPv4config.h"
#include <stdio.h>
#include <string.h>

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_HEADER_01
*
***************************************************************************************************/
/**
* @brief
*  Ensure that when the DUT is requested to generate an IPv4 packet, then the DUT generates an IPv4 
*  Packet containing an IPv4 Header containing a Total Length indicating a value greater than or 
*  equal to 20.
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
int IPv4_HEADER_01()
{
    // 1. TESTER: Send an ICMPv4 Echo Request
    ICMP_Packet ICMP_P = CreateICMP();
    ICMP_Compute_checksum(&ICMP_P);
    SendICMP(ICMP_P);

    // 2. TESTER: Listen for a response
	Packet_filter filter;
    strcpy(filter.dstAdd, (char*)IPv4Config.TESTER_IP);
    strcpy(filter.SrcAdd, (char*)IPv4Config.DUT_IP);

    ICMP_Packet ICMP_PR = ListenICMP(filter, IPv4Config.LISTEN_TIME);

    // 3. DUT: Generates an ICMPv4 Echo Reply including a Total Length Frame in the IPv4 Header greater or equal to 20
    if(ICMP_PR.length == 0)
    {
        printf("------------- IPv4_HEADER_01: Test failed -> DUT not responding -------------\n");
        return 1;
    }
    else if((int)GetICMPField(&ICMP_PR, IP, IP_TotalLength) >= 20)
    {
        printf("------------- IPv4_HEADER_01: Test passed -------------\n");
        return 0;
    }
    else
    {
        printf("------------- IPv4_HEADER_01: Test failed -------------\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_HEADER_02
*
***************************************************************************************************/
/**
* @brief
*  Ensure that when the DUT receives an IPv4 packet containing an IPv4 Header containing a Header
*  Length indicating a value less than 20, then the DUT discards the IPv4 Packet silently.
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
int IPv4_HEADER_02() /*didn't detect any icmp request on wiresark*/
{
    // 1. TESTER: Send an ICMPv4 Echo Request
    ICMP_Packet ICMP_P = CreateICMP();
    EditICMPField(&ICMP_P, IP, IP_IHL, (void *)4);
    ICMP_Compute_checksum(&ICMP_P);
    SendICMP(ICMP_P);

    // 2. TESTER: Listen for a response
	Packet_filter filter;
    strcpy(filter.dstAdd, (char*)IPv4Config.TESTER_IP);
    strcpy(filter.SrcAdd, (char*)IPv4Config.DUT_IP);

    ICMP_Packet ICMP_PR = ListenICMP(filter, IPv4Config.LISTEN_TIME);
    
    // 3. DUT: Does not send an ICMPv4 Echo Reply
    if(ICMP_PR.length == 0)
    {
        printf("------------- IPv4_HEADER_02: Test passed -------------\n");
        return 0;
    }
    else
    {
        printf("------------- IPv4_HEADER_02: Test failed -> DUT sent a response -------------\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_HEADER_03
*
***************************************************************************************************/
/**
* @brief
*  Ensure that when the DUT is requested to generate an IPv4 packet, then the DUT sends an IPv4 
*  Packet containing an IPv4 Header containing a Source Address indicating one of its defined IPv4 
*  addresses.
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
int IPv4_HEADER_03()
{
    // 1. TESTER: Send an ICMPv4 Echo Request
    ICMP_Packet ICMP_P = CreateICMP();
    ICMP_Compute_checksum(&ICMP_P);
    SendICMP(ICMP_P);
    
    // 2. TESTER: Listen for a response 
	Packet_filter filter;
    strcpy(filter.dstAdd, (char*)IPv4Config.TESTER_IP);
    strcpy(filter.SrcAdd, (char*)IPv4Config.DUT_IP);

    ICMP_Packet ICMP_PR = ListenICMP(filter, IPv4Config.LISTEN_TIME);

    // 3. DUT: Generates an ICMPv4 Echo Reply with Source Address being one of its defined IPv4 addresses
    if(ICMP_PR.length == 0)
    {
        printf("------------- IPv4_HEADER_03: Test failed -> DUT not responding -------------\n");
        return 1;
    }
    else if(!strcmp((char*)GetICMPField(&ICMP_PR, IP, IP_SrcAddress), (char*)GetICMPField(&ICMP_P, IP, IP_DstAddress)))
    {
        printf("------------- IPv4_HEADER_03: Test passed -------------\n");
        return 0;
    }
    else
    {
        printf("------------- IPv4_HEADER_03: Test failed -------------\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_HEADER_04
*
***************************************************************************************************/
/**
* @brief
*  Ensure that when the DUT receives an IPv4 packet containing an IPv4 Header containing a Destination
*  Address indicating a value different from the DUT’s IPv4 address and is not a Broadcast or Multicast
*  address, then the DUT discards the IPv4 Packet silently.
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
int IPv4_HEADER_04()
{
    // 1. TESTER: Send an ICMPv4 Echo Request
    ICMP_Packet ICMP_P = CreateICMP();
    EditICMPField(&ICMP_P, IP, IP_DstAddress, (void *) INCORRECT_DESTINATION_ADDRESS);
    ICMP_Compute_checksum(&ICMP_P);
    SendICMP(ICMP_P);
    
    // 2. TESTER: Listen for a response 
	Packet_filter filter;
    strcpy(filter.dstAdd, (char*)IPv4Config.TESTER_IP);
    strcpy(filter.SrcAdd, (char*)IPv4Config.DUT_IP);

    ICMP_Packet ICMP_PR = ListenICMP(filter, IPv4Config.LISTEN_TIME);

    // 3. DUT: Does not send an ICMPv4 Echo Reply
    if(ICMP_PR.length == 0)
    {
        printf("------------- IPv4_HEADER_04: Test passed ------------- \n");
        return 0;
    }
    else
    {
        printf("------------- IPv4_HEADER_04: Test failed -> DUT sent a response -------------\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_HEADER_05
*
***************************************************************************************************/
/**
* @brief
*  All hosts must be prepared to accept datagrams of up to 576 octets.
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
int IPv4_HEADER_05()
{
    // 1. TESTER: Send an ICMPv4 Echo Request
    char payload[548];
    memset(payload, 1, 548);
    ICMP_Packet ICMP_P = CreateICMP();
    EditICMPField(&ICMP_P, IP, IP_TotalLength, (void *)576);
    EditICMPField(&ICMP_P, PAYLAOD, PAYLOAD_data, (void *)payload);
    ICMP_Compute_checksum(&ICMP_P);
    SendICMP(ICMP_P);

    // 2. TESTER: Listen for a response 
	Packet_filter filter;
    strcpy(filter.dstAdd, (char*)IPv4Config.TESTER_IP);
    strcpy(filter.SrcAdd, (char*)IPv4Config.DUT_IP);

    ICMP_Packet ICMP_PR = ListenICMP(filter, IPv4Config.LISTEN_TIME);

    // 3. DUT: Send ICMP Reply containing the same id, sequence number and data as the ICMP request 
    if(ICMP_PR.length == 0)
    {
        printf("------------- IPv4_HEADER_5: Test failed -> DUT not responding -------------\n");
        return 1;
    }
    else if
    (
        ((int*)GetICMPField(&ICMP_PR, ICMP, ICMP_identifier) == (int*)GetICMPField(&ICMP_P, ICMP, ICMP_identifier)) 
        && 
        !strcmp((char*)GetICMPField(&ICMP_PR, PAYLAOD, PAYLOAD_data), (char*)GetICMPField(&ICMP_P, PAYLAOD, PAYLOAD_data))
    )
    {
        printf("------------- IPv4_HEADER_05: Test passed -------------\n");
        return 0;
    }
    else
    {
        printf("------------- IPv4_HEADER_05: Test failed -------------\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_HEADER_08
*
***************************************************************************************************/
/**
* @brief
*  Internet Header Length is the length of the internet header in 32 bit words, and thus points to 
*  the beginning of the data. Note that the minimum value for a correct header is 5. 
*
* @return int 0: The test  was successful | 1: The test failed
*
* @note
*  Tests that DUT discards a packet with total length smaller than implied by IHL value.
*
* @warning
* -
*
***************************************************************************************************/
int IPv4_HEADER_08()
{
    // 1. TESTER: Send an ICMPv4 Echo Request with IP_IHL = 13
    char payload[] = "ECU NETWORK VALIDATION TEST";
    ICMP_Packet ICMP_P = CreateICMP();
    EditICMPField(&ICMP_P, PAYLAOD, PAYLOAD_data, (void *)payload);
    EditICMPField(&ICMP_P, IP, IP_IHL, (void *)13);
    ICMP_Compute_checksum(&ICMP_P);
    SendICMP(ICMP_P);
    
    // 2. TESTER: Listen for a response
	Packet_filter filter;
    strcpy(filter.dstAdd, (char*)IPv4Config.TESTER_IP);
    strcpy(filter.SrcAdd, (char*)IPv4Config.DUT_IP);

    ICMP_Packet ICMP_PR = ListenICMP(filter, IPv4Config.LISTEN_TIME);

    // 3. Does not Send ICMP Echo Reply
    if(ICMP_PR.length == 0)
    {
        printf("------------- IPv4_HEADER_08: Test passed -------------\n");
        return 0;
    }
    else
    {
        printf("------------- IPv4_HEADER_08: Test failed -> DUT sent a response -------------\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_HEADER_09
*
***************************************************************************************************/
/**
* @brief
*  Total Length is the length of the datagram, measured in octets, including internet header and 
*  data.
*
* @return int 0: The test  was successful | 1: The test failed 
*
* @note
*  Tests that DUT discards a packet with total length bigger than the actual transmitted data.
*
* @warning
* -
*
***************************************************************************************************/
int IPv4_HEADER_09() /* Problem */
{
    // 1. TESTER: Send an ICMPv4 Echo Request
    char payload[] = "ECU NETWORK VALIDATION TEST";
    ICMP_Packet ICMP_P = CreateICMP();
    EditICMPField(&ICMP_P, PAYLAOD, PAYLOAD_data, (void *)payload);
    EditICMPField(&ICMP_P, IP, IP_TotalLength, (void *)48);
    ICMP_Compute_checksum(&ICMP_P);
    SendICMP(ICMP_P);

    // 2. TESTER: Listen for a response
	Packet_filter filter;
    strcpy(filter.dstAdd, (char*)IPv4Config.TESTER_IP);
    strcpy(filter.SrcAdd, (char*)IPv4Config.DUT_IP);

    ICMP_Packet ICMP_PR = ListenICMP(filter, IPv4Config.LISTEN_TIME);

    // 3. DUT: Does not Send ICMP Echo Reply
    if(ICMP_PR.length == 0)
    {
        printf("------------- IPv4_HEADER_09: Test passed -------------\n");
        return 0;
    }
    else
    {
        printf("------------- IPv4_HEADER_09: Test failed -> DUT sent a response -------------\n");
        return 1;
    }
}
