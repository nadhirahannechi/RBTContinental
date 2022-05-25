#include <stdio.h>
#include <unistd.h>

#include "AbstractionAPI.h"
#include "TestabilityProtocol_Intern.h"
#include "TestabilityProtocol_api.h"

#include "UDP_MessageFormat.h"
#include "split.h"
#include "UDP_config.h"

extern UDP_config_t UDPConfiguration;

uint8 callback_Result_MessageF;
uint16 socketId_MessageF, socketid_M;
uint16 Len_M, Srcport_M;
ip4addr ipv4Addr_M;
vint8 UDP_Data_M;


void GenaralStartTest_cb_MessageF(TP_ResultID_t Result) {
    callback_Result_MessageF = Result;
}

void UdpCreateAndBind_cb_MessageF(TP_ResultID_t Result, uint16 s) {
    callback_Result_MessageF = Result;
    socketId_MessageF = s;
}

void TP_UdpReceiveAndForward_cb_MessageF(TP_ResultID_t Result, uint16 s) {
    callback_Result_MessageF = Result;
    socketid_M = s;
}

void TP_Udp_ReceiveAndForward_even_cb_MessageF(TP_ResultID_t Result, uint16 Length, uint16 Port, ip4addr Address,
                                               vint8 DataUdp) {
    callback_Result_MessageF = Result;
    Len_M = Length;
    Srcport_M = Port;
    ipv4Addr_M = Address;
    UDP_Data_M = DataUdp;
}

void UdpCloseSocket_cb_MessageF(TP_ResultID_t Result) {
    callback_Result_MessageF = Result;
}

void GenaralEndTest_cb_MessageF(TP_ResultID_t Result) {
    callback_Result_MessageF = Result;
}


int UDP_MessageFormat_02() {
    uint8_t test_Result = 1;
    callback_Result_MessageF = 2;
    ip4addr ipv4DUT;
    UDP_Packet UDP_S;
    vint8 Casename;

    ipv4DUT.dataLength = 4;
    extractIpAddress(UDPConfiguration.Dst_IP_addr, &ipv4DUT);


    uint8 Case[] = "UDP_MessageFormat_02: To verify that IUT accepts an UDP packet containing a well-formed UDP header";
    Casename.dataLength = sizeof(Case);
    Casename.Data = (uint8 *) calloc(Casename.dataLength, sizeof(uint8));
    memcpy(Casename.Data, Case, Casename.dataLength);

    //Start Test
    TP_GenaralStartTest(GenaralStartTest_cb_MessageF);
    while (callback_Result_MessageF != 0) { sleep(1); }
    callback_Result_MessageF = 2;
    printf("\n[+]:Test UDP_MessageFormat_02 was started\n");

    //create and bind socket
    TP_UdpCreateAndBind(UdpCreateAndBind_cb_MessageF, TRUE, UDPConfiguration.DUTUDPPort, ipv4DUT);
    while (callback_Result_MessageF != 0) { sleep(1); }
    callback_Result_MessageF = 2;
    printf("\n[+]:Socket was successfully created and bound\n");


    //Creation an UDP UDP_S
    printf("\n[+]:Creation of UDP packet");
    UDP_S = CreateUDP();
    if (UDP_S.length == 0) {
        printf("\n[+]:Failure to create a udp packet\n");
        TP_GenaralEndTest(GenaralEndTest_cb_MessageF, 1, Casename);
        printf("\n[+]:Test case UDP_Message_Format_02 end with error n°1\n");
    } else {
        printf("\n[+]:UDP packet was successfully created\n");

        //set payload data
        EditUDPField(&UDP_S, PAYLAOD, PAYLOAD_data, (void *) UDPConfiguration.UDPDefaultData);

        //set payload length
        EditUDPField(&UDP_S, PAYLAOD, PAYLOAD_length, (void *) UDPConfiguration.UDPDefaultDataLen);

        //set UDP Dst port
        EditUDPField(&UDP_S, UDP, UDP_DstPort, (void *) UDPConfiguration.DUTUDPPort);

        //Set UPD src port
        EditUDPField(&UDP_S, UDP, UDP_SrcPort, (void *) UDPConfiguration.testerUDPPort);

        //Set UPD header length
        EditUDPField(&UDP_S, UDP, UDP_Length, (void *) (UDPConfiguration.UDPDefaultDataLen + 8));


        //Compute packet checksums
        UDP_Compute_checksum(&UDP_S);

        //push DUT to listen and listen incoming UDP data
        TP_UdpReceiveAndForward(TP_UdpReceiveAndForward_cb_MessageF, TP_Udp_ReceiveAndForward_even_cb_MessageF,socketId_MessageF, 8, UDPConfiguration.UDPDefaultDataLen);
        while (callback_Result_MessageF != 0) { sleep(1); }
        callback_Result_MessageF = 2;
        printf("\n[+]:DUT is ready to receive and forward data\n");

        //Send an UDP UDP_S
        if (SendUDP(UDP_S) == -1) {
            printf("\n[+]:Error on sending \n");
            TP_GenaralEndTest(GenaralEndTest_cb_MessageF, 1, Casename);
            printf("\n[+]:Test case UDP_Message_Format_02 end with error n°2\n");
            test_Result = 1;

        } else {
            callback_Result_MessageF = 2;
            printf("\n[+]:Packet delivered\n");

            sleep(1);
            //Close socket
            TP_UdpCloseSocket(UdpCloseSocket_cb_MessageF, socketId_MessageF);
            while (callback_Result_MessageF != 0) { sleep(1); }
            callback_Result_MessageF = 2;
            printf("\n[+]:socket id %d closed \n", socketId_MessageF);

            //Ending test
            TP_GenaralEndTest(GenaralEndTest_cb_MessageF, 1, Casename);
            printf("\n*******************[+]:test case UDP_Message_Format_02 was successfully end*********************** \n");


        }
    }

return
test_Result;
}