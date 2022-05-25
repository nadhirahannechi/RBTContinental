
#include <stdio.h>
#include <unistd.h>

#include "AbstractionAPI.h"
#include "TestabilityProtocol_Intern.h"
#include "TestabilityProtocol_api.h"

#include "UDP_DatagramLength.h"
#include "UDP_config.h"
#include "split.h"

extern UDP_config_t UDPConfiguration;

uint8 callback_Result_Datagram;
uint16 socketId_Datagram, socketid_Datagram;
uint16 Len_Datagram, Srcport_Datagram;
ip4addr ipv4Addr_Datagram;
vint8 UDP_Data_Datagram;


void GenaralStartTest_cb_Datagram(TP_ResultID_t Result) {
    callback_Result_Datagram = Result;
}

void UdpCreateAndBind_cb_Datagram(TP_ResultID_t Result, uint16 s) {
    callback_Result_Datagram = Result;
    socketId_Datagram = s;
}

void TP_UdpReceiveAndForward_cb_Datagram(TP_ResultID_t Result, uint16 s) {
    callback_Result_Datagram = Result;
    socketid_Datagram = s;
}

void TP_Udp_ReceiveAndForward_even_cb_Datagram(TP_ResultID_t Result, uint16 Length, uint16 Port, ip4addr Address,
                                               vint8 DataUdp) {
    callback_Result_Datagram = Result;
    Len_Datagram = Length;
    Srcport_Datagram = Port;
    ipv4Addr_Datagram = Address;
    UDP_Data_Datagram = DataUdp;
}

void UdpCloseSocket_cb_Datagram(TP_ResultID_t Result) {
    callback_Result_Datagram = Result;
}

void GenaralEndTest_cb_Datagram(TP_ResultID_t Result) {
    callback_Result_Datagram = Result;
}


int UDP_DatagramLength_01() {
    uint16 test_Result = 1;
    callback_Result_Datagram = 2;
    ip4addr ipv4DUT;

    ipv4DUT.dataLength = 4;
    extractIpAddress(UDPConfiguration.Dst_IP_addr, &ipv4DUT);

    UDP_Packet UDP_S;

    vint8 Casename;

    uint8 Case[] = "UDP_DatagramLength_01: To verify that IUT discards a truncated UDP datagram.";
    Casename.dataLength = sizeof(Case);
    Casename.Data = (uint8 *) calloc(Casename.dataLength, sizeof(uint8));
    memcpy(Casename.Data, Case, Casename.dataLength);


    //start the test test
    TP_GenaralStartTest(GenaralStartTest_cb_Datagram);
    while (callback_Result_Datagram != 0) { sleep(1); }
    printf("Test UDP_DatagramLength was started\n");
    callback_Result_Datagram = 2;

    //create and bind socket
    TP_UdpCreateAndBind(UdpCreateAndBind_cb_Datagram, TRUE, UDPConfiguration.DUTUDPPort, ipv4DUT);
    while (callback_Result_Datagram != 0) { sleep(1); }
    printf("\n[+]:Socket was successfully created and bound\n");
    callback_Result_Datagram = 2;

    //Creation an UDP UDP_S
    UDP_S = CreateUDP();
    if (UDP_S.length == 0) {
        printf("\n[+]:UDP creation error\n");
        TP_GenaralEndTest(GenaralEndTest_cb_Datagram, 1, Casename);
        printf("\n[+]:Test case UDP_DatagramLength_01 end with error n°1");
    } else {
        printf("\n[+]:Udp packet was successfully created\n");

        //Set payload data to default data
        EditUDPField(&UDP_S, PAYLAOD, PAYLOAD_data, (void *) UDPConfiguration.UDPDefaultData);

        //set Payload length
        EditUDPField(&UDP_S, PAYLAOD, PAYLOAD_length, (void *) UDPConfiguration.UDPDefaultDataLen);

        //Set UDP Header length
        EditUDPField(&UDP_S, UDP, UDP_Length, (void *) 7);

        //Set UPD Src port to tester port
        EditUDPField(&UDP_S, UDP, UDP_SrcPort, (void *) UDPConfiguration.testerUDPPort);

        //Set UDP Dst port to DUT port
        EditUDPField(&UDP_S, UDP, UDP_DstPort, (void *) UDPConfiguration.DUTUDPPort);

        //Compute the packet checksums
        UDP_Compute_checksum(&UDP_S);

        //Send the UDP data to DUT
        callback_Result_Datagram = 2;
        //receive and forward
        TP_UdpReceiveAndForward(TP_UdpReceiveAndForward_cb_Datagram, TP_Udp_ReceiveAndForward_even_cb_Datagram,
                                socketId_Datagram, 8, UDPConfiguration.UDPDefaultDataLen);
        while (callback_Result_Datagram != 0) { sleep(1); }
        printf("\n[+]:DUT is ready to receive and forward data\n");

        //Send Data to DUT
        if (SendUDP(UDP_S) != 1) {
            printf("\n[+]:Error sending \n");
            TP_GenaralEndTest(GenaralEndTest_cb_Datagram, 1, Casename);
            printf("\n[+]:Test case UDP_DatagramLength_01 end with error n°3");
            test_Result = 1;
        } else {
            printf("\n[+]:Packet sent...!\n");

            //close socket
            sleep(1);
            TP_UdpCloseSocket(UdpCloseSocket_cb_Datagram, socketId_Datagram);
            while (callback_Result_Datagram != 0) { sleep(1); }
            printf("\n[+]:Socket id %d closed\n", socketId_Datagram);

            //end Test
            TP_GenaralEndTest(GenaralEndTest_cb_Datagram, 1, Casename);
            test_Result = 0;
            printf("\n**************[+]:Test case UDP_DatagramLength_01 was successfully_01 end***************\n");

        }
    }


return test_Result;
}
