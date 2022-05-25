#include <stdio.h>
#include <unistd.h>


//APIs and header
#include "AbstractionAPI.h"
#include "TestabilityProtocol_api.h"
#include "TestabilityProtocol_Intern.h"

#include "UDP_INTRODUCTION.h"
#include "split.h"
#include "UDP_config.h"

extern UDP_config_t UDPConfiguration;

uint8 callback_Result_Introduction;
uint16 socketId_Intro, socketid_Intro;
uint16 Len_Intro, Srcport_Intro;
ip4addr ipv4Addr_Intro;
vint8 UDP_Data_Intro;


void GenaralStartTest_cb_Introduction(TP_ResultID_t Result) {
    callback_Result_Introduction = Result;
}

void UdpCreateAndBind_cb_Introduction(TP_ResultID_t Result, uint16 s) {
    callback_Result_Introduction = Result;
    socketId_Intro = s;
}

void TP_UdpReceiveAndForward_cb_Introduction(TP_ResultID_t Result, uint16 s) {
    callback_Result_Introduction = Result;
    socketid_Intro = s;
}

void TP_Udp_ReceiveAndForward_even_cb_Introduction(TP_ResultID_t Result, uint16 Length, uint16 Port, ip4addr Address,
                                                   vint8 DataUdp) {
    callback_Result_Introduction = Result;
    Len_Intro = Length;
    Srcport_Intro = Port;
    ipv4Addr_Intro = Address;
    UDP_Data_Intro = DataUdp;
}

void UdpCloseSocket_cb_Introduction(TP_ResultID_t Result) {
    callback_Result_Introduction = Result;
}

void GenaralEndTest_cb_Introduction(TP_ResultID_t Result) {
    callback_Result_Introduction = Result;
}


int UDP_INTRODUCTION_01() {
    uint8_t test_Result = 1;
    callback_Result_Introduction = 2;
    ip4addr ipv4DUT;
    ipv4DUT.dataLength = 4;
    extractIpAddress(UDPConfiguration.Dst_IP_addr, &ipv4DUT);

    UDP_Packet UDP_S;

    vint8 Casename;

    uint8 Case[] = "UDP_INTRODUCTION_01: Introduction – Broadcast Destination Address";
    Casename.dataLength = sizeof(Case);
    Casename.Data = (uint8 *) calloc(Casename.dataLength, sizeof(uint8));
    memcpy(Casename.Data, Case, Casename.dataLength);


    //start the Test
    TP_GenaralStartTest(GenaralStartTest_cb_Introduction);
    while (callback_Result_Introduction != 0) { sleep(1); }
    callback_Result_Introduction = 2;
    printf("\n[+]:Test UDP_INTRODUCTION_01 was started\n");

    //Create and bind socket
    TP_UdpCreateAndBind(UdpCreateAndBind_cb_Introduction, TRUE, UDPConfiguration.unusedUDPDstPort1, ipv4DUT);
    while (callback_Result_Introduction != 0) { sleep(1); }
    callback_Result_Introduction = 2;
    printf("\n[+]:Socket was successfully created and bound\n");

    //Create the udp packet a
    UDP_S = CreateUDP();
    if (UDP_S.length == 0) {//verify that packet was created
        printf("\n[+]:UDP packet creation error\n");
        TP_GenaralEndTest(GenaralEndTest_cb_Introduction, 27, Casename);
        printf("\n[+]:Test case UDP_INTRODUCTION_01 end with error n°1\n");
    } else {
        printf("\n[+]Udp packet successfully created\n");

        //Set Payload data to default data
        EditUDPField(&UDP_S, PAYLAOD, PAYLOAD_length, (void *) (UDPConfiguration.UDPDefaultDataLen));

        //set payload length
        EditUDPField(&UDP_S, UDP, UDP_Length, (void *) (UDPConfiguration.UDPDefaultDataLen + 8));

        //set UDP Dst port to unusedUDPDst port1
        EditUDPField(&UDP_S, UDP, UDP_DstPort, (void *) UDPConfiguration.unusedUDPDstPort1);

        //set UDP Src port to tester port
        EditUDPField(&UDP_S, UDP, UDP_SrcPort, (void *) UDPConfiguration.testerUDPPort);

        //set Dest IP @ to broadcast @
        EditUDPField(&UDP_S, IP, IP_DstAddress, (void *) UDPConfiguration.AIface_0_BcastIP);

        //compute packet checksums
        UDP_Compute_checksum(&UDP_S);

        //push DUT to receive data
        TP_UdpReceiveAndForward(TP_UdpReceiveAndForward_cb_Introduction,TP_Udp_ReceiveAndForward_even_cb_Introduction, socketId_Intro, 8,UDPConfiguration.UDPDefaultDataLen);
        while (callback_Result_Introduction != 0) { sleep(1); }
        callback_Result_Introduction = 2;
        printf("\n[+]:DUT is now ready to receive data\n");

        //send data to dut
        if (SendUDP(UDP_S) ==-1) {
            printf("\n[+]:Error to send the packet\n");
            TP_GenaralEndTest(GenaralEndTest_cb_Introduction, 1, Casename);
            printf("\n[+]:Test case UDP_INTRODUCTION_01 end with error n°4\n");
            test_Result = 1;
        } else {
            printf("\n[+]:Packet delivered!\n");

            //close socket
            sleep(1);
            TP_UdpCloseSocket(UdpCloseSocket_cb_Introduction, socketId_Intro);
            while (callback_Result_Introduction != 0) { sleep(1); }
            callback_Result_Introduction = 2;
            printf("\n[+]:Socket id %d closed \n", socketId_Intro);

            //End the test
            TP_GenaralEndTest(GenaralEndTest_cb_Introduction, 1, Casename);
            test_Result = 0;
            printf("\n**********[+]:Test case UDP_INTRODUCTION_01 was successfully ended**********\n");

        }
    }


return test_Result;
}

