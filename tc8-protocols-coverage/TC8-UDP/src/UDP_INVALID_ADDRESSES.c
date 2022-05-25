#include <stdio.h>
#include <unistd.h>

#include "AbstractionAPI.h"
#include "TestabilityProtocol_api.h"
#include "TestabilityProtocol_Intern.h"

#include "UDP_INVALID_ADDRESSES.h"
#include "split.h"
#include "UDP_config.h"

extern UDP_config_t UDPConfiguration;

uint8 callback_Result_InvalidAddr;
uint16 socketId_InvalidAddr, socketid_Invalid;
uint16 Len_Invalid, Srcport_Invalid;
ip4addr ipv4Addr_Invalid;
vint8 UDP_Data_Invalid;

void GenaralStartTest_cb_InvalidAddr(TP_ResultID_t Result) {
    callback_Result_InvalidAddr = Result;
}

void UdpCreateAndBind_cb_InvalidAddr(TP_ResultID_t Result, uint16 s) {
    callback_Result_InvalidAddr = Result;
    socketId_InvalidAddr = s;
}

void TP_UdpReceiveAndForward_cb_InvalidAddr(TP_ResultID_t Result, uint16 s) {
    callback_Result_InvalidAddr = Result;
    socketid_Invalid = s;
}

void TP_Udp_ReceiveAndForward_even_cb_InvalidAddr(TP_ResultID_t Result, uint16 Length, uint16 Port, ip4addr Address,
                                                  vint8 DataUdp) {
    callback_Result_InvalidAddr = Result;
    Len_Invalid = Length;
    Srcport_Invalid = Port;
    ipv4Addr_Invalid = Address;
    UDP_Data_Invalid = DataUdp;
    printf("\n[(+]:******************* Len = %lu", Len_Invalid);
}

void TP_UdpSendData_cb_InvalidAddr(TP_ResultID_t Result) {
    callback_Result_InvalidAddr = Result;
}

void UdpCloseSocket_cb_InvalidAddr(TP_ResultID_t Result) {
    callback_Result_InvalidAddr = Result;
}

void GenaralEndTest_cb_InvalidAddr(TP_ResultID_t Result) {
    callback_Result_InvalidAddr = Result;
}


int UDP_INVALID_ADDRESSES_01() {
    callback_Result_InvalidAddr = 2;
    uint8_t test_Result = 1;

    ip4addr ipv4DUT;
    ipv4DUT.dataLength = 4;
    extractIpAddress(UDPConfiguration.Dst_IP_addr, &ipv4DUT);
    UDP_Packet UDP_S;

    vint8 Casename;


    uint8 Case[] = "UDP_INVALID_ADDRESSES_01: Invalid Addresses - multicast source address";
    Casename.dataLength = sizeof(Case);
    Casename.Data = (uint8 *) calloc(Casename.dataLength, sizeof(uint8));
    strcpy(Casename.Data, Case);

    //start the Test
    TP_GenaralStartTest(GenaralStartTest_cb_InvalidAddr);
    while (callback_Result_InvalidAddr != 0) { sleep(1); }
    callback_Result_InvalidAddr = 2;
    printf(" \n[+]:Test UDP_INVALID_ADDRESSES_01 was started\n");

    //Create and bind the socket
    TP_UdpCreateAndBind(UdpCreateAndBind_cb_InvalidAddr, TRUE, UDPConfiguration.unusedUDPDstPort1, ipv4DUT);
    while (callback_Result_InvalidAddr != 0) { sleep(1); }
    callback_Result_InvalidAddr = 2;
    printf(" \n[+]:Socket was successfully created and bound \n");


    //Create the udp packet
    UDP_S = CreateUDP();
    if (UDP_S.length == 0) {
        printf("\n[+]: UDP packet creation error\n");
        TP_GenaralEndTest(GenaralEndTest_cb_InvalidAddr, 1, Casename);
        printf("\n[+]: Test case UDP_INVALID_ADDRESSES_01 end with error n°1\n");
        test_Result = 1;
    } else {
        printf("\n[+]: Udp packet successfully created\n");

        //set Payload data
        EditUDPField(&UDP_S, PAYLAOD, PAYLOAD_data, (void *) UDPConfiguration.UDPDefaultData);

        //set payload length
        EditUDPField(&UDP_S, PAYLAOD, PAYLOAD_length, (void *) UDPConfiguration.UDPDefaultDataLen);

        //set UDP Header length
        EditUDPField(&UDP_S, UDP, UDP_Length, (void *) (UDPConfiguration.UDPDefaultDataLen + 8));

        //Set UPD Src port
        EditUDPField(&UDP_S, UDP, UDP_SrcPort, (void *) UDPConfiguration.testerUDPPort);

        //Set UDP Dst port
        EditUDPField(&UDP_S, UDP, UDP_DstPort, (void *) UDPConfiguration.unusedUDPDstPort1);

        //set Dst ip @
        EditUDPField(&UDP_S, IP, IP_DstAddress, (void *) UDPConfiguration.allSystemMCastAddr);

        //Compute packet checksums
        UDP_Compute_checksum(&UDP_S);

        //Push DUT to listen and receive the data
        TP_UdpReceiveAndForward(TP_UdpReceiveAndForward_cb_InvalidAddr, TP_Udp_ReceiveAndForward_even_cb_InvalidAddr,socketId_InvalidAddr, 8, UDPConfiguration.UDPDefaultDataLen);
        while (callback_Result_InvalidAddr != 0) { sleep(1); }
        callback_Result_InvalidAddr = 2;
        printf("\n[+]: DUT is ready to receive and forward data\n");

        //Send the UDP packet
        if (SendUDP(UDP_S) ==-1) {
            printf("\n[+]: Error to send the packet\n");
            TP_GenaralEndTest(GenaralEndTest_cb_InvalidAddr, 1, Casename);
            printf("\n[+]: Test case UDP_INVALID_ADDRESSES_01 end with error n°4\n");
            test_Result = 1;

        } else {
            printf("\n[+]: Packet delivered!\n");

            //close socket
            sleep(1);
            TP_UdpCloseSocket(UdpCloseSocket_cb_InvalidAddr, socketId_InvalidAddr);
            while (callback_Result_InvalidAddr != 0) { sleep(1); }
            callback_Result_InvalidAddr = 2;
            printf("\n[+]: Socket id %d closed \n", socketId_InvalidAddr);

            //End the test
            TP_GenaralEndTest(GenaralEndTest_cb_InvalidAddr, 1, Casename);
            test_Result = 0;
            printf("\n***********[+]: Test case UDP_INVALID_ADDRESSES_01 was successfully ended***********\n");

        }
    }


return test_Result;
}

