#include <stdio.h>
#include <unistd.h>

//APIs and header
#include "AbstractionAPI.h"
#include "TestabilityProtocol_api.h"
#include "TestabilityProtocol_Intern.h"
#include "UDP_config.h"
#include "UDP_FIELDS.h"
#include "split.h"

extern UDP_config_t UDPConfiguration;

uint8 callback_Result_Fields;
uint16 socketId_Fields, socketid_Fields;
uint16 Len_Fields, Srcport_Fields;
ip4addr ipv4Addr_Fields;
vint8 UDP_Data_Fields;

void GenaralStartTest_cb_Fields(TP_ResultID_t Result) {
    callback_Result_Fields = Result;
}

void UdpCreateAndBind_cb_Fields(TP_ResultID_t Result, uint16 s) {
    callback_Result_Fields = Result;
    socketId_Fields = s;
}

void TP_UdpReceiveAndForward_cb_Fields(TP_ResultID_t Result, uint16 s) {
    callback_Result_Fields = Result;
    socketid_Fields = s;
}

void TP_Udp_ReceiveAndForward_even_cb_Fields(TP_ResultID_t Result, uint16 Length, uint16 Port, ip4addr Address,
                                             vint8 DataUdp) {
    callback_Result_Fields = Result;
    Len_Fields = Length;
    Srcport_Fields = Port;
    ipv4Addr_Fields = Address;
    UDP_Data_Fields = DataUdp;
    printf("\n[+]:************ %d", Length);
}

void TP_UdpSendData_cb_Fields(TP_ResultID_t Result) {
    callback_Result_Fields = Result;
}

void UdpCloseSocket_cb_Fields(TP_ResultID_t Result) {
    callback_Result_Fields = Result;
}

void GenaralEndTest_cb_Fields(TP_ResultID_t Result) {
    callback_Result_Fields = Result;
}


int UDP_FIELDS_01() {
    callback_Result_Fields = 2;
    uint8_t test_Result = 2;
    ip4addr ipv4DUT, ipv4Tester;


    ipv4DUT.dataLength = 4;
    extractIpAddress(UDPConfiguration.Dst_IP_addr, &ipv4DUT);

    ipv4Tester.dataLength = 4;
    extractIpAddress(UDPConfiguration.Host_1_IP, &ipv4Tester);


    UDP_Packet UDP_R;
    Packet_filter filter;
    vint8 Casename, Data;

    uint8 Case[] = "UDP_FIELDS_01: Fields–Specify Source Port";
    Casename.dataLength = sizeof(Case);
    Casename.Data = (uint8 *) calloc(Casename.dataLength, sizeof(uint8));
    memcpy(Casename.Data, Case, Casename.dataLength);

    uint8 data[] = "From DUT to Listen tester";
    Data.dataLength = sizeof(data);
    Data.Data = (uint8 *) calloc(Data.dataLength, sizeof(uint8));
    memcpy(Data.Data, data, sizeof(data));

    //start test
    TP_GenaralStartTest(GenaralStartTest_cb_Fields);
    while (callback_Result_Fields != 0) { sleep(1); }
    printf("\n[+]:Test UDP_FIELDS_01 was started\n");
    callback_Result_Fields = 2;

    //Create socket and bind it
    TP_UdpCreateAndBind(UdpCreateAndBind_cb_Fields, TRUE, UDPConfiguration.unusedUDPDstPort1, ipv4DUT);
    while (callback_Result_Fields != 0) { sleep(1); }
    callback_Result_Fields = 2;
    printf("\n[+]:socket was successfully created and bound\n");

    //set the filter configuration to receive the packet from DUT
    strcpy(filter.SrcAdd, UDPConfiguration.Dst_IP_addr);
    filter.Srcport = UDPConfiguration.unusedUDPDstPort1;
    strcpy(filter.dstAdd, UDPConfiguration.Host_1_IP);
    filter.Dstport = UDPConfiguration.testerUDPPort;
    uint8_t timeout = 3;

    //DUT sending data
    TP_UdpSendData(TP_UdpSendData_cb_Fields, socketId_Fields, 0, UDPConfiguration.testerUDPPort, ipv4Tester, Data);
    UDP_R = ListenUDP(filter, timeout);//Tester listen and receive the  incoming data from DUT
    while (callback_Result_Fields != 0) { sleep(1); }
    callback_Result_Fields = 2;
    printf("\n[+]:DUT send data\n");

    //Verify that tester receive the packet
    if (UDP_R.length == 0) {
        printf("\n[+]:Error not udp packet received\n");
        TP_GenaralEndTest(GenaralEndTest_cb_Fields, 1, Casename);
        printf("Test case UDP_FIELDS_01 was ended with error n°1\n");
    } else {
        printf("\n[+]:Packet received \n");
        printf("\n[+]:UDP source Port %ld:\n", GetUDPField(&UDP_R, UDP, UDP_SrcPort));

        //Close socket
        TP_UdpCloseSocket(UdpCloseSocket_cb_Fields, socketId_Fields);
        while (callback_Result_Fields != 0) { sleep(1); }
        printf("\n[+]:socket %d was closed \n", socketId_Fields);

        //End Test
        TP_GenaralEndTest(GenaralEndTest_cb_Fields, 1, Casename);
        test_Result = 0;
        printf("\n*************[+]:Test case UDP_FIELDS_01 was successfully ended***********\n");


    }
    return test_Result;
}

int UDP_FIELDS_02() {
    uint8_t test_Result = 2;
    callback_Result_Fields = 2;
    ip4addr ipv4DUT, ipv4Tester;
    Packet_filter filter;

    ipv4DUT.dataLength = 4;
    extractIpAddress(UDPConfiguration.Dst_IP_addr, &ipv4DUT);

    ipv4Tester.dataLength = 4;
    extractIpAddress(UDPConfiguration.Host_1_IP, &ipv4Tester);

    UDP_Packet UDP_S, UDP_R;
    vint8 Casename, UDPDefaultData;

    uint8 Case[] = "UDP_FIELDS_02: Fields – Specify Destination Port";
    Casename.dataLength = sizeof(Case);
    Casename.Data = (uint8 *) calloc(Casename.dataLength, sizeof(uint8));
    memcpy(Casename.Data, Case, Casename.dataLength);

    UDPDefaultData.dataLength = UDPConfiguration.UDPDefaultDataLen;
    UDPDefaultData.Data = (uint8 *) calloc(UDPDefaultData.dataLength, sizeof(uint8));
    //start Test

    TP_GenaralStartTest(GenaralStartTest_cb_Fields);
    while (callback_Result_Fields != 0) { sleep(1); }
    printf("\n[+]:Test UDP_FIELDS_02 was started\n");
    callback_Result_Fields = 2;

    //Create and bind of the socket
    TP_UdpCreateAndBind(UdpCreateAndBind_cb_Fields, TRUE, UDPConfiguration.unusedUDPDstPort1, ipv4DUT);
    while (callback_Result_Fields != 0) { sleep(1); }
    printf("\n[+]:Socket was successfully create and bound\n");
    callback_Result_Fields = 2;

    //create a UDP packet
    printf("[+]:Creation of UDP packet\n");
    UDP_S = CreateUDP();
    if (UDP_S.length == 0) {
        printf("Udp packet creation error\n");
        TP_GenaralEndTest(GenaralEndTest_cb_Fields, 5, Casename);
        printf("Test case UDP_FIELDS_02 end with error n°1");
        test_Result = 1;
    } else {
        printf("\n[+]:udp packet was successfully created\n");

        //set Payload data
        EditUDPField(&UDP_S, PAYLAOD, PAYLOAD_data, (void *) UDPConfiguration.UDPDefaultData);

        //set Payload length
        EditUDPField(&UDP_S, PAYLAOD, PAYLOAD_length, (void *) UDPConfiguration.UDPDefaultDataLen);

        //set UDP src port
        EditUDPField(&UDP_S, UDP, UDP_SrcPort, (void *) UDPConfiguration.unusedUDPSrcPort);

        //Set UDP Dest port
        EditUDPField(&UDP_S, UDP, UDP_DstPort, (void *) UDPConfiguration.unusedUDPDstPort1);

        //Set UDP header length
        EditUDPField(&UDP_S, UDP, UDP_Length, (void *) (UDPConfiguration.UDPDefaultDataLen + 8));

        //Edit IP header length
        UDP_Compute_checksum(&UDP_S);

        //Set DUT to receive packet
        TP_UdpReceiveAndForward(TP_UdpReceiveAndForward_cb_Fields, TP_Udp_ReceiveAndForward_even_cb_Fields,
                                socketId_Fields, 8, UDPConfiguration.UDPDefaultDataLen);
        while (callback_Result_Fields != 0) { sleep(1); }
        printf("\n[+]:DUT is ready to receive and forward Data\n");
        callback_Result_Fields = 2;

        //Send data to DUT
        if (SendUDP(UDP_S) != 1) {
            printf("Sending error");
            TP_GenaralEndTest(GenaralEndTest_cb_Fields, 5, Casename);
            printf("\nTest case UDP_FIELDS_02 end with error n°3\n");
            test_Result = 1;
        } else {
            printf("\n[+]:Packet delivered!\n");

            strcpy(UDPDefaultData.Data, UDPConfiguration.UDPDefaultData);

            //set filter configuration to receive the packet from DUT
            strcpy(filter.SrcAdd, UDPConfiguration.Dst_IP_addr);
            filter.Srcport = UDPConfiguration.unusedUDPDstPort1;
            strcpy(filter.dstAdd, UDPConfiguration.Host_1_IP);
            filter.Dstport = UDPConfiguration.unusedUDPSrcPort;
            uint8_t timeout = 3;

            //DUT send the packet
            TP_UdpSendData(TP_UdpSendData_cb_Fields, socketId_Fields, 0, UDPConfiguration.unusedUDPSrcPort, ipv4Tester,
                           UDPDefaultData);
            UDP_R = ListenUDP(filter, timeout);//Tester listen and receive the incoming data from DUT
            while (callback_Result_Fields != 0) { sleep(1); }

            printf("\n[+]:DUT is ready to receive and forward Data\n");
            callback_Result_Fields = 2;

            //Verify that tester received the udp packet
            if (UDP_R.length == 0) {
                printf("\n[+]:Error ===>DUT doesn't respond\n");
                TP_GenaralEndTest(GenaralEndTest_cb_Fields, 2, Casename);
                printf("\n[+]:Test end with error n°4\n");
                test_Result = 1;

            } else {
                printf("\n[+]:Tester receive a data from DUT\n");

                //display Destination port
                printf("\n[+]: UDP destination port is : %ld", GetUDPField(&UDP_R, UDP, UDP_DstPort));

                //Close socket
                TP_UdpCloseSocket(UdpCloseSocket_cb_Fields, socketId_Fields);
                while (callback_Result_Fields != 0) { sleep(1); }

                printf("\n[+]:socket %d  was closed\n", socketId_Fields);
                callback_Result_Fields = 2;

                //Ending test
                TP_GenaralEndTest(GenaralEndTest_cb_Fields, 2, Casename);
                test_Result = 0;
                printf("\n*************[+]:test case UDP_FIELDS_02 ended ***************\n");


            }
        }
    }

    return test_Result;

}