int UDP_INTRODUCTION_02() {
    callback_Result_Introduction = 2;
    uint8_t test_Result = 1;
    ip4addr ipv4DUT;
    ipv4DUT.dataLength = 4;
    extractIpAddress(UDPConfiguration.Dst_IP_addr, &ipv4DUT);

    UDP_Packet UDP_S;

    vint8 Casename;
    uint8 Case[] = "UDP_INTRODUCTION_02: Introduction – Multicast Destination Address";
    Casename.dataLength = sizeof(Case);
    Casename.Data = (uint8 *) calloc(Casename.dataLength, sizeof(uint8));
    memcpy(Casename.Data, Case, Casename.dataLength);


    //start the Test
    TP_GenaralStartTest(GenaralStartTest_cb_Introduction);
    while (callback_Result_Introduction != 0) { sleep(1); }
    callback_Result_Introduction = 2;
    printf("\n[+]:Test UDP_INTRODUCTION_02 was started\n");

    //Create and bind socket
    TP_UdpCreateAndBind(UdpCreateAndBind_cb_Introduction, TRUE, UDPConfiguration.unusedUDPDstPort1, ipv4DUT);
    while (callback_Result_Introduction != 0) { sleep(1); }
    callback_Result_Introduction = 2;
    printf("\n[+]:Socket was successfully created and bound\n");

    //Create the udp packet
    UDP_S=CreateUDP();
    if (UDP_S.length == 0) {
        printf("\n[+]:UDP packet creation error\n");
        TP_GenaralEndTest(GenaralEndTest_cb_Introduction, 28, Casename);
        printf("\n[+]:Test case UDP_INTRODUCTION_02 end with error n°1\n");
        test_Result = 1;

    } else {
        printf("\n[+]Udp packet successfully created\n");

        //Set Payload data to default data
        EditUDPField(&UDP_S, PAYLAOD, PAYLOAD_data, (void *) UDPConfiguration.UDPDefaultData);

        //Set Payload length
        EditUDPField(&UDP_S, PAYLAOD, PAYLOAD_length, (void *) (UDPConfiguration.UDPDefaultDataLen));

        //Set UDP header Length
        EditUDPField(&UDP_S, UDP, UDP_Length, (void *) (UDPConfiguration.UDPDefaultDataLen + 8));

        //Set UDP Dst port to unusedUDPDstport1
        EditUDPField(&UDP_S, UDP, UDP_DstPort, (void *) UDPConfiguration.unusedUDPDstPort1);

        //Set UDP Src  port to tester port
        EditUDPField(&UDP_S, UDP, UDP_SrcPort, (void *) UDPConfiguration.testerUDPPort);

        //Set Dst IP @ to all system McCast@
        EditUDPField(&UDP_S, IP, IP_DstAddress, (void *) UDPConfiguration.allSystemMCastAddr);
        printf("\n[+]:Destination ip address successfully set\n");

        //Compute packet checksums
        UDP_Compute_checksum(&UDP_S);

        //Push DUT to Receive incoming data
        TP_UdpReceiveAndForward(TP_UdpReceiveAndForward_cb_Introduction, TP_Udp_ReceiveAndForward_even_cb_Introduction,
                                socketId_Intro, 8, UDPConfiguration.UDPDefaultDataLen);
        while (callback_Result_Introduction != 0) { sleep(1); }
        callback_Result_Introduction = 2;
        printf("\n[+]:DUT is now ready to receive data\n");

        //Send udp packet
        if (SendUDP(UDP_S) ==-1) {
            printf("\n[+]:Error to send the packet\n");
            TP_GenaralEndTest(GenaralEndTest_cb_Introduction, 28, Casename);
            printf("\n[+]:Test case UDP_INTRODUCTION_02 end with error n°4\n");
            test_Result = 1;

        } else {
            printf("\n[+]:Packet delivered!\n");
            //close socket
            sleep(1);
            TP_UdpCloseSocket(UdpCloseSocket_cb_Introduction, socketId_Intro);
            while (callback_Result_Introduction != 0) { sleep(1); }
            callback_Result_Introduction = 2;
            printf("\n[+]:Socket id %d closed \n", socketId_Intro);

            //End the test
            TP_GenaralEndTest(GenaralEndTest_cb_Introduction, 28, Casename);
            test_Result = 0;
            printf("\n************[+]:Test case UDP_INTRODUCTION_02 was successfully ended*************\n");

        }
    }

return test_Result;
}

