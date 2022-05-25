#include <stdio.h>
#include <unistd.h>

#include "AbstractionAPI.h"
#include "TestabilityProtocol_api.h"
#include "TestabilityProtocol_Intern.h"

#include "UDP_USER_INTERFACE.h"
#include "UDP_config.h"
#include "split.h"

extern UDP_config_t UDPConfiguration;

uint8 callback_Result_USER;
uint16 socketId_USER, socketid_USER;
uint16 Len_USER, Srcport_USER;
ip4addr ipv4Addr_USER;
vint8 UDP_Data_USER;


void GenaralStartTest_cb_USER(TP_ResultID_t Result) {
    callback_Result_USER = Result;
}

void UdpCreateAndBind_cb_USER(TP_ResultID_t Result, uint16 s) {
    callback_Result_USER = Result;
    socketId_USER = s;
}

void TP_UdpReceiveAndForward_cb_USER(TP_ResultID_t Result, uint16 s) {
    callback_Result_USER = Result;
    socketid_USER = s;
}

void TP_Udp_ReceiveAndForward_even_cb_USER(TP_ResultID_t Result, uint16 Length, uint16 Port, ip4addr Address,
                                           vint8 DataUdp) {
    callback_Result_USER = Result;
    Len_USER = Length;
    Srcport_USER = Port;
    ipv4Addr_USER = Address;
    UDP_Data_USER = DataUdp;
    printf("\n[+]:***********%d", Length);
}

void TP_UdpSendData_cb_USER(TP_ResultID_t Result) {
    callback_Result_USER = Result;
}

void UdpCloseSocket_cb_USER(TP_ResultID_t Result) {
    callback_Result_USER = Result;
}

void GenaralEndTest_cb_USER(TP_ResultID_t Result) {
    callback_Result_USER = Result;
}


int UDP_USER_INTERFACE_01() {
    uint8_t test_Result = 1;
    callback_Result_USER = 2;
    uint16 Ports[] = {33000, 33001, 33002, 33003, 33004, 33005, 33006, 33007, 33008, 33009};
    ip4addr ipv4DUT;
    ipv4DUT.dataLength = 4;
    extractIpAddress(UDPConfiguration.Dst_IP_addr, &ipv4DUT);
    vint8 Casename;
    uint8 Case[] = "UDP_USER_INTERFACE_01: User Interface - New Receive Port";
    Casename.dataLength = sizeof(Case);
    Casename.Data = (uint8 *) calloc(Casename.dataLength, sizeof(uint8));
    memcpy(Casename.Data, Case, Casename.dataLength);

    TP_GenaralStartTest(GenaralStartTest_cb_USER);
    sleep(3);
    while (callback_Result_USER != 0) { sleep(2); }

    printf("\n[+]:Test started\n");
    uint8_t i = 0;
    while (i < 10) {
        callback_Result_USER = 2;

        //Create 10 ports and bind them to the socket
        TP_UdpCreateAndBind(UdpCreateAndBind_cb_USER, TRUE, Ports[i], ipv4DUT);
        while (callback_Result_USER != 0) { sleep(2); }

        callback_Result_USER = 2;
        printf("\n[+]:the %d port was create and bind to socket id : %d\n", Ports[i], socketId_USER);
        i++;
    }
    printf("\n[+]:Creation of 10 ports was done successfully!\n");

    //Close the socket
    TP_UdpCloseSocket(UdpCloseSocket_cb_USER, socketId_USER);
    while (callback_Result_USER != 0) { sleep(1); }
    callback_Result_USER = 2;
    printf("\n[+]:socket id %d closed \n", socketId_USER);

    //End test
    TP_GenaralEndTest(GenaralEndTest_cb_USER, 1, Casename);
    printf("\n[+]:test case UDP_USER_INTERFACE_01 was successfully ended\n");
    test_Result = 0;

    return test_Result;
}