int UDP_FIELDS_03() {
    callback_Result_Fields = 2;
    uint8_t test_Result = 1;
    ip4addr ipv4DUT;
    ipv4DUT.dataLength = 4;
    extractIpAddress(UDPConfiguration.Dst_IP_addr, &ipv4DUT);

    UDP_Packet UDP_S;
    vint8 Casename;

    uint8 Case[] = "UDP_FIELDS_03 :Fields - Accept Source Port set to zero";
    Casename.dataLength = sizeof(Case);
    Casename.Data = (uint8 *) calloc(Casename.dataLength, sizeof(uint8));
    memcpy(Casename.Data, Case, Casename.dataLength);


    //Start Test
    TP_GenaralStartTest(GenaralStartTest_cb_Fields);
    while (callback_Result_Fields != 0) { sleep(1); }

    printf("[+]:Test UDP_FIELDS_03 was started\n");
    callback_Result_Fields = 2;

    //Create and bind the socket
    TP_UdpCreateAndBind(UdpCreateAndBind_cb_Fields, TRUE, UDPConfiguration.unusedUDPDstPort1, ipv4DUT);
    while (callback_Result_Fields != 0) { sleep(1); }

    printf("\n[+]:Socket was created and bound successfully\n");
    callback_Result_Fields = 2;

    //Create UDP packet
    UDP_S = CreateUDP();
    if (UDP_S.length == 0) {
        printf("\n[+]:Udp packet creation error\n");
        TP_GenaralEndTest(GenaralEndTest_cb_Fields, 3, Casename);
        printf("\n[+]:Test case UDP_FIELDS_03 end with error n°1\n");
        test_Result = 1;
    } else {
        printf("\n[+]:UDP packet was successfully created\n");

        //set Payload data
        EditUDPField(&UDP_S, PAYLAOD, PAYLOAD_data, (void *) UDPConfiguration.UDPDefaultData);

        //set payload length
        EditUDPField(&UDP_S, PAYLAOD, PAYLOAD_length, (void *) UDPConfiguration.UDPDefaultDataLen);

        //Set Source port to 0
        EditUDPField(&UDP_S, UDP, UDP_SrcPort, (void *) 0);

        //Set UDP dest port to unusedDst port
        EditUDPField(&UDP_S, UDP, UDP_DstPort, (void *) UDPConfiguration.unusedUDPDstPort1);

        //set UDP header length
        EditUDPField(&UDP_S, UDP, UDP_Length, (void *) (UDPConfiguration.UDPDefaultDataLen + 8));

        //Compute packet checksums
        UDP_Compute_checksum(&UDP_S);

        //Prepare the DUT to receive the socket
        TP_UdpReceiveAndForward(TP_UdpReceiveAndForward_cb_Fields, TP_Udp_ReceiveAndForward_even_cb_Fields,
                                socketId_Fields, 8, UDPConfiguration.UDPDefaultDataLen);
        while (callback_Result_Fields != 0) { sleep(1); }

        printf("\n[+]:DUT is ready to receive and forward data\n");
        callback_Result_Fields = 2;

        //Send the UDP packet
        if (SendUDP(UDP_S) != 1) {
            printf("\n[+]:error on send the udp packet");
            TP_GenaralEndTest(GenaralEndTest_cb_Fields, 3, Casename);
            printf("\n[+]:Test case UDP_FIELDS_03 end with error n°4");
            test_Result = 1;
        } else {
            printf("\n[+]:Packet delivered\n");


            //Close socket
            sleep(1);
            TP_UdpCloseSocket(UdpCloseSocket_cb_Fields, socketId_Fields);
            while (callback_Result_Fields != 0) { sleep(1); }

            printf("\n[+]:socket %d was closed\n", socketId_Fields);

            //ENd test
            TP_GenaralEndTest(GenaralEndTest_cb_Fields, 3, Casename);
            test_Result = 0;
            printf("\n*************[+]:Test case UDP_FIELDS_03 was successfully ended***************\n");

        }
    }
    return test_Result;
}

