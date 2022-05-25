#include "AbstractionAPI.h"
#include "TestabilityProtocol_api.h"
#include "TestabilityProtocol_Intern.h"
#include "TestabilityProtocol_cbTypes.h"
#include "TestabilityProtocolTypes.h"
#include "IPv4_FRAGMENTS.h"
#include "IPv4_Common.h"
#include "IPv4config.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

uint8_t OK = 1;
uint16_t socketId;

// Call back functions
void GenaralStartTest_cb(TP_ResultID_t A)
{
	OK = A;
}

void UdpCreateAndBind_cb(TP_ResultID_t A, uint16 s)
{
	OK = A;
	socketId = s;
}

void TP_UdpSendData_cb (TP_ResultID_t A)
{
	OK = A;
}

void UdpCloseSocket_cb(TP_ResultID_t A)
{
	OK = A;
}

void GenaralEndTest_cb(TP_ResultID_t A)
{
	OK = A;
}


/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_FRAGMENTS_01
*
***************************************************************************************************/
/**
* @brief
*  To assemble the fragments of an internet datagram, an internet protocol module (for example at a
*  destination host) combines internet datagrams that all have the same value for the four fields: 
*  identification, source, destination, and protocol.
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
int IPv4_FRAGMENTS_01()
{
    // 1. TESTER: Construct an ICMP Echo Request. Send an IP packet
    char firstHalfPayload[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    char secondHalfPayload[] = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
    char totalPayload[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
    ICMP_Packet ICMP_P = CreateICMP();
    EditICMPField(&ICMP_P, PAYLAOD, PAYLOAD_data, (void *)firstHalfPayload);
    EditICMPField(&ICMP_P, IP, IP_TotalLength, (void *)60);
    EditICMPField(&ICMP_P, IP, IP_Identification, (void *)0x0020);
    EditICMPField(&ICMP_P, IP, IP_Offset, (void *)0b0010000000000000);
    EditICMPField(&ICMP_P, ICMP, ICMP_identifier, (void *)0x9735);
    EditICMPField(&ICMP_P, ICMP, ICMP_sequence, (void *)0x0600);
    ICMP_Compute_checksum(&ICMP_P);
    EditICMPField(&ICMP_P, ICMP, ICMP_checksum, (void *)0x9504);
    SendICMP(ICMP_P);

    // 2. TESTER: Send an IP packet
    IP_Packet IP_P = CreateIP();
    EditIPField(&IP_P, PAYLAOD, PAYLOAD_data, (void *)secondHalfPayload);
    EditIPField(&IP_P, IP, IP_TotalLength, (void *)60);
    EditIPField(&IP_P, IP, IP_Identification, (void *)0x0020);
    EditIPField(&IP_P, IP, IP_Offset, (void *)0b0000000000000101);
    EditIPField(&IP_P, IP, IP_Protocol, (void *)(intptr_t)IPv4Config.IP_TYPE_ICMP);
    IP_Compute_checksum(&IP_P);
    SendIP(IP_P);

    // 3. TESTER: Listen for ICMP Echo Reply
	Packet_filter filter;
    strcpy(filter.dstAdd, (char*)IPv4Config.TESTER_IP);
    strcpy(filter.SrcAdd, (char*)IPv4Config.DUT_IP);
    ICMP_Packet ICMP_PR = ListenICMP(filter, IPv4Config.LISTEN_TIME);

    if(ICMP_PR.length == 0)
    {
        printf("------------- IPv4_FRAGMENTS_01: Test failed -> DUT not responding -------------\n");
        return 1;
    }
    // 4. TESTER: Verify that Identifier, Sequence Number and Data of ICMP Echo Reply are same as those of ICMP Echo Request sent in two fragments.
    else if
    (
        (int)GetICMPField(&ICMP_PR, ICMP, ICMP_identifier) == (int)GetICMPField(&ICMP_P, ICMP, ICMP_identifier) 
        &&
        (int)GetICMPField(&ICMP_PR, ICMP, ICMP_sequence) == (int)GetICMPField(&ICMP_P, ICMP, ICMP_sequence)
        &&
        !strcmp((char*)GetICMPField(&ICMP_PR, PAYLAOD, PAYLOAD_data), totalPayload)
    )
    {
        printf("------------- IPv4_FRAGMENTS_01: Test passed -------------\n");
        return 0;
    }
    else
    {
        printf("------------- IPv4_FRAGMENTS_01: Test failed ------------- \n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_FRAGMENTS_02
*
***************************************************************************************************/
/**
* @brief
*  To assemble the fragments of an internet datagram, an internet protocol module (for example at a
*  destination host) combines internet datagrams that all have the same value for the four fields: 
*  identification, source, destination, and protocol.
*
* @return int 0: The test  was successful | 1: The test failed  
*
* @note
*  This test verifies that IP module does not assemble the fragments if identification is different.
*
* @warning
* -
*
***************************************************************************************************/
int IPv4_FRAGMENTS_02()
{
    char firstHalfPayload[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    char secondHalfPayload[] = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
    char totalPayload[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";

    // 1. TESTER: Construct an ICMP Echo Request. Send an IP packet containing id1
    ICMP_Packet ICMP_P = CreateICMP();
    EditICMPField(&ICMP_P, PAYLAOD, PAYLOAD_data, (void *)firstHalfPayload);
    EditICMPField(&ICMP_P, IP, IP_TotalLength, (void *)60);
    EditICMPField(&ICMP_P, IP, IP_Identification, (void *)0x0021);
    EditICMPField(&ICMP_P, IP, IP_Offset, (void *)0b0010000000000000);
    EditICMPField(&ICMP_P, ICMP, ICMP_identifier, (void *)0x9735);
    EditICMPField(&ICMP_P, ICMP, ICMP_sequence, (void *)0x0600);   
    //ICMP_Compute_Length(&ICMP_P);
 
    ICMP_Compute_checksum(&ICMP_P);
    EditICMPField(&ICMP_P, ICMP, ICMP_checksum, (void *)0x9504);
    SendICMP(ICMP_P);

    // 2. TESTER: Send an IP packet containing id2
    IP_Packet IP_P1 = CreateIP();
    EditIPField(&IP_P1, PAYLAOD, PAYLOAD_data, (void *)secondHalfPayload);
    EditIPField(&IP_P1, IP, IP_TotalLength, (void *)60);
    EditIPField(&IP_P1, IP, IP_Identification, (void *)0x0022);
    EditIPField(&IP_P1, IP, IP_Offset, (void *)0b0000000000000101);
    EditIPField(&IP_P1, IP, IP_Protocol, (void *)(intptr_t)IPv4Config.IP_TYPE_ICMP);
    IP_Compute_checksum(&IP_P1);
    SendIP(IP_P1);

	Packet_filter filter;
    strcpy(filter.dstAdd, (char*)IPv4Config.TESTER_IP);
    strcpy(filter.SrcAdd, (char*)IPv4Config.DUT_IP);
    ICMP_Packet ICMP_PR = ListenICMP(filter, IPv4Config.LISTEN_TIME);

    if(ICMP_PR.length == 0)
    {
        // 3. TESTER: Send an IP packet containing id1
        IP_Packet IP_P2 = CreateIP();
        EditIPField(&IP_P2, PAYLAOD, PAYLOAD_data, (void *)secondHalfPayload);
        EditIPField(&IP_P2, IP, IP_TotalLength, (void *)60);
        EditIPField(&IP_P2, IP, IP_Identification, (void *)0x0021);
        EditIPField(&IP_P2, IP, IP_Offset, (void *)0b0000000000000101);
        EditIPField(&IP_P2, IP, IP_Protocol, (void *)(intptr_t)IPv4Config.IP_TYPE_ICMP);
        IP_Compute_checksum(&IP_P2);
        SendIP(IP_P2);

        // 4. TESTER: Listen for ICMP Echo Reply and verify that Identifier, Sequence Number and Data of ICMP Echo Reply are same as those of ICMP Echo Request sent in two fragments.
        ICMP_Packet ICMP_PR2 = ListenICMP(filter, IPv4Config.LISTEN_TIME);
        if (
            (int)GetICMPField(&ICMP_PR2, ICMP, ICMP_identifier) == (int)GetICMPField(&ICMP_P, ICMP, ICMP_identifier) 
            &&
            (int)GetICMPField(&ICMP_PR2, ICMP, ICMP_sequence) == (int)GetICMPField(&ICMP_P, ICMP, ICMP_sequence)
            &&
            !strcmp((char*)GetICMPField(&ICMP_PR2, PAYLAOD, PAYLOAD_data), totalPayload)
        )
        {
            printf("------------- IPv4_FRAGMENTS_02: Test passed ------------- \n");
            sleep(IPv4Config.IP_INI_REASSEMBLE_TIMEOUT);
            return 0;
        }
        else 
        {
            printf("------------- IPv4_FRAGMENTS_02: Test failed -> DUT not responding ------------- \n");
            sleep(IPv4Config.IP_INI_REASSEMBLE_TIMEOUT);
            return 1;
        }
    }
    else
    {
        printf("------------- IPv4_FRAGMENTS_02: Test failed -> DUT sent a response ------------- \n");
        sleep(IPv4Config.IP_INI_REASSEMBLE_TIMEOUT);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_FRAGMENTS_03
*
***************************************************************************************************/
/**
* @brief
*  To assemble the fragments of an internet datagram, an internet protocol module (for example at a
*  destination host) combines internet datagrams that all have the same value for the four fields: 
*  identification, source, destination, and protocol.
*
* @return int 0: The test  was successful | 1: The test failed 
*
* @note
*  This test verifies that IP module does not assemble the fragments if source is different.
*
* @warning
* -
*
***************************************************************************************************/
int IPv4_FRAGMENTS_03()
{
    // 1. TESTER: Construct an ICMP Echo Request. Send an IP packet
    char firstHalfPayload[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    char secondHalfPayload[] = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
    char payload[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
    ICMP_Packet ICMP_P = CreateICMP();
    EditICMPField(&ICMP_P, PAYLAOD, PAYLOAD_data, (void *)firstHalfPayload);
    EditICMPField(&ICMP_P, IP, IP_TotalLength, (void *)60);
    EditICMPField(&ICMP_P, IP, IP_Identification, (void *)0x0023);
    EditICMPField(&ICMP_P, IP, IP_Offset, (void *)0b0010000000000000);
    EditICMPField(&ICMP_P, ICMP, ICMP_identifier, (void *)0x9735);
    EditICMPField(&ICMP_P, ICMP, ICMP_sequence, (void *)0x0600);    
    ICMP_Compute_checksum(&ICMP_P);
    EditICMPField(&ICMP_P, ICMP, ICMP_checksum, (void *)0x9504);
    SendICMP(ICMP_P);

    // 2. TESTER: Send an IP packet containg Source Address field set to different address from host
    IP_Packet IP_P1 = CreateIP();
    EditIPField(&IP_P1, IP, IP_SrcAddress, (void *)INCORRECT_SOURCE_ADDRESS);  
    EditIPField(&IP_P1, PAYLAOD, PAYLOAD_data, (void *)secondHalfPayload); 
    EditIPField(&IP_P1, IP, IP_TotalLength, (void *)60);
    EditIPField(&IP_P1, IP, IP_Identification, (void *)0x0023);
    EditIPField(&IP_P1, IP, IP_Offset, (void *)0b0000000000000101);
    EditIPField(&IP_P1, IP, IP_Protocol, (void *)(intptr_t)IPv4Config.IP_TYPE_ICMP);
    IP_Compute_checksum(&IP_P1);
    SendIP(IP_P1);

    //3. DUT: Do not send ICMP Echo Reply
	Packet_filter filter;
    strcpy(filter.dstAdd, (char*)IPv4Config.TESTER_IP);
    strcpy(filter.SrcAdd, (char*)IPv4Config.DUT_IP);
    ICMP_Packet ICMP_PR = ListenICMP(filter, IPv4Config.LISTEN_TIME);

    if(ICMP_PR.length == 0)
    {
        // 4. TESTER: Send an IP packet
        IP_Packet IP_P2 = CreateIP();
        EditIPField(&IP_P2, PAYLAOD, PAYLOAD_data, (void *)secondHalfPayload);
        EditIPField(&IP_P2, IP, IP_TotalLength, (void *)60);
        EditIPField(&IP_P2, IP, IP_Identification, (void *)0x0023);
        EditIPField(&IP_P2, IP, IP_Offset, (void *)0b0000000000000101);
        EditIPField(&IP_P2, IP, IP_Protocol, (void *)(intptr_t)IPv4Config.IP_TYPE_ICMP);
        IP_Compute_checksum(&IP_P2);
        SendIP(IP_P2);

        // 5. TESTER: Listen for ICMP Echo Reply and verify that Identifier, Sequence Number and Data of ICMP Echo Reply are same as those of ICMP Echo Request sent in two fragments.
        ICMP_Packet ICMP_PR2 = ListenICMP(filter, IPv4Config.LISTEN_TIME);
        if (ICMP_PR2.length == 0)
        {
            printf("------------- IPv4_FRAGMENTS_03: Test failed -> DUT not responding ------------- \n");
            sleep(IPv4Config.IP_INI_REASSEMBLE_TIMEOUT);
            return 1;

        }
        if (
            (int)GetICMPField(&ICMP_PR2, ICMP, ICMP_identifier) == (int)GetICMPField(&ICMP_P, ICMP, ICMP_identifier) 
            &&
            (int)GetICMPField(&ICMP_PR2, ICMP, ICMP_sequence) == (int)GetICMPField(&ICMP_P, ICMP, ICMP_sequence)
            &&
            !strcmp((char*)GetICMPField(&ICMP_PR2, PAYLAOD, PAYLOAD_data), payload)
        )
        {
            printf("------------- IPv4_FRAGMENTS_03: Test passed ------------- \n");
            sleep(IPv4Config.IP_INI_REASSEMBLE_TIMEOUT);
            return 0;
        }
        else 
        {
            printf("------------- IPv4_FRAGMENTS_03: Test failed -------------\n");
            sleep(IPv4Config.IP_INI_REASSEMBLE_TIMEOUT);
            return 1;
        }
    }
    else
    {
        printf("------------- IPv4_FRAGMENTS_03: Test failed -> DUT sent a response ------------- \n");
        sleep(IPv4Config.IP_INI_REASSEMBLE_TIMEOUT);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_FRAGMENTS_04
*
***************************************************************************************************/
/**
* @brief
*  To assemble the fragments of an internet datagram, an internet protocol module (for example at a
*  destination host) combines internet datagrams that all have the same value for the four fields: 
*  identification, source, destination, and protocol.
*
* @return int 0: The test  was successful | 1: The test failed 
*
* @note
*  This test verifies that IP module does not assemble the fragments if protocol is different.
*
* @warning
* -
*
***************************************************************************************************/
int IPv4_FRAGMENTS_04() /*Fails*/
{
    // 1. TESTER: Construct an ICMP Echo Request. Send an IP packet
    char firstHalfPayload[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    char secondHalfPayload[] = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
    char totalPayload[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
    ICMP_Packet ICMP_P = CreateICMP();
    EditICMPField(&ICMP_P, PAYLAOD, PAYLOAD_data, (void *)firstHalfPayload);
    EditICMPField(&ICMP_P, IP, IP_TotalLength, (void *)60);
    EditICMPField(&ICMP_P, IP, IP_Identification, (void *)0x0024);
    EditICMPField(&ICMP_P, IP, IP_Offset, (void *)0b0010000000000000);
    ICMP_Compute_checksum(&ICMP_P);
    EditICMPField(&ICMP_P, ICMP, ICMP_checksum, (void *)0x027e);
    SendICMP(ICMP_P);

    // 2. TESTER: Send an IP packet containing TCP protocol type
    IP_Packet IP_P1 = CreateIP();
    EditIPField(&IP_P1, PAYLAOD, PAYLOAD_data, (void *)secondHalfPayload); 
    EditIPField(&IP_P1, IP, IP_TotalLength, (void *)60);
    EditIPField(&IP_P1, IP, IP_Identification, (void *)0x0024);
    EditIPField(&IP_P1, IP, IP_Offset, (void *)0b0000000000000101);
    EditIPField(&IP_P1, IP, IP_Protocol, (void *)(intptr_t)IPv4Config.IP_TYPE_TCP);
    IP_Compute_checksum(&IP_P1);
    SendIP(IP_P1);

    // 3. DUT: Do not send ICMP Echo Reply
	Packet_filter filter;
    strcpy(filter.dstAdd, (char*)IPv4Config.TESTER_IP);
    strcpy(filter.SrcAdd, (char*)IPv4Config.DUT_IP);
    ICMP_Packet ICMP_PR = ListenICMP(filter, IPv4Config.LISTEN_TIME);

    if(ICMP_PR.length == 0)
    {
        // 4. TESTER: Send an IP packet
        IP_Packet IP_P2 = CreateIP();
        EditIPField(&IP_P2, PAYLAOD, PAYLOAD_data, (void *)secondHalfPayload);
        EditIPField(&IP_P2, IP, IP_TotalLength, (void *)60);
        EditIPField(&IP_P2, IP, IP_Identification, (void *)0x0024);
        EditIPField(&IP_P2, IP, IP_Offset, (void *)0b0000000000000101);
        EditIPField(&IP_P2, IP, IP_Protocol, (void *)(intptr_t)IPv4Config.IP_TYPE_ICMP);
        IP_Compute_checksum(&IP_P2);
        SendIP(IP_P2);

        // 5. TESTER: Listen for ICMP Echo Reply and verify that Identifier, Sequence Number and Data of ICMP Echo Reply are same as those of ICMP Echo Request sent in two fragments.
        ICMP_Packet ICMP_PR2 = ListenICMP(filter, IPv4Config.LISTEN_TIME);
        if (
            (int)GetICMPField(&ICMP_PR2, ICMP, ICMP_identifier) == (int)GetICMPField(&ICMP_P, ICMP, ICMP_identifier) 
            &&
            (int)GetICMPField(&ICMP_PR2, ICMP, ICMP_sequence) == (int)GetICMPField(&ICMP_P, ICMP, ICMP_sequence)
            &&
            !strcmp((char*)GetICMPField(&ICMP_PR2, PAYLAOD, PAYLOAD_data), totalPayload)
        )
        {
            printf("------------- IPv4_FRAGMENTS_04: Test passed -------------\n");
            sleep(IPv4Config.IP_INI_REASSEMBLE_TIMEOUT);
            return 0;
        }
        else 
        {
            printf("------------- IPv4_FRAGMENTS_04: Test failed -> DUT not responding -------------\n");
            sleep(IPv4Config.IP_INI_REASSEMBLE_TIMEOUT);
            return 1;
        }
    }
    else
    {
        printf("------------- IPv4_FRAGMENTS_04: Test failed -> DUT sent a response -------------\n");
        sleep(IPv4Config.IP_INI_REASSEMBLE_TIMEOUT);
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_FRAGMENTS_05
*
***************************************************************************************************/
/**
* @brief
*  The fragmentation strategy is designed so than an unfragmented datagram has all zero fragmentation
*  information (MF = 0, fragment offset = 0).
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
int IPv4_FRAGMENTS_05()
{
    /* 1. TESTER: Cause DUT to send a Message with <UDPDefaultData> from <DIface-0> with - Source IP Address set to <DIface-0-IP> - 
    Destination IP Address set to <HOST-1-IP> - Source UDP Port field set to <unusedUDPDstPort1> (20001) - Destination UDP Port 
    field set to <unusedUDPSrcPort> (20000) */
	/*ip4addr ipv4Dut, ipv4Tester;
    uint16 unusedUDPDstPort1 = 20001;
	uint16 unusedUDPSrcPort = 20000;
	ipv4Dut.dataLength = 4;
    stoiIP(IPv4Config.DUT_IP, ipv4Dut.Data);
	ipv4Tester.dataLength = 4;
	stoiIP(IPv4Config.TESTER_IP, ipv4Tester.Data);

    vint8 Data;
	uint8 data[] = "From DUT to Tester";
	Data.dataLength = sizeof(data);
	Data.Data = (uint8*) malloc(Data.dataLength);
	strcpy(Data.Data, data);

	//Open control channel
	TP_OpenControlChannel(ipv4Dut, 56000);
	printf("\n[+]:Control channel was opened\n");
	
	//Start test
	TP_GenaralStartTest(GenaralStartTest_cb);
	printf("\n[+]:Test was started\n");
	
	//Create socket and bind it
	while(OK!=0){ sleep(1);}
	TP_UdpCreateAndBind(UdpCreateAndBind_cb,TRUE,unusedUDPDstPort1,ipv4Dut);
	printf("\n[+]:socket was successfully created and bound\n");
	
	//DUT sending data
	sleep(1);
	printf("\n[+]:%d",OK);
	TP_UdpSendData(TP_UdpSendData_cb,socketId,0,unusedUDPSrcPort,ipv4Tester,data);
	printf("\n[+]:DUT send data\n");
	
	// 2. TESTER: wait for a udp message 
    Packet_filter filter;
    filter.Srcport = unusedUDPDstPort1;
	strcpy(filter.SrcAdd, (char*)IPv4Config.DUT_IP);
	filter.Dstport = unusedUDPSrcPort;
    strcpy(filter.SrcAdd, (char*)IPv4Config.TESTER_IP);

    UDP_Packet UDP_R;
	UDP_R = ListenUDP(filter, IPv4Config.LISTEN_TIME);

    // 4. TESTER: Verify that the received packet contains IP Flags field = 0
	if(GetUDPField(&UDP_R, IP, IP_Offset) == 0){
		printf("\n[+]:Packet received \n");
		printf("\n[+]:Destination port equal to %ld:\n", GetUDPField(&UDP_R,UDP,UDP_SrcPort));
		
		//Close socket
		while(OK!=0){ sleep(1);}
		TP_UdpCloseSocket(UdpCloseSocket_cb, socketId);
		printf("\n[+]:socket %d was closed \n", socketId);
		//End Test
		sleep(1);
		TP_GenaralEndTest(GenaralEndTest_cb, 0, (text){0});
		printf("\n[+]:Test case 4 was successfully ended\n");
		//close Control channel
		TP_CloseControlChannel();
	}
	return 0;*/
    return 2;
}