int UDP_USER_INTERFACE_02() {
    uint8_t test_Result = 1;
    callback_Result_USER = 2;
    ip4addr ipv4DUT;
    ipv4DUT.dataLength = 4;
    extractIpAddress(UDPConfiguration.Dst_IP_addr, &ipv4DUT);


    UDP_Packet UDP_S;
    vint8 Casename;

    uint8 Case[] = "UDP_USER_INTERFACE_02: User Interface - Data octets";
    Casename.dataLength = sizeof(Case);
    Casename.Data = (uint8 *) calloc(Casename.dataLength, sizeof(uint8));
    memcpy(Casename.Data, Case, Casename.dataLength);


    //start test
    TP_GenaralStartTest(GenaralStartTest_cb_USER);
    while (callback_Result_USER != 0) { sleep(1); }

    callback_Result_USER = 2;
    printf("\n[+]:Test UDP_USER_INTERFACE_02 was started\n");

    //Create and bind the socket
    TP_UdpCreateAndBind(UdpCreateAndBind_cb_USER, TRUE, UDPConfiguration.unusedUDPDstPort1, ipv4DUT);
    while (callback_Result_USER != 0) { sleep(1); }

    callback_Result_USER = 2;
    printf("\n[+]:Socket as successfully created and bound\n");


    //Creation of the packet
    UDP_S = CreateUDP();
    if (UDP_S.length == 0) {//verify that creation of packet done
        printf("\n[+]:Error to create UDP packet \n");
        TP_GenaralEndTest(GenaralEndTest_cb_USER, 3, Casename);
        printf("\n[+]:Test case UDP_USER_INTERFACE_03end with error n°1\n");
    } else {
        printf("\n[+]:Udp packet well created\n");

        //set payload data
        EditUDPField(&UDP_S, PAYLAOD, PAYLOAD_data, (void *) UDPConfiguration.UDPDefaultData);

        //set payload length
        EditUDPField(&UDP_S, PAYLAOD, PAYLOAD_length, (void *) UDPConfiguration.UDPDefaultDataLen);

        //set UDP src port
        EditUDPField(&UDP_S, UDP, UDP_SrcPort, (void *) UDPConfiguration.testerUDPPort);

        //set UDP Dst port
        EditUDPField(&UDP_S, UDP, UDP_DstPort, (void *) UDPConfiguration.unusedUDPDstPort1);

        //set UDP header length
        EditUDPField(&UDP_S, UDP, UDP_Length, (void *) (UDPConfiguration.UDPDefaultDataLen + 8));

        //compute  Packet checksums
        UDP_Compute_checksum(&UDP_S);

        //DUT listen and receive the incoming data
        TP_UdpReceiveAndForward(TP_UdpReceiveAndForward_cb_USER, TP_Udp_ReceiveAndForward_even_cb_USER, socketId_USER,
                                8, UDPConfiguration.UDPDefaultDataLen);
        while (callback_Result_USER != 0) { sleep(1); }

        callback_Result_USER = 2;
        printf("\n[+]:DUT is ready to receive and forward data\n");

        //Send the packet to DUT
        if (SendUDP(UDP_S) == -1) {
            printf("\n[+]:Error on sending\n");
            TP_GenaralEndTest(GenaralEndTest_cb_USER, 3, Casename);
            printf("\n[+]:Ending test UDP_USER_INTERFACE_02 with error n°4\n");
            test_Result = 1;
        } else {
            printf("\n[+]:Udp packet well delivered \n");

            //Close the socket
            sleep(1);
            TP_UdpCloseSocket(UdpCloseSocket_cb_USER, socketId_USER);
            while (callback_Result_USER != 0) { sleep(1); }
            callback_Result_USER = 2;
            printf("\n[+]:socket id %d closed", socketId_USER);

            //end the test
            TP_GenaralEndTest(GenaralEndTest_cb_USER, 3, Casename);
            test_Result = 0;
            printf("\n*********************[+]:Test case UDP_USER_INTERFACE_02 was successfully ended**************************\n");


        }
    }
    return test_Result;
}