int UDP_FIELDS_04() {/*
    callback_Result_Fields=2;
    ip4addr ipv4DUT,ipv4Tester1,ipv4Tester2;
    //DUT ip address
    ipv4DUT.dataLength= 4;
    ipv4DUT.Data[0]=192;
    ipv4DUT.Data[1] = 168;
    ipv4DUT.Data[2]=1;
    ipv4DUT.Data[3]=18;

    //tester1 ip address
    ipv4Tester1.dataLength=4;
    ipv4Tester1.Data[0]=192;
    ipv4Tester1.Data[1]=168;
    ipv4Tester1.Data[2]=1;
    ipv4Tester1.Data[3]=14;

    //tester2 ip address
    ipv4Tester2.dataLength=4;
    ipv4Tester2.Data[0]=192;
    ipv4Tester2.Data[1]=168;
    ipv4Tester2.Data[2]=1;
    ipv4Tester2.Data[3]=10;


    UDP_Packet UDP_R1,UDP_R2;
    Packet_filter filter;
    vint8 Message,Casename;


    uint8 Case[]="UDP_FIELDS_04:Fields - Same Destination Port with Different IP Address (send)";
    Casename.dataLength= sizeof(Case);
    Casename.Data=(uint8*) calloc(Casename.dataLength,sizeof(uint8));
    memcpy(Casename.Data,Case,Casename.dataLength);

    uint8 UdpData[]="From DUT to Tester UDP_FIELDS Test Case";
    Message.dataLength= sizeof(UdpData);
    Message.Data=(uint8*) calloc(sizeof(UdpData), sizeof(uint8));
    memcpy(Message.Data,UdpData, sizeof(UdpData));

    //start the test
    TP_GenaralStartTest(GenaralStartTest_cb_Fields);
    while(callback_Result_Fields!=0){ sleep(1;}
    if(callback_Result_Fields==1){
        printf("\n[+]:Error at GenaralStartTest\n");
        TP_GenaralEndTest(GenaralEndTest_cb_Fields,4,Casename);
    }
    else {
        callback_Result_Fields = 2;
        printf("\n[+]:Test UDP_FIELDS_04 was started\n");
        //Create and bind the socketK

        TP_UdpCreateAndBind(UdpCreateAndBind_cb_Fields, TRUE, UDPPort, ipv4DUT);
        while(callback_Result_Fields!=0){ sleep(1;}
        if(callback_Result_Fields==1){
            printf("\n[+]:Error at UdpCreateAndBind\n");
            TP_GenaralEndTest(GenaralEndTest_cb_Fields,4,Casename);
        }
        else {
            callback_Result_Fields = 2;
            printf("\n[+]:Socket was successfully created and bound\n");

            //Listen for incoming data
            strcpy(filter.SrcAdd,UDPConfiguration.Dst_IP_addr);
            filter.Srcport = UDPConfiguration.unusedUDPDstPort1;
            strcpy(filter.dstAdd,UDPConfiguration.Host_1_IP);
            filter.Dstport = testerU;
            int timeout = 3;
            //send the data
            TP_UdpSendData(TP_UdpSendData_cb_Fields, socketId_Fields, 0, UDPConfiguration.testerUDPPort, ipv4Tester1, Message);
            UDP_R1 = ListenUDP(filter, timeout);
            while(callback_Result_Fields!=0){ sleep(1;}
            if(callback_Result_Fields==1){
                printf("\n[+]:Error at UdpSendData\n");
                TP_GenaralEndTest(GenaralEndTest_cb_Fields,4,Casename);
            }
            else {
                callback_Result_Fields = 2;
                printf("\n[+]:DUT send the udp data\n");


                if (UDP_R1.length == 0) {
                    printf("\n[+]:Error DUT doesn't respond\n");
                    TP_GenaralEndTest(GenaralEndTest_cb_Fields, 4, Casename);
                    printf("\n[+]:Test case UDP_FIELDS_04 end with error n°1\n");
                }
                else {

                    printf("\n[+]:Destination ip address equal to %s\n", GetUDPField(&UDP_R1, IP, IP_DstAddress));
                    printf("\n[+]:UDP Destination port equal to %d :\n", GetUDPField(&UDP_R1, UDP, UDP_DstPort));

                    //send data to tester 2

                    //listen for incoming data
                    strcpy(filter.SrcAdd,UDPConfiguration.Dst_IP_addr);
                    filter.Srcport = UDPConfiguration.unusedUDPDstPort1;
                    filter.dstAdd = "192.168.1.10";
                    filter.Dstport = testerU;
                    int timeout=3;

                    TP_UdpSendData(TP_UdpSendData_cb_Fields, socketId_Fields, 0, UDPConfiguration.testerUDPPort, ipv4Tester2, Message);
                    UDP_R2 = ListenUDP(filter,timeout);
                    while(callback_Result_Fields!=0){ sleep(1;}
                    if(callback_Result_Fields==1){
                        printf("\n[+]:Error at UdpSendData2\n");
                        TP_GenaralEndTest(GenaralEndTest_cb_Fields,4,Casename);
                    }
                    else {
                        callback_Result_Fields = 2;
                        printf("\n[+]:DUT sent Data\n");
                        if (UDP_R2.length == 0) {
                            printf("\n[+]:Error DUT doesn't respond\n");

                            TP_GenaralEndTest(GenaralEndTest_cb_Fields, 4, Casename);
                            printf("\n[+]:Test case UDP_FIELDS_04 end with error n°2\n");
                        }
                        else {
                            printf("\n[+] Destination ip address equal to %s", GetUDPField(&UDP_R2, IP, IP_DstAddress));
                            printf("\n[+]:UDP dest port equal to %d", GetUDPField(&UDP_R2, UDP, UDP_DstPort));

                            //close the socket
                            TP_UdpCloseSocket(UdpCloseSocket_cb_Fields, socketId_Fields);
                            while(callback_Result_Fields!=0){ sleep(1;}
                            if(callback_Result_Fields==1){
                                printf("\n[+]:Error at UdpSendData\n");
                                TP_GenaralEndTest(GenaralEndTest_cb_Fields,4,Casename);
                            }
                            else {
                                callback_Result_Fields = 2;
                                printf("\n[+]:socket id %d was closed \n", socketId_Fields);
                                //End test
                                TP_GenaralEndTest(GenaralEndTest_cb_Fields,4, Casename);
                                printf("\n[+]:Test case UDP_FIELDS_04 successfully end \n");
                                //close the control channel
                                //TP_CloseControlChannel();
                                printf("\n[+]:control channel closed\n");
                            }
                        }
                    }
                }
            }
        }
    }*/
    return 2;
}

int UDP_FIELDS_05() {

    uint8_t test_Result = 1;/*
    callback_Result_Fields=2;
    ip4addr ipv4DUT, ipv4Tester1, ipv4Tester2;
    //DUT ip address
    ipv4DUT.dataLength = 4;
    extractIpAddress(UDPConfiguration.Dst_IP_addr,&ipv4DUT);


    //tester1 ip address
    ipv4Tester1.dataLength = 4;
    extractIpAddress(UDPConfiguration.Host_1_IP,&ipv4Tester1);


    //tester2 ip address
    ipv4Tester2.dataLength = 4;
    extractIpAddress(UDPConfiguration.Host_2_IP,&ipv4Tester2);


    UDP_Packet UDP_S1,UDP_S2;
    vint8 Casename;
    uint8 *UDPData;

    uint8 Case[]= "UDP_FIELDS_05: Fields - Same Port with Different IP Address (receive and send)";
    Casename.dataLength = sizeof(Case);
    Casename.Data=(uint8*) calloc(Casename.dataLength,sizeof(uint8));
    memcpy(Casename.Data,Case,Casename.dataLength);

    UDPData= (uint8*)calloc(UDPConfiguration.UDPDataLen,sizeof(uint8));
    memcpy(UDPData,UDPConfiguration.UDPData,UDPConfiguration.UDPDataLen);
    //start the test

    TP_GenaralStartTest(GenaralStartTest_cb_Fields);
    while(callback_Result_Fields!=0){ sleep(1;}
    if(callback_Result_Fields==1){
        printf("\n[+]:Error at GenaralStartTest\n");
        TP_GenaralEndTest(GenaralEndTest_cb_Fields,5,Casename);
    }
    else {
        callback_Result_Fields = 2;
        printf("Test UDP_FIELDS_05 was started\n");
        //Create and bind the socket
        TP_UdpCreateAndBind(UdpCreateAndBind_cb_Fields, TRUE, UDPConfiguration.unusedUDPDstPort1, ipv4DUT);
        while (callback_Result_Fields !=0) { sleep(1; }
        if (callback_Result_Fields == 1) {
            printf("\n[+]:Error at UdpCreateAndBind\n");
            TP_GenaralEndTest(GenaralEndTest_cb_Fields,5, Casename);
        } else {
            callback_Result_Fields = 2;
            printf("Socket was successfully created and bound\n");

            //create the packet
            UDP_S1 = CreateUDP();
            if (UDP_S1.length == 0) {
                printf("Udp packet creation error\n");
                TP_GenaralEndTest(GenaralEndTest_cb_Fields,5, Casename);
                printf("Test case UDP_FIELDS_05 end with error n°1\n");
            } else {
                printf("Udp packet was successfully created \n");
                //edit the fields before send the packet
                EditUDPField(&UDP_S1, PAYLAOD, PAYLOAD_data, (void *) UDPConfiguration.UDPData);
                printf("\n[+]:Payload data modified\n");

                EditUDPField(&UDP_S1, PAYLAOD, PAYLOAD_length, (void *) UDPConfiguration.UDPDataLen);
                printf("\n[+]: Payload length modified \n");

                EditUDPField(&UDP_S1, UDP, UDP_SrcPort, (void *) UDPConfiguration.testerUDPPort);
                printf("\n[+]; Source port modified\n");

                EditUDPField(&UDP_S1, UDP, UDP_DstPort, (void *) UDPConfiguration.unusedUDPDstPort1);
                printf("\n[+]:UDP Dst port successfully set\n");

                EditUDPField(&UDP_S1, UDP, UDP_Length, (void *) (UDPConfiguration.UDPDataLen+8));
                printf("\n[+]:UPD length modified");

                EditUDPField(&UDP_S1, IP, IP_SrcAddress, (void *) UDPConfiguration.Host_1_IP);
                printf("\n[+]:Source ip address successfully set\n");

                EditUDPField(&UDP_S1, IP, IP_DstAddress, (void *) UDPConfiguration.Dst_IP_addr);
                printf("\n[+]:Destination ip address successfully set\n");

                UDP_Compute_checksum(&UDP_S1) ;

                TP_UdpReceiveAndForward(TP_UdpReceiveAndForward_cb_Fields, TP_Udp_ReceiveAndForward_even_cb_Fields, socketId_Fields, 8,UDPConfiguration.UDPDataLen);
                while (callback_Result_Fields !=0) { sleep(1; }
                if (callback_Result_Fields == 1) {
                    printf("\n[+]:Error at GenaralStartTest\n");
                    TP_GenaralEndTest(GenaralEndTest_cb_Fields,5, Casename);
                } else {
                    callback_Result_Fields = 2;
                    printf("DUT is ready to receive and forward data\n");
                    //send data to DUT
                    if (SendUDP(UDP_S1) !=1) {
                        printf("Sending error");
                        TP_GenaralEndTest(GenaralEndTest_cb_Fields,5, Casename);
                        printf("Test case UDP_FIELDS_05 end with error n°7\n");
                    } else {
                        printf("Udp packet was delivered\n");
                        UDP_S2 = CreateUDP();
                        if (UDP_S2.length == 0) {
                            printf("Udp packet creation error\n");
                            TP_GenaralEndTest(GenaralEndTest_cb_Fields, 5, Casename);
                            printf("Test case UDP_FIELDS_05 end with error n°8\n");
                        } else {
                            //edit the fields before send the packet
                            EditUDPField(&UDP_S2,PAYLAOD,PAYLOAD_data,(void*)UDPConfiguration.UDPDefaultData);
                            EditUDPField(&UDP_S2, PAYLAOD, PAYLOAD_length, (void *) UDPConfiguration.UDPDefaultDataLen);

                            EditUDPField(&UDP_S2, UDP, UDP_SrcPort, (void *) UDPConfiguration.testerUDPPort);

                            EditUDPField(&UDP_S2, UDP, UDP_DstPort, (void *) UDPConfiguration.unusedUDPDstPort1);
                            printf("\n[+]:UDP dst port modified\n");

                            EditUDPField(&UDP_S2, UDP, UDP_Length, (void *) (UDPConfiguration.UDPDefaultDataLen+8));

                            EditUDPField(&UDP_S2, IP, IP_SrcAddress, (void *) UDPConfiguration.Host_2_IP);
                            printf("\n[+]:Source address ip successfully set\n");

                            EditUDPField(&UDP_S2, IP, IP_DstAddress, (void *) UDPConfiguration.Dst_IP_addr);
                            printf("\n[+]:IP Dest address modified\n");

                            UDP_Compute_checksum(&UDP_S2);

                            TP_UdpReceiveAndForward(TP_UdpReceiveAndForward_cb_Fields, TP_Udp_ReceiveAndForward_even_cb_Fields,socketId_Fields, 8,UDPConfiguration.UDPDefaultDataLen);
                            while (callback_Result_Fields !=0) { sleep(1; }
                            if (callback_Result_Fields == 1) {
                                printf("\n[+]:Error at GenaralStartTest\n");
                                TP_GenaralEndTest(GenaralEndTest_cb_Fields, 5, Casename);
                            } else {
                                callback_Result_Fields = 2;
                                printf("\n[+]:DUT is ready to receive and forward data\n");

                                //send data to DUT
                                if (SendUDP(UDP_S2) !=1) {
                                    printf("Sending error");
                                    TP_GenaralEndTest(GenaralEndTest_cb_Fields, 5, Casename);
                                    printf("Test case UDP_FIELDS_05 end with error n°14\n");
                                }
                                else {
                                    printf("\n[+]:Packet delivered\n");
                                    //close socket
                                    TP_UdpCloseSocket(UdpCloseSocket_cb_Fields, socketId_Fields);
                                    while (callback_Result_Fields !=0) { sleep(1; }
                                    if (callback_Result_Fields == 1) {
                                        printf("\n[+]:Error at UdpCloseSocket\n");
                                        TP_GenaralEndTest(GenaralEndTest_cb_Fields, 5, Casename);
                                    } else {
                                        callback_Result_Fields = 2;
                                        printf("socket id %d closed", socketId_Fields);
                                        //end test
                                        TP_GenaralEndTest(GenaralEndTest_cb_Fields, 5, Casename);
                                        test_Result=0;
                                        printf("*******\n[+]:Test case UDP_FIELDS_05 was successfully ended********* \n");
                                    }
                                }

                            }
                        }
                    }
                }
            }
        }
    }*/
    return test_Result;
}