int UDP_INVALID_ADDRESSES_02() {
    uint8_t test_Result = 1;
    callback_Result_InvalidAddr = 2;
    ip4addr ipv4DUT;
    ipv4DUT.dataLength = 4;
    extractIpAddress(UDPConfiguration.Dst_IP_addr, &ipv4DUT);
    UDP_Packet UDP_S;
    vint8 Casename;

    uint8 Case[] = "UDP_INVALID_ADDRESSES_02: Invalid Addresses - broadcast source address";
    Casename.dataLength = sizeof(Case);
    Casename.Data = (uint8 *) calloc(Casename.dataLength, sizeof(uint8));
    memcpy(Casename.Data, Case, Casename.dataLength);

    //start the Test
    TP_GenaralStartTest(GenaralStartTest_cb_InvalidAddr);
    while (callback_Result_InvalidAddr != 0) { sleep(1); }
    callback_Result_InvalidAddr = 2;
    printf("\n[+]:Test UDP_INVALID_ADDRESSES_02 was started\n");

    //Create and bind the socket
    TP_UdpCreateAndBind(UdpCreateAndBind_cb_InvalidAddr, TRUE, UDPConfiguration.unusedUDPDstPort1, ipv4DUT);
    while (callback_Result_InvalidAddr == 2) { sleep(1); }
    callback_Result_InvalidAddr = 2;
    printf("\n[+]:Socket was successfully created and bound\n");


    //Create the udp packet
    UDP_S = CreateUDP();

    //Verify that creation of packet well done
    if (UDP_S.length == 0) {
        printf("\n[+]:UDP packet creation error\n");
        TP_GenaralEndTest(GenaralEndTest_cb_InvalidAddr, 02, Casename);
        printf("\n[+]:Test case UDP_INVALID_ADDRESS_02 end with error n°1\n");
    } else {
        printf("\n[+]:Udp packet successfully created\n");

        //Set payload data
        EditUDPField(&UDP_S, PAYLAOD, PAYLOAD_data, (void *) UDPConfiguration.UDPDefaultData);

        //set payload length
        EditUDPField(&UDP_S, PAYLAOD, PAYLOAD_length, (void *) UDPConfiguration.UDPDefaultDataLen);

        //set UDP Src port
        EditUDPField(&UDP_S, UDP, UDP_SrcPort, (void *) UDPConfiguration.testerUDPPort);

        //set UDP Dst port
        EditUDPField(&UDP_S, UDP, UDP_DstPort, (void *) UDPConfiguration.unusedUDPDstPort1);

        //Set UDP Header length
        EditUDPField(&UDP_S, UDP, UDP_Length, (void *) (UDPConfiguration.UDPDefaultDataLen + 8));

        //Set Dst IP @ to broadcast address
        EditUDPField(&UDP_S, IP, IP_DstAddress, (void *) UDPConfiguration.AIface_0_BcastIP);

        // compute packet checksums
        if (UDP_Compute_checksum(&UDP_S) == -1) {
            printf("\n[+]:Error to Edit ip and upd checksum");
            TP_GenaralEndTest(GenaralEndTest_cb_InvalidAddr, 2, Casename);
            test_Result = 1;

        } else {
            printf("\n[+]: ip and udp checksum was successfully set\n");

            //push DUT to listen and receive data
            TP_UdpReceiveAndForward(TP_UdpReceiveAndForward_cb_InvalidAddr,TP_Udp_ReceiveAndForward_even_cb_InvalidAddr, socketId_InvalidAddr, 8,UDPConfiguration.UDPDefaultDataLen);
            while (callback_Result_InvalidAddr == 2) { sleep(1); }
            callback_Result_InvalidAddr = 2;
            printf("\n[+]:DUT is ready to receive and forward data\n");

            //Send UDP data to DUT
            if (SendUDP(UDP_S) ==-1) {
                printf("\n[+]:Error to send the packet\n");
                TP_GenaralEndTest(GenaralEndTest_cb_InvalidAddr, 2, Casename);
                printf("\n[+]:Test case UDP_INVALID_ADDRESSES_02 end with error n°4\n");
                test_Result = 1;

            } else {
                printf("\n[+]:Packet delivered!\n");

                //close socket
                sleep(1);
                TP_UdpCloseSocket(UdpCloseSocket_cb_InvalidAddr, socketId_InvalidAddr);
                while (callback_Result_InvalidAddr == 2) { sleep(1); }
                callback_Result_InvalidAddr = 2;
                printf("\n[+]:Socket id %d closed \n", socketId_InvalidAddr);

                //End the test
                TP_GenaralEndTest(GenaralEndTest_cb_InvalidAddr, 2, Casename);
                test_Result = 0;
                printf("\n************[+]:Test case UDP_INVALID_ADDRESSES_02 was successfully ended***************\n");

            }
        }
    }


return test_Result;
}