int UDP_USER_INTERFACE_03() {
    uint8_t test_Result = 1;
    callback_Result_USER = 2;

    ip4addr ipv4DUT;
    ipv4DUT.dataLength = 4;
    extractIpAddress(UDPConfiguration.Dst_IP_addr, &ipv4DUT);


    UDP_Packet UDP_S;
    vint8 Casename;

    uint8 Case[] = "UDP_USER_INTERFACE_03: User Interface – Return Source Port";
    Casename.dataLength = sizeof(Case);
    Casename.Data = (uint8 *) calloc(Casename.dataLength, sizeof(uint8));
    memcpy(Casename.Data, Case, Casename.dataLength);

    //start test
    TP_GenaralStartTest(GenaralStartTest_cb_USER);
    while (callback_Result_USER != 0) { sleep(1); }
    callback_Result_USER = 2;
    printf("\n[+]:Test UDP_USER_INTERFACE_03 was started\n");

    //Create and bind the socket
    TP_UdpCreateAndBind(UdpCreateAndBind_cb_USER, TRUE, UDPConfiguration.unusedUDPDstPort1, ipv4DUT);
    while (callback_Result_USER != 0) { sleep(1); }
    callback_Result_USER = 2;
    printf("\n[+]:Socket as successfully created and bound\n");


    //Creation of the packet
    UDP_S = CreateUDP();
    if (UDP_S.length == 0) {//Verify that creation of packet done
        printf("\n[+]:Error to create UDP packet \n");
        TP_GenaralEndTest(GenaralEndTest_cb_USER, 3, Casename);
        printf("\n[+]:Test case UDP_USER_INTERFACE_03end with error n°1\n");
        test_Result = 1;
    } else {
        printf("\n[+]:Udp packet well created\n");

        //set payload data
        EditUDPField(&UDP_S, PAYLAOD, PAYLOAD_data, (void *) UDPConfiguration.UDPDefaultData);

        //set payload length
        EditUDPField(&UDP_S, PAYLAOD, PAYLOAD_length, (void *) UDPConfiguration.UDPDefaultDataLen);

        //set UDP src port
        EditUDPField(&UDP_S, UDP, UDP_SrcPort, (void *) UDPConfiguration.unusedUDPSrcPort);

        //set UDP Dst port
        EditUDPField(&UDP_S, UDP, UDP_DstPort, (void *) UDPConfiguration.unusedUDPDstPort1);

        //Set UDP Header length
        EditUDPField(&UDP_S, UDP, UDP_Length, (void *) (UDPConfiguration.UDPDefaultDataLen + 8));


        //compute packet checksums
        UDP_Compute_checksum(&UDP_S);

        //DUT listen for incoming data
        TP_UdpReceiveAndForward(TP_UdpReceiveAndForward_cb_USER, TP_Udp_ReceiveAndForward_even_cb_USER, socketId_USER,
                                8, UDPConfiguration.UDPDefaultDataLen);
        while (callback_Result_USER != 0) { sleep(1); }

        callback_Result_USER = 2;
        printf("\n[+]:DUT is ready to receive and forward data\n");

        //Send the packet to DUT
        if (SendUDP(UDP_S) != 1) {
            printf("\n[+]:Error on sending\n");
            TP_GenaralEndTest(GenaralEndTest_cb_USER, 3, Casename);
            printf("\n[+]:Ending test UDP_USER_INTERFACE_03 with error n°4\n");
            test_Result = 1;
        } else {
            printf("\n[+]:Udp packet well delivered \n");

            //Close the socket
            sleep(1);
            TP_UdpCloseSocket(UdpCloseSocket_cb_USER, socketId_USER);
            while (callback_Result_USER != 0) { sleep(1); }
            callback_Result_USER = 2;
            printf("\n[+]:socket id %d closed", socketId_USER);

            //end the test
            TP_GenaralEndTest(GenaralEndTest_cb_USER, 3, Casename);
            test_Result = 0;
            printf("\n*******[+]:Test case UDP_USER_INTERFACE_03 was successfully ended**********\n");


        }
    }

    return test_Result;
}

