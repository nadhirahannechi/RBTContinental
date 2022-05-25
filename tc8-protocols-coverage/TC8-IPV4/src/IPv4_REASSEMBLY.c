#include "AbstractionAPI.h"
#include "IPv4_Common.h"
#include "IPv4config.h"
#include "IPv4_REASSEMBLY.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_REASSEMBLY_04
*
***************************************************************************************************/
/**
* @brief
*  Ensure that when the DUT receives a series of unordered IPv4 Fragments of a large IPv4 packet 
*  then the DUT reassembles and accept the IPv4 packet.
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
int IPv4_REASSEMBLY_04()
{
    // 1. TESTER: Send an IPv4 packet MF = 1 
    char firstPayload[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    char secondPayload[] = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
    char thirdPayload[] = "cccccccccccccccccccccccccccccccccccccccc";
    char finalPayload[] = "dddddddddddddddddddddddddddddddddddddddd";
    ICMP_Packet ICMP_P = CreateICMP();
    EditICMPField(&ICMP_P, PAYLAOD, PAYLOAD_data, (void *)firstPayload);
    EditICMPField(&ICMP_P, IP, IP_TotalLength, (void *)60);
    EditICMPField(&ICMP_P, IP, IP_Offset, (void *)0b0010000000000000);
    ICMP_Compute_checksum(&ICMP_P);
    EditICMPField(&ICMP_P, ICMP, ICMP_checksum, (void *)0x66e2);
    SendICMP(ICMP_P);

    // 2. TESTER: Send an IPv4 packet MF = 1 
    IP_Packet IP_P2 = CreateIP();
    EditIPField(&IP_P2, PAYLAOD, PAYLOAD_data, (void *)thirdPayload);
    EditIPField(&IP_P2, IP, IP_TotalLength, (void *)60);
    EditIPField(&IP_P2, IP, IP_Offset, (void *)0b0010000000001010);
    EditIPField(&IP_P2, IP, IP_Protocol, (void *)(intptr_t)IPv4Config.IP_TYPE_ICMP);
    IP_Compute_checksum(&IP_P2);
    SendIP(IP_P2);

    // 3. TESTER: Send an IPv4 packet MF = 1 
    IP_Packet IP_P3 = CreateIP();
    EditIPField(&IP_P3, PAYLAOD, PAYLOAD_data, (void *)secondPayload);
    EditIPField(&IP_P3, IP, IP_TotalLength, (void *)60);
    EditIPField(&IP_P3, IP, IP_Offset, (void *)0b0010000000000101);
    EditIPField(&IP_P3, IP, IP_Protocol, (void *)(intptr_t)IPv4Config.IP_TYPE_ICMP);
    IP_Compute_checksum(&IP_P3);
    SendIP(IP_P3);

    // 4. TESTER: Send an IPv4 packet MF = 0
    IP_Packet IP_P4 = CreateIP();
    EditIPField(&IP_P4, PAYLAOD, PAYLOAD_data, (void *)finalPayload);
    EditIPField(&IP_P4, IP, IP_TotalLength, (void *)60);
    EditIPField(&IP_P4, IP, IP_Offset, (void *)0b0000000000001111);
    EditIPField(&IP_P4, IP, IP_Protocol, (void *)(intptr_t)IPv4Config.IP_TYPE_ICMP);
    IP_Compute_checksum(&IP_P4);
    SendIP(IP_P4);

	Packet_filter filter;
    strcpy(filter.dstAdd, (char*)IPv4Config.TESTER_IP);
    strcpy(filter.SrcAdd, (char*)IPv4Config.DUT_IP);
    ICMP_Packet ICMP_PR = ListenICMP(filter, IPv4Config.LISTEN_TIME);


    // 5. DUT: Sends an ICMPv4 Echo Reply
    if(ICMP_PR.length == 0)
    {
        printf("------------- IPv4_REASSEMBLY_04: -> DUT not responding -------------");
        return 1;
    }
    else if((int)GetICMPField(&ICMP_PR ,ICMP, ICMP_type) == 0 && (int)GetICMPField(&ICMP_PR ,ICMP, ICMP_code) == 0)
    {
        printf("------------- IPv4_REASSEMBLY_04: Test passed -------------\n");
        return 0;
    }
    else
    {
        printf("------------- IPv4_REASSEMBLY_04: Test failed -------------\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_REASSEMBLY_06
*
***************************************************************************************************/
/**
* @brief
*  Ensure that when the DUT receives a series of IPv4 Fragments of a large IPv4 packet containing an
*  IPv4 Header containing an Offset indicating a value different than 0 but with correct increment
*  then the DUT does not reassemble and accept the IPv4 packet.
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
int IPv4_REASSEMBLY_06()
{
    // 1. TESTER: Send an IPv4 packet MF = 1 and Offset not equal to 0,
    char firstPayload[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    char lastPayload[] = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
    ICMP_Packet ICMP_P = CreateICMP();
    EditICMPField(&ICMP_P, PAYLAOD, PAYLOAD_data, (void *)firstPayload);
    EditICMPField(&ICMP_P, IP, IP_TotalLength, (void *)60);
    EditICMPField(&ICMP_P, IP, IP_Offset, (void *)0b0010000000000001);
    ICMP_Compute_checksum(&ICMP_P);
    SendICMP(ICMP_P);
    
    // 2. TESTER: Send an IPv4 packet MF = 0
    IP_Packet IP_P2 = CreateIP();
    EditIPField(&IP_P2, PAYLAOD, PAYLOAD_data, (void *)lastPayload);
    EditIPField(&IP_P2, IP, IP_TotalLength, (void *)60);
    EditIPField(&IP_P2, IP, IP_Offset, (void *)0b0000000000000101);
    EditIPField(&IP_P2, IP, IP_Protocol, (void *)(intptr_t)IPv4Config.IP_TYPE_ICMP);
    IP_Compute_checksum(&IP_P2);
    SendIP(IP_P2);

    // 3. DUT: Does not send an ICMPv4 Echo Reply
	Packet_filter filter;
    strcpy(filter.dstAdd, (char*)IPv4Config.TESTER_IP);
    strcpy(filter.SrcAdd, (char*)IPv4Config.DUT_IP);
    ICMP_Packet ICMP_PR = ListenICMP(filter, IPv4Config.LISTEN_TIME);

    if(ICMP_PR.length == 0)
    {
        printf("------------- IPv4_REASSEMBLY_06: Test passed -------------\n");
        return 0;
    }
    else
    {
        printf("------------- IPv4_REASSEMBLY_06: Test failed -> DUT sent a response -------------\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_REASSEMBLY_07
*
***************************************************************************************************/
/**
* @brief
*  Ensure that when the DUT receives a series of IPv4 Fragments of a large IPv4 packet including the
*  first and the last fragment but missing some in between then the DUT does not reassemble and 
*  accept the IPv4 packet.
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
int IPv4_REASSEMBLY_07()
{
    // 1. TESTER: Send an IPv4 packet MF = 1 
    char firstPayload[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    char secondPayload[] = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
    char lastPayload[] = "cccccccccccccccccccccccccccccccccccccccc";
    ICMP_Packet ICMP_P = CreateICMP();
    EditICMPField(&ICMP_P, PAYLAOD, PAYLOAD_data, (void *)firstPayload);
    EditICMPField(&ICMP_P, IP, IP_TotalLength, (void *)60);
    EditICMPField(&ICMP_P, IP, IP_Offset, (void *)0b0010000000000000);
    ICMP_Compute_checksum(&ICMP_P);
    EditICMPField(&ICMP_P, ICMP, ICMP_checksum, (void *)0x0682);
    SendICMP(ICMP_P);

    // 2. TESTER: Send an IPv4 packet MF = 1 
    IP_Packet IP_P = CreateIP();
    EditIPField(&IP_P, PAYLAOD, PAYLOAD_data, (void *)secondPayload);
    EditIPField(&IP_P, IP, IP_TotalLength, (void *)60);
    EditIPField(&IP_P, IP, IP_Offset, (void *)0b0010000000000101);
    EditIPField(&IP_P, IP, IP_Protocol, (void *)(intptr_t)IPv4Config.IP_TYPE_ICMP);
    IP_Compute_checksum(&IP_P);
    SendIP(IP_P);

    // 3. TESTER: Send an IPv4 packet MF = 0 and Offset is incremented as if one more fragment would have been sent
    IP_Packet IP_P2 = CreateIP();
    EditIPField(&IP_P2, PAYLAOD, PAYLOAD_data, (void *)lastPayload);
    EditIPField(&IP_P2, IP, IP_TotalLength, (void *)60);
    EditIPField(&IP_P2, IP, IP_Offset, (void *)0b0000000000001111);
    EditIPField(&IP_P2, IP, IP_Protocol, (void *)(intptr_t)IPv4Config.IP_TYPE_ICMP);
    IP_Compute_checksum(&IP_P2);
    SendIP(IP_P2);

    // 4. DUT: Does not send an ICMPv4 Echo Reply
	Packet_filter filter;
    strcpy(filter.dstAdd, (char*)IPv4Config.TESTER_IP);
    strcpy(filter.SrcAdd, (char*)IPv4Config.DUT_IP);
    ICMP_Packet ICMP_PR = ListenICMP(filter, IPv4Config.LISTEN_TIME);

    if(ICMP_PR.length == 0)
    {
        printf("------------- IPv4_REASSEMBLY_07: Test passed -------------\n");
        return 0;
    }
    else
    {
        printf("------------- IPv4_REASSEMBLY_07: Test failed -> DUT sent a response -------------\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_REASSEMBLY_09
*
***************************************************************************************************/
/**
* @brief
*  Ensure that when the DUT receives an IPv4 packet containing an IPv4 Header containing Flags and
*  containing a MF flag indicating that there are more fragments coming: MF = 1, then the DUT 
*  discards the IPv4 Packet silently.
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
int IPv4_REASSEMBLY_09()
{
    // 1. TESTER: Send an ICMPv4 Echo Request with MF = 1
    char firstPayload[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    ICMP_Packet ICMP_P = CreateICMP();
    EditICMPField(&ICMP_P, PAYLAOD, PAYLOAD_data, (void *)firstPayload);
    EditICMPField(&ICMP_P, IP, IP_TotalLength, (void *)60);
    EditICMPField(&ICMP_P, IP, IP_Offset, (void *)0b0010000000000000);
    ICMP_Compute_checksum(&ICMP_P);
    SendICMP(ICMP_P);

    // 2. DUT: Does not send an ICMPv4 Echo Reply
	Packet_filter filter;
    strcpy(filter.dstAdd, (char*)IPv4Config.TESTER_IP);
    strcpy(filter.SrcAdd, (char*)IPv4Config.DUT_IP);
    ICMP_Packet ICMP_PR = ListenICMP(filter, IPv4Config.LISTEN_TIME);


    if(ICMP_PR.length == 0)
    {
        printf("------------- IPv4_REASSEMBLY_09: Test passed -------------\n");
        return 0;
    }
    else
    {
        printf("------------- IPv4_REASSEMBLY_09: Test failed -> DUT sent a response -------------\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_REASSEMBLY_10
*
***************************************************************************************************/
/**
* @brief
*  The current recommendation for the initial timer setting is 15 seconds.
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
int IPv4_REASSEMBLY_10()
{
    // 1. TESTER: Construct an ICMP Echo Request. Send an IP packet with ttl = 15 and MF = 1
    char firstPayload[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    char lastPayload[] = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
    char payload[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
    ICMP_Packet ICMP_P = CreateICMP();
    EditICMPField(&ICMP_P, PAYLAOD, PAYLOAD_data, (void *)firstPayload);
    EditICMPField(&ICMP_P, IP, IP_TotalLength, (void *)60);
    EditICMPField(&ICMP_P, IP, IP_Offset, (void *)0b0010000000000000);
    EditICMPField(&ICMP_P, IP, IP_Ttl, (void *)15);
    ICMP_Compute_checksum(&ICMP_P);
    EditICMPField(&ICMP_P, ICMP, ICMP_checksum, (void *)0x027e);
    SendICMP(ICMP_P);
    sleep(IPv4Config.IP_INI_REASSEMBLE_TIMEOUT - PARAMETER_TOLERANCE_TIME);

    // 2. Send an IP packet with ttl = 15 and MF = 0
    IP_Packet IP_P1 = CreateIP();
    EditIPField(&IP_P1, PAYLAOD, PAYLOAD_data, (void *)lastPayload);
    EditIPField(&IP_P1, IP, IP_TotalLength, (void *)60);
    EditIPField(&IP_P1, IP, IP_Offset, (void *)0b0000000000000101);
    EditIPField(&IP_P1, IP, IP_Ttl, (void *)15);
    EditIPField(&IP_P1, IP, IP_Protocol, (void *)(intptr_t)IPv4Config.IP_TYPE_ICMP);
    IP_Compute_checksum(&IP_P1);
    SendIP(IP_P1);

    // 3. DUT: Send ICMP Echo Reply
	Packet_filter filter;
    strcpy(filter.dstAdd, (char*)IPv4Config.TESTER_IP);
    strcpy(filter.SrcAdd, (char*)IPv4Config.DUT_IP);
    ICMP_Packet ICMP_PR = ListenICMP(filter, IPv4Config.LISTEN_TIME);

    if(ICMP_PR.length == 0)
    {
        printf("------------- IPv4_REASSEMBLY_10: Test failed -> DUT not responding -------------\n");
        return 1;
    }
    else if
    (
        (int)GetICMPField(&ICMP_PR, ICMP, ICMP_identifier) == (int)GetICMPField(&ICMP_P, ICMP, ICMP_identifier) 
        &&
        (int)GetICMPField(&ICMP_PR, ICMP, ICMP_sequence) == (int)GetICMPField(&ICMP_P, ICMP, ICMP_sequence)
        &&
        !strcmp((char*)GetICMPField(&ICMP_PR ,PAYLAOD ,PAYLOAD_data), payload)
    )
    { 
        // 4. Send an IP packet with ttl = 15 and MF = 1
        ICMP_Packet ICMP_P1 = CreateICMP();
        EditICMPField(&ICMP_P1, PAYLAOD, PAYLOAD_data, (void *)firstPayload);
        EditICMPField(&ICMP_P1, IP, IP_TotalLength, (void *)60);
        EditICMPField(&ICMP_P1, IP, IP_Offset, (void *)0b0010000000000000);
        EditICMPField(&ICMP_P1, IP, IP_Ttl, (void *)15);
        ICMP_Compute_checksum(&ICMP_P1);
        EditICMPField(&ICMP_P1, ICMP, ICMP_checksum, (void *)0xe848);
        SendICMP(ICMP_P1);
        sleep(IPv4Config.IP_INI_REASSEMBLE_TIMEOUT + PARAMETER_TOLERANCE_TIME);

        // 4. Send an IP packet with ttl = 15 and MF = 0
        IP_Packet IP_P3 = CreateIP();
        EditIPField(&IP_P3, PAYLAOD, PAYLOAD_data, (void *)lastPayload);
        EditIPField(&IP_P3, IP, IP_TotalLength, (void *)60);
        EditIPField(&IP_P3, IP, IP_Offset, (void *)0b0000000000000101);
        EditIPField(&IP_P3, IP, IP_Ttl, (void *)15);
        EditIPField(&IP_P3, IP, IP_Protocol, (void *)(intptr_t)IPv4Config.IP_TYPE_ICMP);
        IP_Compute_checksum(&IP_P3);    
        SendIP(IP_P3);
    
        // 5. DUT: Do not send ICMP Echo Reply
        ICMP_Packet ICMP_PR = ListenICMP(filter, IPv4Config.LISTEN_TIME);
        if(ICMP_PR.length == 0)
        {
            printf("------------- IPv4_REASSEMBLY_10: Test passed -------------\n");
            sleep(IPv4Config.IP_INI_REASSEMBLE_TIMEOUT);
            return 0;
        }
        else
        {
            printf("------------- IPv4_REASSEMBLY_10: Test failed -> DUT sent a response-------------\n");
            sleep(IPv4Config.IP_INI_REASSEMBLE_TIMEOUT);
            return 1;
        }
    }
    else
    {
        printf("------------- IPv4_REASSEMBLY_10: Test failed -------------\n");
        sleep(IPv4Config.IP_INI_REASSEMBLE_TIMEOUT);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_REASSEMBLY_11
*
***************************************************************************************************/
/**
* @brief
*  The initial setting of the timer is a lower bound on the reassembly waiting time. This is because
*  the waiting time will be increased if the Time to Live in the arriving fragment is greater than
*  the current timer value.
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
int IPv4_REASSEMBLY_11()
{
    // 1. TESTER: Construct an ICMP Echo Request. Send an IP packet with large TTL
    char firstPayload[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    char lastPayload[] = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
    char payload[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
    ICMP_Packet ICMP_P = CreateICMP();
    EditICMPField(&ICMP_P, PAYLAOD, PAYLOAD_data, (void *)firstPayload);
    EditICMPField(&ICMP_P, IP, IP_TotalLength, (void *)60);
    EditICMPField(&ICMP_P, IP, IP_Offset, (void *)0b0010000000000000);
    EditICMPField(&ICMP_P, IP, IP_Ttl, (void *)(intptr_t)IPv4Config.LARGE_TTL_VALUE);
    ICMP_Compute_checksum(&ICMP_P);    
    EditICMPField(&ICMP_P, ICMP, ICMP_checksum, (void *)0x027e);
    SendICMP(ICMP_P);
    sleep(IPv4Config.IP_INI_REASSEMBLE_TIMEOUT + PARAMETER_TOLERANCE_TIME);

    // 2. TESTER: Send an IP packet with large TTL
    IP_Packet IP_P2 = CreateIP();
    EditIPField(&IP_P2, PAYLAOD, PAYLOAD_data, (void *)lastPayload);
    EditIPField(&IP_P2, IP, IP_TotalLength, (void *)60);
    EditIPField(&IP_P2, IP, IP_Offset, (void *)0b0000000000000101);
    EditIPField(&IP_P2, IP, IP_Protocol, (void *)(intptr_t)IPv4Config.IP_TYPE_ICMP);
    EditIPField(&IP_P2, IP, IP_Ttl, (void *)(intptr_t)IPv4Config.LARGE_TTL_VALUE);
    IP_Compute_checksum(&IP_P2);    
    SendIP(IP_P2);
    sleep(IPv4Config.IP_INI_REASSEMBLE_TIMEOUT - PARAMETER_TOLERANCE_TIME);

    // 3. DUT: Send ICMP Echo Reply
	Packet_filter filter;
    strcpy(filter.dstAdd, (char*)IPv4Config.TESTER_IP);
    strcpy(filter.SrcAdd, (char*)IPv4Config.DUT_IP);
    ICMP_Packet ICMP_PR = ListenICMP(filter, IPv4Config.LISTEN_TIME);

    if(ICMP_PR.length == 0)
    {
        printf("------------- IPv4_REASSEMBLY_11: Test failed -> DUT not responding -------------\n");
        return 1;
    }
    else if
    (
        (int)GetICMPField(&ICMP_PR, ICMP, ICMP_identifier) == (int)GetICMPField(&ICMP_P, ICMP, ICMP_identifier) 
        &&
        (int)GetICMPField(&ICMP_PR, ICMP, ICMP_sequence) == (int)GetICMPField(&ICMP_P, ICMP, ICMP_sequence)
        &&
        !strcmp((char*)GetICMPField(&ICMP_PR ,PAYLAOD ,PAYLOAD_data), payload)
    )
    {
        printf("------------- IPv4_REASSEMBLY_11: Test passed -------------\n");
        return 0;
    }
    else
    {
        printf("------------- IPv4_REASSEMBLY_11: Test failed -------------\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_REASSEMBLY_12
*
***************************************************************************************************/
/**
* @brief
*  The initial setting of the timer is a lower bound on the reassembly waiting time. The waiting
*  time will not be decreased if it is less the Time to Live in the arriving fragment.
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
int IPv4_REASSEMBLY_12()
{
    // 1. TESTER: Construct an ICMP Echo Request. Send an IP packet with low TTL
    char firstPayload[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    char lastPayload[] = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
    char payload[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
    ICMP_Packet ICMP_P = CreateICMP();
    EditICMPField(&ICMP_P, PAYLAOD, PAYLOAD_data, (void *)firstPayload);
    EditICMPField(&ICMP_P, IP, IP_TotalLength, (void *)60);
    EditICMPField(&ICMP_P, IP, IP_Offset, (void *)0b0010000000000000);
    EditICMPField(&ICMP_P, IP, IP_Ttl, (void *)(intptr_t)IPv4Config.LOW_TTL_VALUE);
    ICMP_Compute_checksum(&ICMP_P);    
    EditICMPField(&ICMP_P, ICMP, ICMP_checksum, (void *)0x027e);
    SendICMP(ICMP_P);

    // 2. TESTER: Send an IP packet with low TTL
    IP_Packet IP_P2 = CreateIP();
    EditIPField(&IP_P2, PAYLAOD, PAYLOAD_data, (void *)lastPayload);
    EditIPField(&IP_P2, IP, IP_TotalLength, (void *)60);
    EditIPField(&IP_P2, IP, IP_Offset, (void *)0b0000000000000101);
    EditIPField(&IP_P2, IP, IP_Protocol, (void *)(intptr_t)IPv4Config.IP_TYPE_ICMP);
    EditIPField(&IP_P2, IP, IP_Ttl, (void *)(intptr_t)IPv4Config.LOW_TTL_VALUE);
    IP_Compute_checksum(&IP_P2);    
    SendIP(IP_P2);

    // 3. Wait for ICMP Echo Reply and verify that Identifier, Sequence Number and Data of ICMP Echo Reply are same as those of ICMP Echo Request sent in two fragments.
	Packet_filter filter;
    strcpy(filter.dstAdd, (char*)IPv4Config.TESTER_IP);
    strcpy(filter.SrcAdd, (char*)IPv4Config.DUT_IP);
    ICMP_Packet ICMP_PR = ListenICMP(filter, IPv4Config.LISTEN_TIME);


    if(ICMP_PR.length == 0)
    {
        printf("------------- IPv4_REASSEMBLY_12: Test failed -> DUT not responding -------------\n");
        return 1;
    }
    else if
    (
        (int)GetICMPField(&ICMP_PR, ICMP, ICMP_identifier) == (int)GetICMPField(&ICMP_P, ICMP, ICMP_identifier) 
        &&
        (int)GetICMPField(&ICMP_PR, ICMP, ICMP_sequence) == (int)GetICMPField(&ICMP_P, ICMP, ICMP_sequence)
        &&
        !strcmp((char*)GetICMPField(&ICMP_PR ,PAYLAOD ,PAYLOAD_data), payload)
    )
    {
        printf("------------- IPv4_REASSEMBLY_12: Test passed -------------\n");
        return 0;
    }
    else
    {
        printf("------------- IPv4_REASSEMBLY_12: Test failed -------------\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_REASSEMBLY_13
*
***************************************************************************************************/
/**
* @brief
*  In the case that two or more fragments contain the same data either identically or through a 
*  partial overlap, this procedure will use the more recently arrived copy in the data buffer and 
*  datagram delivered.
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
int IPv4_REASSEMBLY_13()
{/*
    char payload1_frag1[] = "ECU NETWORK VALI";
    char payload1_frag2[] = "DATION T";
    char payload1_frag3[] = "EST";
    char payload2[] = "DUPLICATE FRAGMENTS TEST";
    char finalPayload[] = "ECU NETWORK VALIDATION TEST";

    // 1. TESTER: Construct an ICMP Echo Request, containing: - Data field set to "ECU NETWORK VALIDATION TEST"
    ICMP_Packet ICMP_P1 = CreateICMP();
    EditICMPField(&ICMP_P1, PAYLAOD, PAYLOAD_data, (void *)payload1_frag1);
    EditICMPField(&ICMP_P1, IP, IP_TotalLength, (void *)44);
    EditICMPField(&ICMP_P1, IP, IP_Offset, (void *)0b0010000000000000);
    ICMP_Compute_checksum(&ICMP_P1);    
    EditICMPField(&ICMP_P1, ICMP, ICMP_checksum, (void *)0xa3ec);
    SendICMP(ICMP_P1);

    // 2. DUT: Do not send ICMP Echo Reply
	Packet_filter filter;
    strcpy(filter.dstAdd, (char*)IPv4Config.TESTER_IP);
    strcpy(filter.SrcAdd, (char*)IPv4Config.DUT_IP);
    ICMP_Packet ICMP_PR1 = ListenICMP(filter, IPv4Config.FRAGMENT_REASSEMBLY_TIMEOUT/4);
    if(ICMP_PR1.length == 0)
    {
        // 3. TESTER: Send an IP packet
        IP_Packet IP_P1 = CreateIP();
        EditIPField(&IP_P1, PAYLAOD, PAYLOAD_data, (void *)payload2);
        EditIPField(&IP_P1, IP, IP_TotalLength, (void *)44);
        EditIPField(&IP_P1, IP, IP_Offset, (void *)0b0010000000000011);
        EditIPField(&IP_P1, IP, IP_Protocol, (void *)(intptr_t)IPv4Config.IP_TYPE_ICMP);
        IP_Compute_checksum(&IP_P1);    
        SendIP(IP_P1);
        
        // 4. DUT: Do not send ICMP Echo Reply
        IP_Packet IP_PR1 = ListenIP(filter, IPv4Config.FRAGMENT_REASSEMBLY_TIMEOUT/4);

        if(IP_PR1.length == 0)
        {
            // 5. TESTER: Send an IP packet
            IP_Packet IP_P2 = CreateIP();
            EditIPField(&IP_P2, PAYLAOD, PAYLOAD_data, (void *)payload1_frag2);
            EditIPField(&IP_P2, IP, IP_TotalLength, (void *)28);
            EditIPField(&IP_P2, IP, IP_Offset, (void *)0b0010000000000011);
            EditIPField(&IP_P2, IP, IP_Protocol, (void *)(intptr_t)IPv4Config.IP_TYPE_ICMP);
            IP_Compute_checksum(&IP_P2);
            SendIP(IP_P2);

            // 6. DUT: Do not send ICMP Echo Reply
            IP_Packet IP_PR2 = ListenIP(filter, IPv4Config.FRAGMENT_REASSEMBLY_TIMEOUT/4);

            if(IP_PR2.length == 0)
            {
                // 7. TESTER: Send an IP packet
                IP_Packet IP_P3 = CreateIP();
                EditIPField(&IP_P3, PAYLAOD, PAYLOAD_data, (void *)payload1_frag3);
                EditIPField(&IP_P3, IP, IP_TotalLength, (void *)23);
                EditIPField(&IP_P3, IP, IP_Offset, (void *)0b0000000000000100);
                EditIPField(&IP_P3, IP, IP_Protocol, (void *)(intptr_t)IPv4Config.IP_TYPE_ICMP);
                IP_Compute_checksum(&IP_P3);    
                SendIP(IP_P3);

                // 8. TESTER: Verify that the received ICMP Echo Reply contains: - Data field set to "ECU NETWORK VALIDATION TEST"
                ICMP_Packet ICMP_PR = ListenICMP(filter, IPv4Config.FRAGMENT_REASSEMBLY_TIMEOUT/4);
                if(ICMP_PR.length == 0)
                {
                    printf("------------- IPv4_REASSEMBLY_13: Test failed -> DUT not responding -------------\n");
                    return 1;
                }
                else if(!strcmp((char*)GetICMPField(&ICMP_PR ,PAYLAOD ,PAYLOAD_data), finalPayload))
                {
                    printf("------------- IPv4_REASSEMBLY_13: Test passed -------------\n");
                    return 0;
                }
                else
                {
                    printf("------------- IPv4_REASSEMBLY_13: Test failed -------------\n");
                    return 1;
                }
            }
            else
            {
                printf("------------- IPv4_REASSEMBLY_10: Test failed -> DUT sent a response -------------\n");
                return 1;
            }
        }
        else
        {
            printf("------------- IPv4_REASSEMBLY_10: Test failed -> DUT sent a response -------------\n");
            return 1;
        }
    }
    else
    {
        printf("------------- IPv4_REASSEMBLY_10: Test failed -> DUT sent a response -------------\n");
        return 1;
    }*/
    return 2;
}