int UDP_FIELDS_06() {

    callback_Result_Fields = 2;
    uint8_t test_Result = 1;
    ip4addr ipv4DUT, ipv4Tester;
    ipv4DUT.dataLength = 4;
    extractIpAddress(UDPConfiguration.Dst_IP_addr, &ipv4DUT);


    ipv4Tester.dataLength = 4;
    extractIpAddress(UDPConfiguration.Host_1_IP, &ipv4Tester);

    UDP_Packet UDP_R;
    Packet_filter filter;


    vint8 udpUserData;
    vint8 Casename;
    uint8 Case[] = "UDP_FIELDS_F6 :Fields - Total Length";
    Casename.dataLength = sizeof(Case);
    Casename.Data = (uint8 *) calloc(Casename.dataLength, sizeof(uint8));
    memcpy(Casename.Data, Case, Casename.dataLength);

    //Set data for DUT to set it
    udpUserData.dataLength = UDPConfiguration.udpUserDataSize;
    udpUserData.Data = (uint8 *) calloc(udpUserData.dataLength, sizeof(uint8));

    //Start Test
    TP_GenaralStartTest(GenaralStartTest_cb_Fields);
    printf("\n[+]:Test UDP_FIELDS_06 was started\n");

    while (callback_Result_Fields != 0) { sleep(1); }

    callback_Result_Fields = 2;
    printf("\n[+]:Test was started\n");

    //Create and bind the socket
    TP_UdpCreateAndBind(UdpCreateAndBind_cb_Fields, TRUE, UDPConfiguration.DUTUDPPort, ipv4DUT);
    while (callback_Result_Fields != 0) { sleep(1); }

    callback_Result_Fields = 2;

    //set udp configuration to receive the packet from DUT
    printf("\n[+]:Socket was successfully created and bound \n");
    strcpy(filter.SrcAdd, UDPConfiguration.Dst_IP_addr);
    filter.Srcport = UDPConfiguration.DUTUDPPort;
    strcpy(filter.dstAdd, UDPConfiguration.Host_1_IP);
    filter.Dstport = UDPConfiguration.testerUDPPort;
    uint16 timeout = 3;

    //push DUT Send data
    TP_UdpSendData(TP_UdpSendData_cb_Fields, socketId_Fields, 0, UDPConfiguration.testerUDPPort, ipv4Tester,
                   udpUserData);
    UDP_R = ListenUDP(filter, timeout);//Tester listen and receive the incoming data from DUT
    while (callback_Result_Fields != 0) { sleep(3); }

    callback_Result_Fields = 2;
    printf("\n[+]:DUT was sent the data\n");

    //verify that tester receive data from DUT
    if (UDP_R.length == 0) {
        printf("\n[+]:DUT doesn't respond\n");
        TP_GenaralEndTest(GenaralEndTest_cb_Fields, 6, Casename);
        printf("\n[+]:Test case UDP_FIELDS_06 end with error n°2\n");
        test_Result = 1;
    } else {
        printf("\n[+]:Tester receive packet\n");

        //Verify UDP header length
        if (GetUDPField(&UDP_R, UDP, UDP_Length) != 109) {
            printf("\n[+]:UDP_length different to 109");
            TP_GenaralEndTest(GenaralEndTest_cb_Fields, 6, Casename);
            printf("\n[+]:Test case UDP_FIELDS_06 was ended with fail\n");
            test_Result = 1;

        } else {
            printf("\n[+]:Udp_Header is equal to 109\n");

            //close socket
            TP_UdpCloseSocket(UdpCloseSocket_cb_Fields, socketId_Fields);
            while (callback_Result_Fields != 0) { sleep(1); }

            callback_Result_Fields = 2;
            printf("\n[+]:socket id %d was closed \n", socketId_Fields);

            //Close Test
            TP_GenaralEndTest(GenaralEndTest_cb_Fields, 6, Casename);
            test_Result = 0;
            printf("\n***********[+]:Test case UDP_FIELDS_06 was successfully ended **************\n");

        }
    }

    return test_Result;
}