int UDP_USER_INTERFACE_04() {
    uint8_t test_Result = 1;
    callback_Result_USER = 2;

    ip4addr ipv4DUT;

    ipv4DUT.dataLength = 4;
    extractIpAddress(UDPConfiguration.Dst_IP_addr, &ipv4DUT);


    UDP_Packet UDP_S;
    vint8 Casename;

    uint8 Case[] = "UDP_USER_INTERFACE_04: User Interface – Return Source IP Address";
    Casename.dataLength = sizeof(Case);
    Casename.Data = (uint8 *) calloc(Casename.dataLength, sizeof(uint8));
    memcpy(Casename.Data, Case, Casename.dataLength);

    //start test
    TP_GenaralStartTest(GenaralStartTest_cb_USER);
    while (callback_Result_USER != 0) { sleep(1); }
    callback_Result_USER = 2;
    printf("\n[+]:Test UDP_USER_INTERFACE_04 was started\n");

    //create and bind the socket
    TP_UdpCreateAndBind(UdpCreateAndBind_cb_USER, TRUE, UDPConfiguration.unusedUDPDstPort1, ipv4DUT);
    while (callback_Result_USER != 0) { sleep(1); }
    callback_Result_USER = 2;
    printf("\n[+]:socket was successfully created and bound ");

    //Creation of the packet
    UDP_S = CreateUDP();
    if (UDP_S.length == 0) {//Verify that creation of packet done
        printf("\n[+]:Failure Error to create to UDP packet");
        TP_GenaralEndTest(GenaralEndTest_cb_USER, 4, Casename);
        printf("\n[+]:Test case UDP_USER_INTERFACE_04 end with error n°1");
        test_Result = 1;

    } else {
        printf("\n[+]:UDP packet well created\n");

        //set payload data
        EditUDPField(&UDP_S, PAYLAOD, PAYLOAD_data, (void *) UDPConfiguration.UDPDefaultData);

        //set payload length
        EditUDPField(&UDP_S, PAYLAOD, PAYLOAD_length, (void *) UDPConfiguration.UDPDefaultDataLen);

        //set UDP src port
        EditUDPField(&UDP_S, UDP, UDP_SrcPort, (void *) UDPConfiguration.testerUDPPort);

        //Set UdP Dst port
        EditUDPField(&UDP_S, UDP, UDP_DstPort, (void *) UDPConfiguration.unusedUDPDstPort1);

        //Set UPD header length
        EditUDPField(&UDP_S, UDP, UDP_Length, (void *) (UDPConfiguration.UDPDefaultDataLen + 8));

        //set tester @ ip
        EditUDPField(&UDP_S, IP, IP_SrcAddress, (void *) UDPConfiguration.Host_1_IP);


        //compute the packet checksums
        UDP_Compute_checksum(&UDP_S);

        //push DUT to listen and receive the incoming data
        TP_UdpReceiveAndForward(TP_UdpReceiveAndForward_cb_USER, TP_Udp_ReceiveAndForward_even_cb_USER, socketId_USER,
                                8, UDPConfiguration.UDPDefaultDataLen);
        while (callback_Result_USER != 0) { sleep(1); }
        callback_Result_USER = 2;
        printf("\n[+]:DUT is ready to receive and forward data\n");

        //Send the packet to DUT
        if (SendUDP(UDP_S) != 1) {
            printf("\n[+]:Error on sending\n");
            TP_GenaralEndTest(GenaralEndTest_cb_USER, 4, Casename);
            printf("\n[+]:Ending test UDP_USER_INTERFACE_04 with error n°4\n");
            test_Result = 1;

        } else {
            printf("\n[+]:Udp packet delivered\n");


            //Close the socket
            sleep(1);
            TP_UdpCloseSocket(UdpCloseSocket_cb_USER, socketId_USER);
            while (callback_Result_USER != 0) { sleep(1); }
            callback_Result_USER = 2;
            printf("\n[+]:Socket id %d closed \n", socketId_USER);

            //end the test
            TP_GenaralEndTest(GenaralEndTest_cb_USER, 4, Casename);
            test_Result = 0;
            printf("\n********[+]:Test Case UDP_USER_INTERFACE_04  was successfully ended********** \n");

        }
    }

    return test_Result;
}


