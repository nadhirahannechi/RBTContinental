
#include <stdio.h>
#include <unistd.h>


#include "AbstractionAPI.h"
#include "TestabilityProtocol_api.h"
#include "TestabilityProtocol_Intern.h"
#include "UDP_config.h"
#include "UDP_Padding.h"
#include "split.h"

extern UDP_config_t UDPConfiguration;

uint8 callback_Result_Padding;
uint16 socketId_Padding;

void GenaralStartTest_cb(TP_ResultID_t Result) {
    callback_Result_Padding = Result;
}

void UdpCreateAndBind_cb(TP_ResultID_t Result, uint16 s) {
    callback_Result_Padding = Result;
    socketId_Padding = s;
}

void TP_UdpSendData_cb(TP_ResultID_t Result) {
    callback_Result_Padding = Result;
}

void UdpCloseSocket_cb(TP_ResultID_t Result) {
    callback_Result_Padding = Result;
}

void GenaralEndTest_cb(TP_ResultID_t Result) {
    callback_Result_Padding = Result;
}


int UDP_Padding_02() {
    callback_Result_Padding = 2;
    uint8_t test_Result = 1;
    ip4addr ipv4DUT, ipv4Tester;
    //DUT ip address
    ipv4DUT.dataLength = 4;
    extractIpAddress(UDPConfiguration.Dst_IP_addr, &ipv4DUT);
    //Tester ip address
    ipv4Tester.dataLength = 4;
    extractIpAddress(UDPConfiguration.Host_1_IP, &ipv4Tester);

    UDP_Packet UDP_R;
    Packet_filter filter;


    vint8 Casename, UdpUserData;

    uint8 Case[] = "UDP_Padding_02 : To verify that IUT generates UDP datagram with even size of payload and no padding at the end";
    Casename.dataLength = sizeof(Case);
    Casename.Data = (uint8 *) calloc(Casename.dataLength, sizeof(uint8));
    memcpy(Casename.Data, Case, Casename.dataLength);

    UdpUserData.dataLength = UDPConfiguration.udpUserDataSize;
    UdpUserData.Data = (uint8 *) calloc(UDPConfiguration.udpUserDataSize, sizeof(uint8));


    //start test
    TP_GenaralStartTest(GenaralStartTest_cb);
    while (callback_Result_Padding != 0) { sleep(1); }
    callback_Result_Padding = 2;
    printf("\n[+]:Test started\n");

    //Create a socket and bind it
    TP_UdpCreateAndBind(UdpCreateAndBind_cb, TRUE, UDPConfiguration.DUTUDPPort, ipv4DUT);
    while (callback_Result_Padding != 0) { sleep(1); }

    callback_Result_Padding = 2;
    printf("\\n[+]Socket was successfully created and bound\n");

    //set the filter configurations to receive the packet
    filter.Dstport = UDPConfiguration.testerUDPPort;
    filter.Srcport = UDPConfiguration.DUTUDPPort;
    strcpy(filter.dstAdd, UDPConfiguration.Host_1_IP);
    strcpy(filter.SrcAdd, UDPConfiguration.Dst_IP_addr);
    uint8_t timeout = 3;

    //push DUT generate and send data
    TP_UdpSendData(TP_UdpSendData_cb, socketId_Padding, 0, UDPConfiguration.testerUDPPort, ipv4Tester, UdpUserData);
    UDP_R = ListenUDP(filter, timeout);//Tester Listen and receive the incoming data
    while (callback_Result_Padding != 0) { sleep(1); }
    callback_Result_Padding = 2;
    printf("\n[+]:DUT sent data\n");

    //Verify that tester received the packet

    if (UDP_R.length == 0) {
        printf("\n[+]:Error DUT doesn't respond\n");
        TP_GenaralEndTest(GenaralEndTest_cb, 1, Casename);
        printf("\n[+]:test case 3 end with error n°1\n");
        test_Result = 1;
    } else {
        printf("\n[+]: UDP packet well received\n");

        //Verify that payload length was 101
        if ((GetUDPField(&UDP_R, IP, IP_TotalLength) - 28) != UDPConfiguration.udpUserDataSize) {
            printf("\n[+]:data length not equal to 101\n");
            TP_GenaralEndTest(GenaralEndTest_cb, 1, Casename);
            printf("\n[+]:Test case UDP_Padding_02 end with error n°2");
            test_Result = 1;

        } else {
            printf("\n[+]:DUT doesn't generate padding bytes\n");

            //Close socket
            TP_UdpCloseSocket(UdpCloseSocket_cb, socketId_Padding);
            while (callback_Result_Padding != 0) { sleep(1); }
            callback_Result_Padding = 2;
            printf("\n[+]:socket id %d closed \n", socketId_Padding);

            //End Test
            TP_GenaralEndTest(GenaralEndTest_cb, 1, Casename);
            test_Result = 0;
            printf("\n[+]:Test cases UDP_Padding_02 was successfully ended\n");
        }
    }

return 2;
}