int UDP_FIELDS_07() {
    callback_Result_Fields = 2;
    uint8_t test_Result = 1;

    ip4addr ipv4DUT, ipv4Tester;
    ipv4DUT.dataLength = 4;
    extractIpAddress(UDPConfiguration.Dst_IP_addr, &ipv4DUT);


    ipv4Tester.dataLength = 4;
    extractIpAddress(UDPConfiguration.Host_1_IP, &ipv4Tester);


    UDP_Packet UDP_R;
    Packet_filter filter;

    vint8 data;
    uint8 Data[] = "";
    data.dataLength = 0;
    data.Data = (uint8 *) calloc(0, sizeof(uint8));
    memcpy(data.Data, Data, 0);

    vint8 Casename;

    uint8 Case[] = "UDP_FIELDS_07: Fields - Total Length (no data) ";
    Casename.dataLength = sizeof(Case);
    Casename.Data = (uint8 *) calloc(Casename.dataLength, sizeof(uint8));
    memcpy(Casename.Data, Case, Casename.dataLength);

    //Start Test
    TP_GenaralStartTest(GenaralStartTest_cb_Fields);
    while (callback_Result_Fields != 0) { sleep(1); }

    callback_Result_Fields = 2;
    printf("\n[+]:Test UDP_FIELDS_07 was started\n");

    //Create socket and bind the socket
    TP_UdpCreateAndBind(UdpCreateAndBind_cb_Fields, TRUE, UDPConfiguration.DUTUDPPort, ipv4DUT);
    while (callback_Result_Fields != 0) { sleep(1); }

    callback_Result_Fields = 2;
    printf("\n[+]:Socket was successfully created and bound\n");

    //set the filter configuration to receive the packet from DUT
    filter.Srcport = UDPConfiguration.DUTUDPPort;
    strcpy(filter.SrcAdd, UDPConfiguration.Dst_IP_addr);
    filter.Dstport = UDPConfiguration.testerUDPPort;
    strcpy(filter.dstAdd, UDPConfiguration.Host_1_IP);
    uint8_t timeout = 3;

    //Dut send data to tester
    TP_UdpSendData(TP_UdpSendData_cb_Fields, socketId_Fields, 0, UDPConfiguration.testerUDPPort, ipv4Tester, data);
    UDP_R = ListenUDP(filter, timeout);//Tester listen and receive the incoming data from DUT
    while (callback_Result_Fields != 0) { sleep(1); }
    callback_Result_Fields = 2;
    printf("\nDUT sent data\n");

    //verify that tester receive the coming data from DUT
    if (UDP_R.length == 0) {
        printf("\n[+]:Error ===>DUT doesn't respond\n");
        TP_GenaralEndTest(GenaralEndTest_cb_Fields, 7, Casename);
        printf("\n[+]:Test case UDP_FIELDS_07 end with error n°1\n");
        test_Result = 1;
    } else {
        printf("\n[+]:Tester receive the UDP data\n");

        //verify that UDP header length equal to 8
        if ((GetUDPField(&UDP_R, UDP, UDP_Length)) != 8) {
            printf("\n[+]:Header length not equal to 8");
            TP_GenaralEndTest(GenaralEndTest_cb_Fields, 7, Casename);
            printf("\n[+]:Test case UDP_FIELDS_07  end with error n°2\n");
            test_Result = 1;
        } else {
            printf("\n[+]:Header length equal to 8 !\n");

            //Close the Socket
            TP_UdpCloseSocket(UdpCloseSocket_cb_Fields, socketId_Fields);
            while (callback_Result_Fields != 0) { sleep(1); }

            callback_Result_Fields = 2;
            printf("\n[+]:socket id %d was closed \n", socketId_Fields);

            //Ending the test
            TP_GenaralEndTest(GenaralEndTest_cb_Fields, 7, Casename);
            test_Result = 0;
            printf("\n***********[+]:Test case UDP_FIELDS_07  was successfully ended ****************\n");
        }
    }
    return test_Result;
}

int UDP_FIELDS_08() {
    callback_Result_Fields = 2;
    uint8_t test_Result = 1;
    ip4addr ipv4DUT;
    ipv4DUT.dataLength = 4;
    extractIpAddress(UDPConfiguration.Dst_IP_addr, &ipv4DUT);

    UDP_Packet UDP_S;

    vint8 Casename;
    uint8 *Data = "";
    uint8 Case[] = "UDP_FIELDS_08:Fields - Total Length (less than 8 bytes)";
    Casename.dataLength = sizeof(Case);
    Casename.Data = (uint8 *) calloc(Casename.dataLength, sizeof(uint8));
    memcpy(Casename.Data, Case, Casename.dataLength);

    //Start Test
    TP_GenaralStartTest(GenaralStartTest_cb_Fields);
    while (callback_Result_Fields != 0) { sleep(1); }

    callback_Result_Fields = 2;
    printf("\n[+]Test UDP_FIELDS_08 was started\n");

    //Create and bind the socket for DUT
    TP_UdpCreateAndBind(UdpCreateAndBind_cb_Fields, TRUE, UDPConfiguration.unusedUDPDstPort1, ipv4DUT);
    while (callback_Result_Fields != 0) { sleep(1); }

    callback_Result_Fields = 2;
    printf("\n[+]:socket was successfully created and bound\n");

    //udp packet Creation
    printf("\n[+]:Creation of UDP packet!\n");
    UDP_S = CreateUDP();
    if (UDP_S.length == 0) {
        printf("\n[+]:udp packet creation error\n");
        TP_GenaralEndTest(GenaralEndTest_cb_Fields, 8, Casename);
        printf("\n[+]:Test case UDP_FIELDS_08 end with error n°1\n");
        test_Result = 1;

    } else {
        printf("\n[+]:packet was created\n");

        //set Payload data
        EditUDPField(&UDP_S, PAYLAOD, PAYLOAD_data, (void *) &Data);

        //set payload length
        EditUDPField(&UDP_S, PAYLAOD, PAYLOAD_length, (void *) 0);

        //set UDP_length
        EditUDPField(&UDP_S, UDP, UDP_Length, (void *) 5);

        //set UDP Dst port
        EditUDPField(&UDP_S, UDP, UDP_SrcPort, (void *) UDPConfiguration.testerUDPPort);

        //Set UDP Dst port
        EditUDPField(&UDP_S, UDP, UDP_DstPort, (void *) UDPConfiguration.unusedUDPDstPort1);


        //Calculated the check sum of packet before sending it
        UDP_Compute_checksum(&UDP_S);

        //Push DUT to listen for incoming data
        TP_UdpReceiveAndForward(TP_UdpReceiveAndForward_cb_Fields, TP_Udp_ReceiveAndForward_even_cb_Fields,socketId_Fields, 8, 0);
        while (callback_Result_Fields != 0) { sleep(1); }

        printf("\n[+]:DUT ready to receive the incoming data\n");

        //Send data to DUT
        if (SendUDP(UDP_S) != 1) {
            printf("Error on sending");
            TP_GenaralEndTest(GenaralEndTest_cb_Fields, 8, Casename);
            printf("\n[+]:Test case UDP_FIELDS_08 end with error n°5\n");
            test_Result = 1;

        } else {
            printf("\n[+]:Packet delivered\n");

            //Close socket
            sleep(1);
            TP_UdpCloseSocket(UdpCloseSocket_cb_Fields, socketId_Fields);
            while (callback_Result_Fields != 0) { sleep(1); }

            callback_Result_Fields = 2;
            printf("\n[+]:socket id %d closed \n", socketId_Fields);

            //end the test
            TP_GenaralEndTest(GenaralEndTest_cb_Fields, 8, Casename);
            test_Result = 0;
            printf("\n***********[+]:Test case UDP_FIELDS_08 was successfully ended*************\n");

        }
    }

    return test_Result;
}