int UDP_USER_INTERFACE_05() {
    callback_Result_USER = 2;
    uint8_t test_Result = 1;
    ip4addr ipv4DUT, ipv4Tester;
    ipv4DUT.dataLength = 4;
    extractIpAddress(UDPConfiguration.Dst_IP_addr, &ipv4DUT);


    ipv4Tester.dataLength = 4;
    extractIpAddress(UDPConfiguration.Host_1_IP, &ipv4Tester);


    UDP_Packet UDP_R;
    Packet_filter filter;
    vint8 UDPData, Casename;

    UDPData.dataLength = UDPConfiguration.UDPDataLen;
    UDPData.Data = (uint8 *) calloc(UDPConfiguration.UDPDataLen, sizeof(uint8));
    memcpy(UDPData.Data, UDPConfiguration.UDPData, UDPData.dataLength);

    uint8 Case[] = "UDP_USER_INTERFACE_05: User Interface - Source Port (to be sent)";
    Casename.dataLength = sizeof(Case);
    Casename.Data = (uint8 *) calloc(Casename.dataLength, sizeof(uint8));
    memcpy(Casename.Data, Case, Casename.dataLength);


    //start test
    TP_GenaralStartTest(GenaralStartTest_cb_USER);
    while (callback_Result_USER != 0) { sleep(1); }
    printf("\n[+]:Test UDP_USER_INTERFACE_05 was started\n");
    callback_Result_USER = 2;

    //Create socket and bind it
    TP_UdpCreateAndBind(UdpCreateAndBind_cb_USER, TRUE, UDPConfiguration.unusedUDPSrcPort, ipv4DUT);
    while (callback_Result_USER != 0) { sleep(1); }
    callback_Result_USER = 2;
    printf("\n[+]:socket was successfully created and bound\n");

    //Set filter configurations to receive the incoming data
    strcpy(filter.SrcAdd, UDPConfiguration.Dst_IP_addr);
    filter.Srcport = UDPConfiguration.unusedUDPSrcPort;
    strcpy(filter.dstAdd, UDPConfiguration.Host_1_IP);
    filter.Dstport = UDPConfiguration.testerUDPPort;
    uint8_t timeout = 3;

    //DUT sending data
    TP_UdpSendData(TP_UdpSendData_cb_USER, socketId_USER, 0, UDPConfiguration.testerUDPPort, ipv4Tester, UDPData);
    UDP_R = ListenUDP(filter, timeout);//Tester listen and receive the incoming data
    while (callback_Result_USER != 0) { sleep(1); }
    callback_Result_USER = 2;
    printf("\n[+]:DUT send data\n");

    //Verify that tester well receive the packet
    if (UDP_R.length == 0) {
        printf("\n[+]:Error not UDP packet received\n");
        TP_GenaralEndTest(GenaralEndTest_cb_USER, 5, Casename);
        printf("Test case UDP_USER_INTERFACE_05 was ended with error n°1\n");
        test_Result = 1;

    } else {
        printf("\n[+]:Packet received \n");

        //display source port
        printf("\n[+]:Source port equal to %ld:\n", GetUDPField(&UDP_R, UDP, UDP_SrcPort));

        //Close socket
        TP_UdpCloseSocket(UdpCloseSocket_cb_USER, socketId_USER);
        while (callback_Result_USER != 0) { sleep(1); }
        printf("\n[+]:socket %d was closed \n", socketId_USER);

        //End Test
        TP_GenaralEndTest(GenaralEndTest_cb_USER, 5, Casename);
        test_Result = 0;
        printf("\n**********[+]:Test case UDP_USER_INTERFACE_05 was successfully ended**************\n");
    }

    return test_Result;
}

int UDP_USER_INTERFACE_06() {
    callback_Result_USER = 2;
    uint8_t test_Result = 1;
    ip4addr ipv4DUT, ipv4Tester;

    UDP_Packet UDP_R;
    ipv4DUT.dataLength = 4;
    extractIpAddress(UDPConfiguration.Dst_IP_addr, &ipv4DUT);


    ipv4Tester.dataLength = 4;
    extractIpAddress(UDPConfiguration.Host_1_IP, &ipv4Tester);


    Packet_filter filter;
    vint8 Casename, Data;

    uint8 Case[] = "UDP_USER_INTERFACE_06: User Interface - Destination Port (to be sent)";
    Casename.dataLength = sizeof(Case);
    Casename.Data = (uint8 *) calloc(Casename.dataLength, sizeof(uint8));
    memcpy(Casename.Data, Case, Casename.dataLength);

    Data.dataLength = UDPConfiguration.UDPDefaultDataLen;
    Data.Data = (uint8 *) calloc(Data.dataLength, sizeof(uint8));
    strcpy(Data.Data, UDPConfiguration.UDPDefaultData);


    //start test
    TP_GenaralStartTest(GenaralStartTest_cb_USER);
    while (callback_Result_USER != 0) { sleep(1); }

    printf("\n[+]:Test UDP_USER_INTERFACE_06 was started\n");
    callback_Result_USER = 2;

    //Create socket and bind it
    TP_UdpCreateAndBind(UdpCreateAndBind_cb_USER, TRUE, UDPConfiguration.DUTUDPPort, ipv4DUT);
    while (callback_Result_USER != 0) { sleep(1); }
    callback_Result_USER = 2;
    printf("\n[+]:socket was successfully created and bound\n");

    //set filter configurations to receive data from DUT
    strcpy(filter.SrcAdd, UDPConfiguration.Dst_IP_addr);
    filter.Srcport = UDPConfiguration.DUTUDPPort;
    strcpy(filter.dstAdd, UDPConfiguration.Host_1_IP);
    filter.Dstport = UDPConfiguration.testerUDPPort;
    uint8_t timeout = 3;

    //DUT sending data
    TP_UdpSendData(TP_UdpSendData_cb_USER, socketId_USER, 0, UDPConfiguration.testerUDPPort, ipv4Tester, Data);
    UDP_R = ListenUDP(filter, timeout);//Tester listen and receive the incoming data
    while (callback_Result_USER != 0) { sleep(1); }
    callback_Result_USER = 2;
    printf("\n[+]:DUT send data\n");

    //verify that tester receive the packet
    if (UDP_R.length == 0) {
        printf("\n[+]:Error not UDP packet received\n");
        TP_GenaralEndTest(GenaralEndTest_cb_USER, 6, Casename);
        printf("Test case UDP_USER_INTERFACE_06 was ended with error n°1\n");
        test_Result = 1;

    } else {
        printf("\n[+]:Packet received \n");

        //print the Destination IP @
        printf("\n[+]:Destination port equal to %ld:\n", GetUDPField(&UDP_R, UDP, UDP_DstPort));

        //Close socket
        TP_UdpCloseSocket(UdpCloseSocket_cb_USER, socketId_USER);
        while (callback_Result_USER != 0) { sleep(1); }
        printf("\n[+]:socket %d was closed \n", socketId_USER);

        //End Test
        TP_GenaralEndTest(GenaralEndTest_cb_USER, 6, Casename);
        test_Result = 0;
        printf("\n************[+]:Test case UDP_USER_INTERFACE_06  was successfully ended*************\n");
        //close Control channel
    }

    return test_Result;
}