int UDP_INTRODUCTION_03() {
    callback_Result_Introduction = 2;
    uint8_t test_Result = 1;
    UDP_Packet UDP_S;
    ICMP_Packet ICMP_R;
    Packet_filter filter;

    ip4addr ipv4DUT;
    ipv4DUT.dataLength = 4;
    extractIpAddress(UDPConfiguration.Dst_IP_addr, &ipv4DUT);

    //Create the udp packet
    UDP_S = CreateUDP();
    if (UDP_S.length == 0) {
        printf("\n[+]:UDP packet creation error\n");
    } else {
        printf("\n[+]:Udp packet successfully created\n");

        //Set payload data ot default data
        EditUDPField(&UDP_S, PAYLAOD, PAYLOAD_data, (void *) UDPConfiguration.UDPDefaultData);

        //Set Payload length
        EditUDPField(&UDP_S, PAYLAOD, PAYLOAD_length, (void *) UDPConfiguration.UDPDefaultDataLen);

        //Set UDP Dst port to unusedUDPDstPort1
        EditUDPField(&UDP_S, UDP, UDP_DstPort, (void *) UDPConfiguration.unusedUDPDstPort1);

        //Set UDP Src port to tester port
        EditUDPField(&UDP_S, UDP, UDP_SrcPort, (void *) UDPConfiguration.testerUDPPort);

        //Set UDP Header length
        EditUDPField(&UDP_S, UDP, UDP_Length, (void *) (UDPConfiguration.UDPDefaultDataLen + 8));

        //compute checksums
        if (UDP_Compute_checksum(&UDP_S) == 1) {
            printf("\n[+]:Error to set checksum\n");
        } else {
            //send data
            if (SendUDP(UDP_S) ==-1) {
                printf("\n[+]:Error to send the packet\n");
                test_Result=1;
            } else {
                printf("\n[+]:Packet delivered!\n");

                //set filter configuration to receive the icmp packet
                strcpy(filter.SrcAdd, UDPConfiguration.Dst_IP_addr);
                strcpy(filter.dstAdd, UDPConfiguration.Host_1_IP);
                uint8_t timeout = 3;

                //Listen and receive the icmp packet
                ICMP_R = ListenICMP(filter, timeout);
                if (ICMP_R.length == 0) {
                    printf("\n[+]:Error DUT doesn't respond\n");


                } else {
                    printf("\n[+]:Tester received a icmp packet\n");

                    //Verify that tester receive a icmp packet Destination unreachable
                    if (GetICMPField(&ICMP_R, ICMP, ICMP_code) != 3) {
                        printf("\n[+]:Error Expected code 3\n");
                        test_Result=1;

                    } else {
                        printf("\n[+]:Destination unreachable\n");
                        test_Result = 0;
                        printf("\n***********[+]: Test case UDP_INTRODUCTION_03 was successfully end**********\n");
                    }

                }
            }
        }
    }
    return test_Result;
}