int UDP_FIELDS_09() {
    uint8_t test_Result = 1;
    callback_Result_Fields = 2;
    ip4addr ipv4DUT;
    ipv4DUT.dataLength = 4;
    extractIpAddress(UDPConfiguration.Dst_IP_addr, &ipv4DUT);


    UDP_Packet UDP_S;

    vint8 Casename;
    uint8 Case[] = "UDP_FIELDS_09:Fields - Total Length (equal to zero)";
    Casename.dataLength = sizeof(Case);
    Casename.Data = (uint8 *) calloc(Casename.dataLength, sizeof(uint8));
    memcpy(Casename.Data, Case, Casename.dataLength);

    //Start the test
    TP_GenaralStartTest(GenaralStartTest_cb_Fields);
    while (callback_Result_Fields != 0) { sleep(1); }

    callback_Result_Fields = 2;
    printf("\n[+]: Test UDP_FIELDS_09 was started\n");

    //Create and bind the socket
    TP_UdpCreateAndBind(UdpCreateAndBind_cb_Fields, TRUE, UDPConfiguration.unusedUDPDstPort1, ipv4DUT);
    while (callback_Result_Fields != 0) { sleep(1); }

    callback_Result_Fields = 2;
    printf("\n[+]: Socket was successfully created and bound\n");

    //Create Packet
    printf("\n[+]: Creation of UDP packet");
    UDP_S = CreateUDP();
    if (UDP_S.length == 0) {
        printf("\n[+]: Udp packet creation error\n");
        TP_GenaralEndTest(GenaralEndTest_cb_Fields, 9, Casename);
        printf("\n[+]: Test case UDP_FIELDS_09 end with error n°1\n");
        test_Result = 1;
    } else {
        printf("\n[+]: udp packet well created\n");
        //edit udp fields
        EditUDPField(&UDP_S, PAYLAOD, PAYLOAD_data, (void *) UDPConfiguration.UDPData);

        //Edit payload length
        EditUDPField(&UDP_S, PAYLAOD, PAYLOAD_length, (void *) UDPConfiguration.UDPDataLen);

        //edit UDP header length
        EditUDPField(&UDP_S, UDP, UDP_Length, (void *) 0);

        //edit UDP Source port
        EditUDPField(&UDP_S, UDP, UDP_SrcPort, (void *) UDPConfiguration.testerUDPPort);

        //edit UDP Destination port
        EditUDPField(&UDP_S, UDP, UDP_DstPort, (void *) UDPConfiguration.unusedUDPDstPort1);

        //Push DUT  to listen and  receive the incoming data
        TP_UdpReceiveAndForward(TP_UdpReceiveAndForward_cb_Fields, TP_Udp_ReceiveAndForward_even_cb_Fields,
                                socketId_Fields, 8, UDPConfiguration.UDPDataLen);
        while (callback_Result_Fields != 0) { sleep(1); }
        printf("\n[+]: DUT ies ready to receive and forward data\n");

        //Calculated the checksum of packet before sending it
        UDP_Compute_checksum(&UDP_S);

        //Send data to DUT
        if (SendUDP(UDP_S) != 1) {
            printf("\n[+]: Error on sending the packet\n");
            TP_GenaralEndTest(GenaralEndTest_cb_Fields, 9, Casename);
            printf("\n[+]: Test case UDP_FIELDS_09 end with error n °4\n");
            test_Result = 1;

        } else {
            printf("\n[+]: Packet delivered!");

            //Close the socket
            sleep(1);
            TP_UdpCloseSocket(UdpCloseSocket_cb_Fields, socketId_Fields);
            while (callback_Result_Fields != 0) { sleep(1); }
            callback_Result_Fields = 2;
            printf("\n[+]: socket id %d closed \n", socketId_Fields);

            //END the test
            TP_GenaralEndTest(GenaralEndTest_cb_Fields, 9, Casename);
            test_Result = 1;
            printf("\n*********[+]: Test case UDP_FIELDS_09 was successfully ended************ \n");

        }
    }


    return test_Result;
}

int UDP_FIELDS_10() {
    callback_Result_Fields = 2;
    uint8_t test_Result = 1;
    ip4addr ipv4DUT;
    ipv4DUT.dataLength = 4;
    extractIpAddress(UDPConfiguration.Dst_IP_addr, &ipv4DUT);

    UDP_Packet UDP_S;

    vint8 Casename;
    uint8 Case[] = "UDP_FIELDS_10:Fields - Total Length (greater than actual)";
    Casename.dataLength = sizeof(Case);
    Casename.Data = (uint8 *) calloc(Casename.dataLength, sizeof(uint8));
    memcpy(Casename.Data, Case, Casename.dataLength);


    //Start the Test
    TP_GenaralStartTest(GenaralStartTest_cb_Fields);
    while (callback_Result_Fields != 0) { sleep(1); }
    callback_Result_Fields = 2;
    printf("Test UDP_FIELDS_10 was started\n");

    //Create the socket and bind it
    TP_UdpCreateAndBind(UdpCreateAndBind_cb_Fields, TRUE, UDPConfiguration.unusedUDPDstPort1, ipv4DUT);
    while (callback_Result_Fields != 0) { sleep(1); }
    callback_Result_Fields = 2;
    printf("Socket was successfully created and bound\n");

    //Create a packet to send to DUt
    printf("\n[+]:Creation of UDP packet");
    UDP_S = CreateUDP();
    if (UDP_S.length == 0) {
        printf("UDP packet creation error\n");
        TP_GenaralEndTest(GenaralEndTest_cb_Fields, 10, Casename);
        printf("Test case UDP_FIELDS_10 end with error n°1");
        test_Result = 1;
    } else {
        printf("\n[+]:UPD packet created\n");

        //Edit payload data
        EditUDPField(&UDP_S, PAYLAOD, PAYLOAD_data, (void *) UDPConfiguration.UDPData);

        //Edit Payload length
        EditUDPField(&UDP_S, PAYLAOD, PAYLOAD_length, (void *) UDPConfiguration.UDPDataLen);

        //Edit UDP source port
        EditUDPField(&UDP_S, UDP, UDP_SrcPort, (void *) UDPConfiguration.testerUDPPort);

        //edit UDP Dest port
        EditUDPField(&UDP_S, UDP, UDP_DstPort, (void *) UDPConfiguration.unusedUDPDstPort1);

        //set UDP header length b
        EditUDPField(&UDP_S, UDP, UDP_Length, (void *) (UDPConfiguration.UDPDataLen + 8 + 1));

        //Push DUT to receive data
        TP_UdpReceiveAndForward(TP_UdpReceiveAndForward_cb_Fields, TP_Udp_ReceiveAndForward_even_cb_Fields,
                                socketId_Fields, 8, UDPConfiguration.UDPDefaultDataLen);
        while (callback_Result_Fields != 0) { sleep(1); }
        printf("\n[+]:DUT is ready to receive data\n");

        //Compute the Checksum of packet before sending it
        UDP_Compute_checksum(&UDP_S);

        //Send the packet ot DUT
        if (SendUDP(UDP_S) != 1) {
            printf("Error on sending \n");
            TP_GenaralEndTest(GenaralEndTest_cb_Fields, 10, Casename);
            printf("Test case UDP_FIELDS_10 end with error n°5");
            test_Result = 1;

        } else {
            printf("Packet delivered\n");

            //Close the socket
            sleep(1);
            TP_UdpCloseSocket(UdpCloseSocket_cb_Fields, socketId_Fields);
            while (callback_Result_Fields != 0) { sleep(1); }
            callback_Result_Fields = 2;
            printf("\n[+]:socket id %d closed \n", socketId_Fields);

            //ENd the test
            TP_GenaralEndTest(GenaralEndTest_cb_Fields, 10, Casename);
            test_Result = 0;
            printf("\n********[+]:Test case UDP_FIELDS_10 was successfully ended*********** \n");
            test_Result = 0;
            return test_Result;

        }
    }

    return test_Result;

}

int UDP_FIELDS_12() {/*
    callback_Result_Fields = 2;
    uint16 test_Result = 1;
    ip4addr ipv4DUT;
    ipv4DUT.dataLength = 4;
    extractIpAddress(UDPConfiguration.Dst_IP_addr, &ipv4DUT);

    UDP_Packet UDP_S;


    vint8 Casename;

    uint8 Case[] = "UDP_FIELDS_12:Fields - Total Length (maximum)";
    Casename.Data = (uint8 *) calloc(Casename.dataLength, sizeof(uint8));
    Casename.dataLength = sizeof(Case);
    memcpy(Casename.Data, Case, Casename.dataLength);

    //Start the test
    TP_GenaralStartTest(GenaralStartTest_cb_Fields);
    while (callback_Result_Fields != 0) { sleep(1); }

    callback_Result_Fields = 2;
    printf("Test UDP_FIELDS_12 was started\n");

    //Create the socket and bind it
    TP_UdpCreateAndBind(UdpCreateAndBind_cb_Fields, TRUE, UDPConfiguration.unusedUDPDstPort1, ipv4DUT);
    while (callback_Result_Fields != 0) { sleep(1); }
    callback_Result_Fields = 2;
    printf("\n[+]:Socket was successfully created and bound\n");

    //Configure DUT to listen and receive the packet
    TP_UdpReceiveAndForward(TP_UdpReceiveAndForward_cb_Fields, TP_Udp_ReceiveAndForward_even_cb_Fields, socketId_Fields,8, 65507);
    while (callback_Result_Fields != 0) { sleep(1); }
    printf("\n[+]:DUt is ready to receive and forward data\n");

    //Create the packet
    UDP_S = CreateUDP();
    if (UDP_S.length == 0) {
        printf("\n[+]:Error on creation the udp packet\n");
        TP_GenaralEndTest(GenaralEndTest_cb_Fields, 12, Casename);
        printf("\n[+]:Test case UDP_FIELDS_12 end with error n°1\n");
    } else {
        printf("\n[+]:Udp packet well created\n");

        //Set payload data
        EditUDPField(&UDP_S, PAYLAOD, PAYLOAD_data, (void *) UDPConfiguration.UDPDefaultData);

        //set payload length to maximum length
        //EditUDPField(&UDP_S, PAYLAOD, PAYLOAD_length, (void *) 65507);

        //Set UDP header length to maximum size
        EditUDPField(&UDP_S,UDP,UDP_Length,(void*)65515);

        //Set UDP Scr port to tester port
        EditUDPField(&UDP_S, UDP, UDP_SrcPort, (void *) UDPConfiguration.testerUDPPort);

        //set UDP Dst port to unusedUDPDstPort
        EditUDPField(&UDP_S, UDP, UDP_DstPort, (void *) UDPConfiguration.unusedUDPDstPort1);

        //Compute checksums
        UDP_Compute_checksum(&UDP_S);

        //send data to DUt
        if (SendUDP(UDP_S) !=1) {
            printf("\n[+]:Error on sending \n");
            TP_GenaralEndTest(GenaralEndTest_cb_Fields, 12, Casename);
            printf("\n[+]:Test case UDP_FIELDS_12 end with error n°5");
            test_Result=1;
        } else {
            printf("\n[+]:Packet delivered\n");

            //Close the socket
            sleep(1);
            TP_UdpCloseSocket(UdpCloseSocket_cb_Fields, socketId_Fields);
            while (callback_Result_Fields != 0) { sleep(1); }
            callback_Result_Fields = 2;
            printf("\n[+]:socket id %d was closed \n", socketId_Fields);

            //End the Test
            TP_GenaralEndTest(GenaralEndTest_cb_Fields, 12, Casename);
            printf("\n[+]:Test case UDP_FIELDS_12 was successfully ended\n");

        }
    }

*/
    return 2;
}