int UDP_USER_INTERFACE_07() {
    callback_Result_USER = 2;
    uint8_t test_Result = 1;
    ip4addr ipv4DUT, ipv4Tester;

    UDP_Packet UDP_R;
    ipv4DUT.dataLength = 4;
    extractIpAddress(UDPConfiguration.Dst_IP_addr, &ipv4DUT);


    ipv4Tester.dataLength = 4;
    extractIpAddress(UDPConfiguration.Host_1_IP, &ipv4Tester);


    Packet_filter filter;
    vint8 Casename, Data;

    uint8 Case[] = "UDP_USER_INTERFACE_07: User Interface - Source IP Address (to be sent)";
    Casename.dataLength = sizeof(Case);
    Casename.Data = (uint8 *) calloc(Casename.dataLength, sizeof(uint8));
    memcpy(Casename.Data, Case, Casename.dataLength);

    Data.dataLength = UDPConfiguration.UDPDefaultDataLen;
    Data.Data = (uint8 *) calloc(Data.dataLength, sizeof(uint8));
    strcpy(Data.Data, UDPConfiguration.UDPDefaultData);


    //start test
    TP_GenaralStartTest(GenaralStartTest_cb_USER);
    while (callback_Result_USER != 0) { sleep(1); }
    printf("\n[+]:Test UDP_USER_INTERFACE_07 was started\n");
    callback_Result_USER = 2;

    //Create socket and bind it
    TP_UdpCreateAndBind(UdpCreateAndBind_cb_USER, TRUE, UDPConfiguration.DUTUDPPort, ipv4DUT);
    while (callback_Result_USER != 0) { sleep(1); }
    callback_Result_USER = 2;
    printf("\n[+]:socket was successfully created and bound\n");

    //Set filter configurations to receive data from DUT
    strcpy(filter.SrcAdd, UDPConfiguration.Dst_IP_addr);
    filter.Srcport = UDPConfiguration.DUTUDPPort;
    strcpy(filter.dstAdd, UDPConfiguration.Host_1_IP);
    filter.Dstport = UDPConfiguration.testerUDPPort;
    uint8_t timeout = 3;

    //DUT sending data
    TP_UdpSendData(TP_UdpSendData_cb_USER, socketId_USER, 0, UDPConfiguration.testerUDPPort, ipv4Tester, Data);
    UDP_R = ListenUDP(filter, timeout);//Tester listen and receive the incoming data
    while (callback_Result_USER != 0) { sleep(1); }
    callback_Result_USER = 2;
    printf("\n[+]:DUT send data\n");

    //Verify that tester well receive the packet
    if (UDP_R.length == 0) {
        printf("\n[+]:Error not UDP packet received\n");
        TP_GenaralEndTest(GenaralEndTest_cb_USER, 7, Casename);
        printf("Test case UDP_USER_INTERFACE_07 was ended with error n°1\n");
        test_Result = 1;
    } else {
        printf("\n[+]:Packet received \n");
        printf("\n[+]:Source IP address equal to :%s\n", GetUDPField(&UDP_R, IP, IP_SrcAddress));

        //Close socket
        TP_UdpCloseSocket(UdpCloseSocket_cb_USER, socketId_USER);
        while (callback_Result_USER != 0) { sleep(1); }
        printf("\n[+]:socket %d was closed \n", socketId_USER);

        //End Test
        TP_GenaralEndTest(GenaralEndTest_cb_USER, 7, Casename);
        test_Result = 0;
        printf("\n***********[+]:Test case UDP_USER_INTERFACE_06 was successfully ended*************\n");
        //close Control channel
    }
    return test_Result;
}

