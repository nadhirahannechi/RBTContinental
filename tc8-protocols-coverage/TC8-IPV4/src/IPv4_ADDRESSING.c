#include "AbstractionAPI.h"
#include "TestabilityProtocol_api.h"
#include "TestabilityProtocol_Intern.h"
#include "TestabilityProtocol_cbTypes.h"
#include "TestabilityProtocolTypes.h"
#include "IPv4_ADDRESSING.h"
#include "IPv4_Common.h"
#include "IPv4config.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_ADDRESSING_01
*
***************************************************************************************************/
/**
* @brief
*  Ensure that when the DUT receives an IPv4 packet containing an IPv4 Header containing a 
*  Destination Address indicating a value of Limited Broadcast, then the DUT accepts the IPv4 Packet.
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
int IPv4_ADDRESSING_01()
{
	/*OK=1;
	ip4addr ipv4DUT;
	ipv4DUT.dataLength = 4;
	ipv4DUT.Data[0] = 192;
	ipv4DUT.Data[1] = 168;
	ipv4DUT.Data[2] = 20;
	ipv4DUT.Data[3] = 243;
	uint16 localPort = 50001;
	char Case[] = "IPv4_ADDRESSING_01";
	vint8 CaseName;
	CaseName.dataLength = sizeof(Case);
	CaseName.Data = (uint8*) malloc(CaseName.dataLength);
	strcpy(CaseName.Data, Case);

    UDP_Packet UDP_P=CreateUDP();
    EditUDPField(&UDP_P, UDP,UDP_SrcPort, (void*)50000);
    EditUDPField(&UDP_P, UDP,UDP_DstPort, (void*)50001);
    EditUDPField(&UDP_P, UDP,UDP_Length, (void*)16);
    EditUDPField(&UDP_P, IP,IP_DstAddress, (void*)IPv4Config.LIMITED_BROADCAST_ADDRESS);
	UDP_Compute_checksum(&UDP_P);
	
	//Open control channel
	TP_OpenControlChannel(ipv4DUT, 56000);
	printf("control channel was opened\n");

	//Start test
	TP_GenaralStartTest(GenaralStartTest_cb);
	printf("Test was started\n");

	//Create and bind socket
	while(OK != 0)
	{
		sleep(1);
	}
	OK=1;
	TP_UdpCreateAndBind(UdpCreateAndBind_cb,TRUE, localPort, ipv4DUT);
	printf("Socket was successfully created and bound\n");

	//Receive and forward
	while(OK != 0)
	{
		sleep(1);
	}
	OK=1;
	TP_UdpReceiveAndForward(TP_UdpReceiveAndForward_cb,TP_Udp_ReceiveAndForward_even_cb, socketId, 8, UDP_P.length);
	printf("DUT is ready to receive and forward data\n");

	while(OK != 0)
	{
		sleep(1);
	}
	OK=1;

    int res = SendUDP(UDP_P);
	sleep(3);
	printf("DATA -> %s\n", UDP_Data.Data);
    if (UDP_P.length == 0 ){
		printf("UDP creation error\n");
		while(OK != 0)
		{
			sleep(1);
		}
		TP_GenaralEndTest(GenaralEndTest_cb,2, CaseName);
		printf("Test case 2 end with error n°1");
		return 1;
	}
	else {
			//Send an UDP UDP_S
			printf("Udp packet was successfully created\n");
			if (res == -1)
			{
				printf("Error sending \n");
				while (OK != 0) {sleep(1);}
				TP_GenaralEndTest(GenaralEndTest_cb, 2, CaseName);
				printf("Test case 2 end with error n°3");
				return 1;
			}
			else
			{
				printf("Packet sent...!\n");
				//close socket
				while (OK != 0) { sleep(1); }
				TP_UdpCloseSocket(UdpCloseSocket_cb, socketId);
				printf("Socket id %d closed\n", socketId);
				//end Test
				while (OK != 0) { sleep(1); }
				TP_GenaralEndTest(GenaralEndTest_cb, 2, CaseName);
				printf("Test case 2 was successfully end \n");
				//close Control channel
				TP_CloseControlChannel();
				printf("control channel closed\n");
				return 0;
			}
	}*/
	return 2;
}

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_ADDRESSING_02
*
***************************************************************************************************/
/**
* @brief
*  Ensure that when the DUT receives an IPv4 packet containing an IPv4 Header containing a 
*  Destination Address indicating a value of Directed Broadcast, then the DUT discards the IPv4 
*  Packet silently.
*
* @return 
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int IPv4_ADDRESSING_02()
{	
	/*OK=1;
	ip4addr ipv4DUT;
	ipv4DUT.dataLength = 4;
	ipv4DUT.Data[0] = 192;
	ipv4DUT.Data[1] = 168;
	ipv4DUT.Data[2] = 20;
	ipv4DUT.Data[3] = 243;
	uint16 localPort = 50001;
	char Case[] = "IPv4_ADDRESSING_02";
	vint8 CaseName;
	CaseName.dataLength = sizeof(Case);
	CaseName.Data = (uint8*) malloc(CaseName.dataLength);
	strcpy(CaseName.Data,Case);
	
	// 1. TESTER: Send a UDP Message with destination address being <directedBroadcastAddress>
    UDP_Packet UDP_P = CreateUDP();
    EditUDPField(&UDP_P, UDP,UDP_SrcPort, (void*)50000);
    EditUDPField(&UDP_P, UDP,UDP_DstPort, (void*)50001);
    EditUDPField(&UDP_P, UDP,UDP_Length, (void*)16);
    EditUDPField(&UDP_P, IP,IP_DstAddress, (void*)IPv4Config.DIRECTED_BROADCAST_ADDRESS);
    UDP_Compute_checksum(&UDP_P);
    SendUDP(UDP_P);

	// 2. TESTER: Verify using Upper Tester that DUT did not receive the UDP Message
	//open control channel
	TP_OpenControlChannel(ipv4DUT, 56000);
	printf("control channel was opened\n");
	//start test
	TP_GenaralStartTest(GenaralStartTest_cb);
	printf("Test was started\n");
	//create and bind socket
	while(OK!=0)
	{
		sleep(1);
	}
	TP_UdpCreateAndBind(UdpCreateAndBind_cb, TRUE, localPort, ipv4DUT);
	printf("Socket was successfully created and bound\n");
	
	//receive and forward
	while(OK!=0){sleep(1);}
	TP_UdpReceiveAndForward(TP_UdpReceiveAndForward_cb,TP_Udp_ReceiveAndForward_even_cb, socketId, 8, UDP_P.length);
	printf("DUT is ready to receive and forward data\n");
    int res = SendUDP(UDP_P);
	sleep(3);*/
	return 2;
}

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_ADDRESSING_03
*
***************************************************************************************************/
/**
* @brief
*  Ensure that when the DUT receives an IPv4 packet containing an IPv4 Header containing a 
*  Destination Address indicating a value of Loop Back, then the DUT discards the IPv4 packet 
*  silently.
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
int IPv4_ADDRESSING_03()
{
	// 1. TESTER: Send an ICMPv4 Echo Request with destination address being <loopBackAddress>
    ICMP_Packet ICMP_P = CreateICMP();
    EditICMPField(&ICMP_P, IP, IP_DstAddress, (void *)LOOPBACK_ADDRESS);
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
        printf("IPv4_ADDRESSING_03: Test passed\n");
		return 0;
    }
    else
    {
        printf("IPv4_ADDRESSING_03: Test failed -> DUT sent a response\n");
		return 1;
    }
}