int UDP_FIELDS_13() {
    callback_Result_Fields = 2;
    uint16 test_Result = 1;
    ip4addr ipv4DUT, ipv4Tester;
    ipv4DUT.dataLength = 4;
    extractIpAddress(UDPConfiguration.Dst_IP_addr, &ipv4DUT);


    ipv4Tester.dataLength = 4;
    extractIpAddress(UDPConfiguration.Host_1_IP, &ipv4Tester);


    vint8 udpUserData, Casename;

    uint8 Case[] = "UDP_FIELDS_13 : Fields - Checksum (with padding)";
    Casename.dataLength = sizeof(Case);
    Casename.Data = (uint8 *) calloc(Casename.dataLength, sizeof(uint8));
    memcpy(Casename.Data, Case, Casename.dataLength);

    udpUserData.dataLength = UDPConfiguration.udpUserDataSize;
    udpUserData.Data = (uint8 *) calloc(udpUserData.dataLength, sizeof(uint8));

    UDP_Packet UDP_R;
    Packet_filter filter;

    //Start Test
    TP_GenaralStartTest(GenaralStartTest_cb_Fields);
    while (callback_Result_Fields != 0) { sleep(1); }

    callback_Result_Fields = 2;
    printf("Test UDP_FIELDS_13 was started\n");

    //Create and bind the socket
    TP_UdpCreateAndBind(UdpCreateAndBind_cb_Fields, TRUE, UDPConfiguration.DUTUDPPort, ipv4DUT);
    while (callback_Result_Fields != 0) { sleep(1); }

    callback_Result_Fields = 2;
    printf("Socket was successfully created and bound\n");

    //Set the filter configuration to listen and receive the incoming data
    uint8_t timeout = 3;
    strcpy(filter.dstAdd, UDPConfiguration.Host_1_IP);
    filter.Dstport = UDPConfiguration.testerUDPPort;
    strcpy(filter.SrcAdd, UDPConfiguration.Dst_IP_addr);
    filter.Srcport = UDPConfiguration.DUTUDPPort;

    //DUT send data
    TP_UdpSendData(TP_UdpSendData_cb_Fields, socketId_Fields, 0, UDPConfiguration.testerUDPPort, ipv4Tester,udpUserData);
    UDP_R = ListenUDP(filter, timeout);//Tester listen and receive the incoming data from DUT
    while (callback_Result_Fields != 0) { sleep(1); }

    callback_Result_Fields = 2;
    printf("DUT sent data\n");

    //Verify that tester receive data from DUT
    if (UDP_R.length == 0) {
        printf("Error =====> DUT doesn't respond \n");
        TP_GenaralEndTest(GenaralEndTest_cb_Fields, 13, Casename);
        printf("Test case UDP_FIELDS_13 end with error n°1\n");
        test_Result = 1;

    } else {
        printf("\n[+]: Packet well received\n");

        //verify Udp checksum
        if (UDP_Verify_Correct_checksum(UDP_R) == 1) {
            printf("\n[+]: Different checksum\n");
            TP_GenaralEndTest(GenaralEndTest_cb_Fields, 13, Casename);
            printf("\n[+]: Test case UDP_FIELDS_13 end with error n°2");
            test_Result = 1;

        } else {
            printf("\n[+]: same checksum");

            //Close socket
            TP_UdpCloseSocket(UdpCloseSocket_cb_Fields, socketId_Fields);
            while (callback_Result_Fields != 0) { sleep(1); }

            callback_Result_Fields = 2;
            printf("\n[+]: Socket closed\n");

            //Close the test
            TP_GenaralEndTest(GenaralEndTest_cb_Fields, 13, Casename);
            test_Result = 0;
            printf("\n***********[+]: Test case UDP_FIELDS_13 was successfully ended***********\n");

        }
    }

    return test_Result;
}


int UDP_FIELDS_14() {
    callback_Result_Fields = 2;
    uint16 test_Result = 1;
    ip4addr ipv4DUT, ipv4Tester;
    ipv4DUT.dataLength = 4;
    extractIpAddress(UDPConfiguration.Dst_IP_addr, &ipv4DUT);


    ipv4Tester.dataLength = 4;
    extractIpAddress(UDPConfiguration.Host_1_IP, &ipv4Tester);


    UDP_Packet UDP_R;
    Packet_filter filter;


    vint8 Casename, UDPMessage;
    uint8 Case[] = "UDP_FIELDS_14: Fields - Checksum (no padding)";
    Casename.dataLength = sizeof(Case);
    Casename.Data = (uint8 *) calloc(Casename.dataLength, sizeof(uint8));
    memcpy(Casename.Data, Case, Casename.dataLength);

    UDPMessage.dataLength = 100;
    UDPMessage.Data = (uint8 *) calloc(100, sizeof(uint8));

    //Start Test
    TP_GenaralStartTest(GenaralStartTest_cb_Fields);
    while (callback_Result_Fields != 0) { sleep(1); }

    callback_Result_Fields = 2;
    printf("Test UDP_FIELDS_14 was started\n");

    //Create and bind the socket
    TP_UdpCreateAndBind(UdpCreateAndBind_cb_Fields, TRUE, UDPConfiguration.DUTUDPPort, ipv4DUT);
    while (callback_Result_Fields != 0) { sleep(1); }

    callback_Result_Fields = 2;
    printf("Socket was successfully created and bound\n");

    //Set the filter configuration to listen and receive the incoming data
    uint8_t timeout = 3;
    strcpy(filter.dstAdd, UDPConfiguration.Host_1_IP);
    filter.Dstport = UDPConfiguration.testerUDPPort;
    strcpy(filter.SrcAdd, UDPConfiguration.Dst_IP_addr);
    filter.Srcport = UDPConfiguration.DUTUDPPort;

    //DUT send data
    TP_UdpSendData(TP_UdpSendData_cb_Fields, socketId_Fields, 0, UDPConfiguration.testerUDPPort, ipv4Tester,
                   UDPMessage);
    UDP_R = ListenUDP(filter, timeout);//Tester listen and receive the incoming data from DUT
    while (callback_Result_Fields != 0) { sleep(1); }

    callback_Result_Fields = 2;
    printf("DUT sent data\n");

    //Verify that tester well receive the data
    if (UDP_R.length == 0) {
        printf("Error =====> DUT doesn't respond \n");
        TP_GenaralEndTest(GenaralEndTest_cb_Fields, 14, Casename);
        printf("Test case UDP_FIELDS_14 end with error n°1\n");
        test_Result = 1;

    } else {
        printf("\n[+]: Packet well received\n");

        //verify Udp checksum
        if (UDP_Verify_Correct_checksum(UDP_R) == 1) {
            printf("\n[+]Different checksum\n");
            TP_GenaralEndTest(GenaralEndTest_cb_Fields, 14, Casename);
            printf("\n[+]:Test case UDP_FIELDS_14 end with error n°2");
            test_Result = 1;
        } else {
            printf("\n[+]:same checksum");

            //Close socket
            TP_UdpCloseSocket(UdpCloseSocket_cb_Fields, socketId_Fields);
            while (callback_Result_Fields != 0) { sleep(1); }

            callback_Result_Fields = 2;
            printf("\n[+]: Socket closed\n");
            //Close the test
            TP_GenaralEndTest(GenaralEndTest_cb_Fields, 14, Casename);
            test_Result = 0;
            printf("\n[+]:*******Test case UDP_FIELDS_14 was successfully ended********\n");

        }
    }

    return test_Result;
}