int UDP_USER_INTERFACE_08() {
    uint8_t test_Result = 1;
    callback_Result_USER = 2;
    ip4addr ipv4DUT, ipv4Tester;

    UDP_Packet UDP_R;
    ipv4DUT.dataLength = 4;
    extractIpAddress(UDPConfiguration.Dst_IP_addr, &ipv4DUT);


    ipv4Tester.dataLength = 4;
    extractIpAddress(UDPConfiguration.Host_1_IP, &ipv4Tester);


    Packet_filter filter;
    vint8 Casename, Data;

    uint8 Case[] = "UDP_USER_INTERFACE_07: User Interface - Source IP Address (to be sent)";
    Casename.dataLength = sizeof(Case);
    Casename.Data = (uint8 *) calloc(Casename.dataLength, sizeof(uint8));
    memcpy(Casename.Data, Case, Casename.dataLength);

    Data.dataLength = UDPConfiguration.UDPDefaultDataLen;
    Data.Data = (uint8 *) calloc(Data.dataLength, sizeof(uint8));
    strcpy(Data.Data, UDPConfiguration.UDPDefaultData);


    //start test
    TP_GenaralStartTest(GenaralStartTest_cb_USER);
    while (callback_Result_USER != 0) { sleep(1); }
    printf("\n[+]:Test UDP_USER_INTERFACE_08 was started\n");
    callback_Result_USER = 2;

    //Create socket and bind it
    TP_UdpCreateAndBind(UdpCreateAndBind_cb_USER, TRUE, UDPConfiguration.DUTUDPPort, ipv4DUT);
    while (callback_Result_USER != 0) { sleep(1); }
    callback_Result_USER = 2;
    printf("\n[+]:socket was successfully created and bound\n");

    //Set filter configuration to receive the data
    strcpy(filter.SrcAdd, UDPConfiguration.Dst_IP_addr);
    filter.Srcport = UDPConfiguration.DUTUDPPort;
    strcpy(filter.dstAdd, UDPConfiguration.Host_1_IP);
    filter.Dstport = UDPConfiguration.testerUDPPort;
    uint8_t timeout = 3;

    //push DUT to send data
    TP_UdpSendData(TP_UdpSendData_cb_USER, socketId_USER, 0, UDPConfiguration.testerUDPPort, ipv4Tester, Data);
    UDP_R = ListenUDP(filter, timeout);//Tester listen and receive the incoming data
    while (callback_Result_USER != 0) { sleep(1); }
    callback_Result_USER = 2;
    printf("\n[+]:DUT send data\n");

    //verify that tester well receive the packet
    if (UDP_R.length == 0) {
        printf("\n[+]:Error not UDP packet received\n");
        TP_GenaralEndTest(GenaralEndTest_cb_USER, 8, Casename);
        printf("Test case UDP_USER_INTERFACE_08 was ended with error n°1\n");
        test_Result = 1;

    } else {
        printf("\n[+]:Packet received \n");

        //print the Source IP @
        printf("\n[+]:Destination IP address equal to %s:\n", GetUDPField(&UDP_R, IP, IP_DstAddress));

        //Close socket
        TP_UdpCloseSocket(UdpCloseSocket_cb_USER, socketId_USER);
        while (callback_Result_USER != 0) { sleep(1); }

        printf("\n[+]:socket %d was closed \n", socketId_USER);

        //End Test
        TP_GenaralEndTest(GenaralEndTest_cb_USER, 8, Casename);
        test_Result = 0;
        printf("\n**********[+]:Test case UDP_USER_INTERFACE_08 was successfully ended***********\n");
    }

    return test_Result;
}