int UDP_FIELDS_15() {
    callback_Result_Fields = 2;
    uint8_t test_Result = 1;
    ip4addr ipv4DUT;
    ipv4DUT.dataLength = 4;
    extractIpAddress(UDPConfiguration.Dst_IP_addr, &ipv4DUT);

    UDP_Packet UDP_S;

    vint8 Casename;

    uint8 Case[] = "UDP_FIELDS_15:Fields - Checksum (incorrect)";
    Casename.dataLength = sizeof(Case);
    Casename.Data = (uint8 *) calloc(Casename.dataLength, sizeof(uint8));
    memcpy(Casename.Data, Case, Casename.dataLength);
    //Start the test
    TP_GenaralStartTest(GenaralStartTest_cb_Fields);
    while (callback_Result_Fields != 0) { sleep(1); }

    callback_Result_Fields = 2;
    printf("\n[+]:Test UDP_FIELDS_15 was started\n");

    //Create the socket and bind it
    TP_UdpCreateAndBind(UdpCreateAndBind_cb_Fields, TRUE, UDPConfiguration.unusedUDPDstPort1, ipv4DUT);
    while (callback_Result_Fields != 0) { sleep(1); }

    callback_Result_Fields = 2;
    printf("\n[+]:Socket was successfully created and bound\n");

    //Create the UDP packet
    UDP_S = CreateUDP();
    if (UDP_S.length == 0) {
        printf("creation udp packet failed\n");
        TP_GenaralEndTest(GenaralEndTest_cb_Fields, 15, Casename);
        printf("Test case UDP_FIELDS_15 end with error n°1\n");
        test_Result = 1;

    } else {
        printf("Udp packet well created\n");

        //set Payload Data
        EditUDPField(&UDP_S, PAYLAOD, PAYLOAD_data, (void *) UDPConfiguration.UDPDefaultData);

        //set Payload length
        EditUDPField(&UDP_S, PAYLAOD, PAYLOAD_length, (void *) (UDPConfiguration.UDPDefaultDataLen));

        //set UDP header length
        EditUDPField(&UDP_S, UDP, UDP_Length, (void *) (UDPConfiguration.UDPDefaultDataLen + 8));

        //set UDP Dst port
        EditUDPField(&UDP_S, UDP, UDP_DstPort, (void *) UDPConfiguration.unusedUDPDstPort1);

        //Set UPD source port
        EditUDPField(&UDP_S, UDP, UDP_SrcPort, (void *) UDPConfiguration.testerUDPPort);

        //Calculated the packet checksums
        UDP_Compute_checksum(&UDP_S);

        //set the UDP_Checksum value to incorrectUDPChecksum
        EditUDPField(&UDP_S, UDP, UDP_Checksum, (void *) UDPConfiguration.incorrectUDPChecksum);
        printf("\n[+]Udp checksum  well modified\n");

        //configure DUT to receive the packet
        TP_UdpReceiveAndForward(TP_UdpReceiveAndForward_cb_Fields, TP_Udp_ReceiveAndForward_even_cb_Fields,
                                socketId_Fields, 8, UDPConfiguration.UDPDefaultDataLen);
        while (callback_Result_Fields != 0) { sleep(1); }


        printf("\n[+]:DUT ready for incoming UDP Data\n");
        //Send the packet
        if (SendUDP(UDP_S) != 1) {
            printf("\n|+]:Sending failed\n");
            TP_GenaralEndTest(GenaralEndTest_cb_Fields, 15, Casename);
            printf("\n[+]:Test case UDP_FIELDS_15 end with error n°3 ");
            test_Result = 1;

        } else {
            printf("\n[+]:Packet delivered!\n");

            //Close the socket
            sleep(1);
            TP_UdpCloseSocket(UdpCloseSocket_cb_Fields, socketId_Fields);
            while (callback_Result_Fields != 0) { sleep(1); }

            callback_Result_Fields = 2;
            printf("\n[+]: Socket id %d closed \n", socketId_Fields);

            //Close the test
            TP_GenaralEndTest(GenaralEndTest_cb_Fields, 15, Casename);
            test_Result = 1;
            printf("\n*************[+]:Test case UDP_FIELDS_15 was successfully ended*************\n");

        }
    }

    return test_Result;
}

int UDP_FIELDS_16() {
    callback_Result_Fields = 2;
    uint8_t test_Result = 1;
    ip4addr ipv4DUT;
    ipv4DUT.dataLength = 4;
    extractIpAddress(UDPConfiguration.Dst_IP_addr, &ipv4DUT);

    UDP_Packet UDP_S;

    vint8 Casename;

    uint8 Case[] = "UDP_FIELDS_16: Fields - Checksum (zero checksum)";
    Casename.dataLength = sizeof(Case);
    Casename.Data = (uint8 *) calloc(Casename.dataLength, sizeof(uint8));
    memcpy(Casename.Data, Case, Casename.dataLength);

    //Start the test
    TP_GenaralStartTest(GenaralStartTest_cb_Fields);
    while (callback_Result_Fields != 0) { sleep(1); }

    callback_Result_Fields = 2;
    printf("\n[+]: Test UDP_FIELDS_16 was started\n");

    //Create the socket and bind it
    TP_UdpCreateAndBind(UdpCreateAndBind_cb_Fields, TRUE, UDPConfiguration.unusedUDPDstPort1, ipv4DUT);
    while (callback_Result_Fields != 0) { sleep(1); }

    callback_Result_Fields = 2;
    printf("\n[+]: socket was successfully created and bound\n");

    //Create the UDP packet
    UDP_S = CreateUDP();
    if (UDP_S.length == 0) {
        printf("\n[+]: failure to create a Udp packet\n");
        TP_GenaralEndTest(GenaralEndTest_cb_Fields, 16, Casename);
        printf("\n[+]: Test case UDP_FIELDS_16 ended with error n°1");
        test_Result = 1;

    } else {
        printf("\n[+]: Udp packet well created\n");

        //set payload data
        EditUDPField(&UDP_S, PAYLAOD, PAYLOAD_data, (void *) UDPConfiguration.UDPDefaultData);

        //set payload length
        EditUDPField(&UDP_S, PAYLAOD, PAYLOAD_length, (void *) UDPConfiguration.UDPDefaultDataLen);

        //set UDP dst port
        EditUDPField(&UDP_S, UDP, UDP_DstPort, (void *) UDPConfiguration.unusedUDPDstPort1);

        //set UDP source port
        EditUDPField(&UDP_S, UDP, UDP_SrcPort, (void *) UDPConfiguration.testerUDPPort);

        //set UPD header length
        EditUDPField(&UDP_S, UDP, UDP_Length, (void *) (UDPConfiguration.UDPDefaultDataLen + 8));

        //Compute the checksums of UDP packet
        UDP_Compute_checksum(&UDP_S);

        EditUDPField(&UDP_S, UDP, UDP_Checksum, (void *) 0x0000);
        printf("\n[+]: UDP packet well modified\n");

        //configure DUT to receive the packet
        callback_Result_Fields != 0;
        TP_UdpReceiveAndForward(TP_UdpReceiveAndForward_cb_Fields, TP_Udp_ReceiveAndForward_even_cb_Fields,
                                socketId_Fields, 8, UDPConfiguration.UDPDefaultDataLen);
        while (callback_Result_Fields != 0) { sleep(1); }
        printf("\n[+]:DUT is ready to receive and forward data\n");

        //send data to DUT
        if (SendUDP(UDP_S) != 1) {
            printf("\n[+]:Sending error\n");
            TP_GenaralEndTest(GenaralEndTest_cb_Fields, 16, Casename);
            printf("\n[+]:Test case UDP_FIELDS_16 end with error n°14\n");
            test_Result = 1;

        } else {
            printf("\n[+]:Packet delivered\n");

            //close socket
            sleep(1);
            TP_UdpCloseSocket(UdpCloseSocket_cb_Fields, socketId_Fields);
            while (callback_Result_Fields != 0) { sleep(1); }

            callback_Result_Fields = 2;
            printf("socket id %d closed", socketId_Fields);

            //end test
            TP_GenaralEndTest(GenaralEndTest_cb_Fields, 16, Casename);
            test_Result = 0;
            printf("\n************[+]:Test case UDP_FIELDS_16 was successfully ended********* \n");
        }
    }


    return test_Result;
}
