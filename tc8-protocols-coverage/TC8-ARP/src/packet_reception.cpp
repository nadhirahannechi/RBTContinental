#include "packet_reception.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AbstractionAPI.h"
#include "TestabilityProtocol_Intern.h"
#include "TestabilityProtocol_api.h"
#include "config.h"
#include"common.h"
#include<thread>
#include <future>
extern Arp_config_t ARP_config;

uint8_t Callback_result  = 2;

void  ARP_CLEAR_CACHE_Cb_Client(TP_ResultID_t b)
{   
    Callback_result = b;
    TP_TRACE_DEBUG("cb TP_ResultID_t : %d  ",b);
}

void ARP_ADD_STATIC_ENTRY_Cb_Client(TP_ResultID_t b)
{   
    Callback_result = b;
    TP_TRACE_DEBUG("cb TP_ResultID_t : %d  ",b);
}

void ARP_DELETE_STATIC_ENTRY_Cb_Client(TP_ResultID_t b)
{   
    Callback_result = b;
    TP_TRACE_DEBUG("cb TP_ResultID_t : %d  ",b);
}

void ECHO_REQUEST_Cb_Client(TP_ResultID_t b)
{   
    Callback_result = b;
    TP_TRACE_DEBUG("cb TP_ECHO_REQUEST_t : %d  ",b);
}

void GenaralStartTest_Cb_Client(TP_ResultID_t b)
{   
    TP_TRACE_DEBUG("cb TP_ResultID_t %d  ",b);
}

void GenaralEndTest_Cb_Client(TP_ResultID_t b)
{
    TP_TRACE_DEBUG("cb TP_ResultID_t %d socketID ",b);
}


int ARP_16()
{   
    int Arp_16_Test_result = 1;
    Packet_filter f;
    f.arp_operation = 0x0002;
    strcpy(f.spa,(char*)ARP_config.DIface_0_IP); 
    strcpy(f.tpa,(char*)ARP_config.HOST_1_IP);
     /*TESTER: <HOST-1> Sends ARP Request to DUT through <DIface-0> containing: 
        -ARP Target Hardware Address set to <all-zeroes>*/
    ARP_Packet ARP_P = CreateARP();
    EditARPField(&ARP_P,ARP,target_hardware_address,(void*)ARP_config.all_zeroes);
    SendARP(ARP_P);
    /*TESTER: <HOST-1> Listens (up to <ParamListenTime>) on <DIface-0> */
    ARP_Packet ARP_PR = ListenARP(f,ARP_config.ParamListenTime);
    if(ARP_PR.length !=0 )
    {
        printf("\n*******test ARP_16 passed******\n");
        Arp_16_Test_result = 0;
    }
    else
    {
        printf("\n******test ARP_16 failed:no arp reply recieved*******\n");
    }
    return Arp_16_Test_result;
}

int ARP_17()
{   
    int Arp_17_Test_result = 1;
    Packet_filter f;
    f.arp_operation = 0x0002;   // arp reply
    strcpy(f.spa,(char*)ARP_config.DIface_0_IP ); // address ip of DUT
    strcpy(f.tpa,(char*)ARP_config.HOST_1_IP);
    /*TESTER: <HOST-1> Sends ARP Request to DUT through <DIface-0> containing: 
        -ARP Target Hardware Address set to <ETHERNET_BROADCAST_ADDR>*/
    ARP_Packet ARP_P = CreateARP();
    EditARPField(&ARP_P,ARP,target_hardware_address,(void*)ARP_config.ETHERNET_BROADCAST_ADDR);
    SendARP(ARP_P);
    /*TESTER: <HOST-1> Listens (up to <ParamListenTime>) on <DIface-0> */
    ARP_Packet ARP_PR = ListenARP(f,ARP_config.ParamListenTime);
    if(ARP_PR.length !=0 )
    {
        printf("\n**********test ARP_17 passed***********\n");
        Arp_17_Test_result = 0;
    }
    else
    {
        printf("\n**********test ARP_17 failed:no arp reply recieved************\n");
    }
    return Arp_17_Test_result;
}

int ARP_18()
{    
    int Arp_18_Test_result = 1;
    Packet_filter f;
    f.arp_operation = 0x0002;   // arp reply
    strcpy(f.spa,(char*)ARP_config.DIface_0_IP ); // address ip of DUT
    strcpy(f.tpa,(char*)ARP_config.HOST_1_IP);
    /*TESTER: <HOST-1> Sends ARP Request to DUT through <DIface-0> containing: 
        -ARP Target Hardware Address set to  ARBIT_MAC_ADDR*/
    ARP_Packet ARP_P = CreateARP();
    EditARPField(&ARP_P,ARP,target_hardware_address,(void*)ARP_config.ARBIT_MAC_ADDR);
    SendARP(ARP_P);
    /*TESTER: <HOST-1> Listens (up to <ParamListenTime>) on <DIface-0> */
    ARP_Packet ARP_PR = ListenARP(f,ARP_config.ParamListenTime);
    if(ARP_PR.length !=0 )
    {
        printf("\n**********test ARP_18 passed************\n");
        Arp_18_Test_result = 0;
    }
    else
    {
        printf("\n***********test ARP_18 failed:no arp reply recieved************\n");
    }
    return Arp_18_Test_result;
}

int ARP_19()
{   
    int Arp_19_Test_result = 1; 
    Packet_filter f;
    f.arp_operation = 0x0002;   // arp reply
    strcpy(f.spa,(char*)ARP_config.DIface_0_IP ); // address ip of DUT
    strcpy(f.tpa,(char*)ARP_config.HOST_1_IP);
    /*TESTER: <HOST-1> Sends ARP Request to DUT through <DIface-0> containing: 
        -ARP Target Hardware Address set to  DIFACE-0-MAC-ADDR*/
    ARP_Packet ARP_P = CreateARP();
    EditARPField(&ARP_P,ARP,target_hardware_address,(void*)ARP_config.DIFACE_O_MAC_ADDR);
    SendARP(ARP_P);
    /*TESTER: <HOST-1> Listens (up to <ParamListenTime>) on <DIface-0> */
    ARP_Packet ARP_PR = ListenARP(f,ARP_config.ParamListenTime);
    if(ARP_PR.length !=0 )
    {
        printf("\n**********test ARP_19 passed************\n");
        Arp_19_Test_result = 0;
    }
    else
    {
        printf("\n***********test ARP_19 failed:no arp reply recieved***********\n");
    }
    return Arp_19_Test_result;
}

int ARP_20()
{   
    Callback_result = 2;
    int Arp_20_Test_result = 1;
      
    TP_GenaralStartTest(GenaralStartTest_Cb_Client);
    /* Configure DUT to clear the dynamic entries in its ARP Cache */
    TP_ClearArpCache(ARP_CLEAR_CACHE_Cb_Client);
    while(Callback_result == 2);
    if(Callback_result == 0) /* arp cache cleared */
    {       
        Callback_result = 2;
        Packet_filter f;
        f.arp_operation = 0x0002;   // arp reply
        strcpy(f.spa,(char*)ARP_config.DIface_0_IP ); // address ip of DUT
        strcpy(f.tpa,(char*)ARP_config.HOST_1_IP);
        /* TESTER: <HOST-1> Sends ARP Request to DUT through <DIface-0> */
        ARP_Packet ARP_P = CreateARP();
        SendARP(ARP_P);
        ARP_Packet ARP_PR = ListenARP(f,ARP_config.ParamListenTime);
        /*TESTER: <HOST-1> Listens (up to <ParamListenTime>) on <DIface-0>*/
        if(ARP_PR.length !=0 )
        {
            printf("\n*******test ARP_20 passed********\n");
            Arp_20_Test_result = 0;
        }
        else
        {
            printf("\n*******test ARP_20 failed: no arp reply recieved********\n");
        }
    }
    else
    {
        printf("\n*******test ARP_20 failed : TP_ClearArpCache*******\n");
    }
    TP_GenaralEndTest(GenaralEndTest_Cb_Client,0,(text){0});
       
    return Arp_20_Test_result;
}

int ARP_21()
{   
    Callback_result = 0;
    int Arp_21_Test_result = 1; 
      
    TP_GenaralStartTest(GenaralStartTest_Cb_Client);
    /* Configure DUT to clear the dynamic entries in its ARP Cache */
    TP_ClearArpCache(ARP_CLEAR_CACHE_Cb_Client);
    while(Callback_result == 2);
    if(Callback_result == 0) /* arp cache cleared */
    {   
        Callback_result = 2;
        Packet_filter f;
        f.arp_operation = 0x0002;   // arp reply
        strcpy(f.spa,(char*)ARP_config.DIface_0_IP ); // address ip of DUT
        strcpy(f.tpa,(char*)ARP_config.HOST_1_IP);
        /* TESTER: <HOST-1> Sends ARP Request to DUT through <DIface-0> containing:
            - Hardware Type set to ARP_HARDWARE_TYPE_UNKNOWN */
        ARP_Packet ARP_P = CreateARP();
        EditARPField(&ARP_P,ARP,hardware_address_format,(void*)ARP_config.ARP_HARDWARE_TYPE_UNKNOWN);
        SendARP(ARP_P);
        /* TESTER: <HOST-1> Listens (up to <ParamListenTime>) on <DIface-0>*/
        ARP_Packet ARP_PR = ListenARP(f,ARP_config.ParamListenTime);
        if(ARP_PR.length == 0 )
        {
            printf("\n********test ARP_21 passed\n********\n");
            Arp_21_Test_result = 0;
        }
        else
        {
            printf("\n*********test ARP_21 failed:Arp request recieved*********\n");
        }
    }
    else
    {
        printf("\n*******test ARP_21 failed : TP_ClearArpCache*******\n");
    }
    TP_GenaralEndTest(GenaralEndTest_Cb_Client,0,(text){0});
       
    return Arp_21_Test_result;   
}

int ARP_22()
{   
    Callback_result = 2;
    int Arp_22_Test_result = 1; 
      
    TP_GenaralStartTest(GenaralStartTest_Cb_Client);
    /* Configure DUT to clear the dynamic entries in its ARP Cache */
    TP_ClearArpCache(ARP_CLEAR_CACHE_Cb_Client);
    while(Callback_result == 2);
    if(Callback_result == 0) /* arp cache cleared */
    {   
        Callback_result = 2;
        /* TESTER: <HOST-1> Sends ARP Response(Gratuitous) to DUT through <DIface-0> containing: 
        - Hardware Type set to ARP_HARDWARE_TYPE_UNKNOWN 
        - ARP Sender Hardware Address set to <MAC-ADDR1> 
        - ARP Target Hardware Address set to ETHERNET_BROADCAST_ADDR  */
        ARP_Packet ARP_P = CreateARP();
        EditARPField(&ARP_P,ARP,opcode,(void*)ARP_config.OPERATION_RESPONSE);   /* ARP Response */
        EditARPField(&ARP_P,ETHERNET,ETH_destinationMAC,(void*)ARP_config.DIFACE_O_MAC_ADDR); /* set the mac address of the DUT */
        EditARPField(&ARP_P,ARP,hardware_address_format,(void *)ARP_config.ARP_HARDWARE_TYPE_UNKNOWN); 
        EditARPField(&ARP_P,ARP,target_hardware_address,(void*)ARP_config.ETHERNET_BROADCAST_ADDR);
        SendARP(ARP_P);
        /*wait <ARP-TOLERANCE-TIME> seconds for the arp cache to get refreshed*/
        sleep(ARP_config.ARP_TOLERANCE_TIME); 
        /* Configure DUT to send a ICMP Request Message to TESTER */
        ip4addr ipv4tester;
        ipv4tester.dataLength = 4;
	    stoiIP((char*)ARP_config.HOST_1_IP,ipv4tester.Data);
        char dataicmp[] = "Test_ARP_22";
        vint8 data_to_send;
        data_to_send.Data = (uint8* )dataicmp;
        data_to_send.dataLength = 12;
        char interf[] = "eth0";
        text text_to_send;
        text_to_send.dataLength = 7;
        text_to_send.Data = (uint8 *)interf;
        Packet_filter f;
        f.arp_operation = 0x002;   // arp reply
        strcpy(f.spa,(char*)ARP_config.DIface_0_IP ); // address ip of DUT
        strcpy(f.tpa,(char*)ARP_config.HOST_1_IP);
        auto future = std::async(ListenARP,f,ARP_config.ParamListenTime);
        TP_IcmpEchoRequest(ECHO_REQUEST_Cb_Client,text_to_send,ipv4tester,data_to_send);
        ARP_Packet ARP_PR = future.get();
        /* TESTER: <HOST-1> Listens (up to <ParamListenTime>) on <DIface-0>*/
        while(Callback_result == 2){usleep(100);}
        if(Callback_result == 0)
        {   
            Callback_result = 2;
            if(ARP_PR.length != 0 )
            {
                printf("\n********test ARP_22 passed*********\n");
                Arp_22_Test_result = 0;
            }
            else
            {
                printf("\n*********test ARP_22 failed**********\n");
            }
        }
        else
        {
            printf("\n******** test ARP_22 failed : TP_IcmpEchoRequest **********\n");
        }
    }
    else
    {
        printf("\n*******test ARP_22 failed : TP_ClearArpCache*******\n");
    }
    TP_GenaralEndTest(GenaralEndTest_Cb_Client,0,(text){0});
       
    return 2;//return Arp_22_Test_result;
}

int ARP_26()
{   
    Callback_result = 2;
    int Arp_26_Test_result = 1;
      
    TP_GenaralStartTest(GenaralStartTest_Cb_Client);
    /* Configure DUT to clear the dynamic entries in its ARP Cache */
    TP_ClearArpCache(ARP_CLEAR_CACHE_Cb_Client);
    while(Callback_result == 2);
    if(Callback_result == 0) /* arp cache cleared */
    {   
        Callback_result = 2;
        Packet_filter f;
        f.arp_operation = 0x0002;   // arp reply
        strcpy(f.spa,(char*)ARP_config.DIface_0_IP ); // address ip of DUT
        strcpy(f.tpa,(char*)ARP_config.HOST_1_IP);
        /*TESTER: <HOST-1> Sends ARP Request to DUT through <DIface-0> containing:
            - Protocol Type set to ARP_PROTOCOL_IP*/
        ARP_Packet ARP_P = CreateARP();
        SendARP(ARP_P);
        /*TESTER: <HOST-1> Listens (up to <ParamListenTime>) on <DIface-0>*/
        ARP_Packet ARP_PR = ListenARP(f,ARP_config.ParamListenTime);
        if(ARP_PR.length != 0)
        {
            printf("\n********test ARP_26 passed\n********\n");
            Arp_26_Test_result = 0;
        }
        else
        {
            printf("\n*********test ARP_26 failed:No ARP reply recieved*********\n");
        }
    }
    else
    {
        printf("\n*******test ARP_26 failed : TP_ClearArpCache*******\n");
    }
    TP_GenaralEndTest(GenaralEndTest_Cb_Client,0,(text){0});
       
    return Arp_26_Test_result;
}

int ARP_27()
{   
    Callback_result = 2;
    int Arp_27_Test_result = 1;
      
    TP_GenaralStartTest(GenaralStartTest_Cb_Client);
    /* Configure DUT to clear the dynamic entries in its ARP Cache */
    TP_ClearArpCache(ARP_CLEAR_CACHE_Cb_Client);
    while(Callback_result == 2);
    if(Callback_result == 0) /* arp cache cleared */
    {   
        Callback_result = 2;
        Packet_filter f;
        f.arp_operation = 0x0002;   // arp reply
        strcpy(f.spa,(char*)ARP_config.DIface_0_IP ); // address ip of DUT
        strcpy(f.tpa,(char*)ARP_config.HOST_1_IP);
        /* TESTER: <HOST-1> Sends ARP Request to DUT through <DIface-0> containing:
        - Protocol Type set to ARP_PROTOCOL_UNKNOWN */
        ARP_Packet ARP_P = CreateARP();
        EditARPField(&ARP_P,ARP,protocol_address_format,(void *)0x1234);
        SendARP(ARP_P);
        ARP_Packet ARP_PR = ListenARP(f,ARP_config.ParamListenTime);
        if(ARP_PR.length == 0 )
        {
            printf("\n********test ARP_27 passed\n********\n");
            Arp_27_Test_result = 0;
        }
        else
        {
            printf("\n*********test ARP_27 failed:ARP reply recieved*********\n");
        }
    }
    else
    {
        printf("\n*******test ARP_27 failed : TP_ClearArpCache*******\n");
    }
    TP_GenaralEndTest(GenaralEndTest_Cb_Client,0,(text){0});
       
    return Arp_27_Test_result;
}

int ARP_28()
{   
    Callback_result = 2;
    int Arp_28_Test_result = 1;
      
    TP_GenaralStartTest(GenaralStartTest_Cb_Client);
    /* Configure DUT to clear the dynamic entries in its ARP Cache */
    TP_ClearArpCache(ARP_CLEAR_CACHE_Cb_Client);
    while(Callback_result == 2);
    if(Callback_result == 0) /* arp cache cleared */
    {   
        Callback_result = 2;
        /*  TESTER: <HOST-1> Sends ARP Response(Gratuitous) to DUT through <DIface-0> containing: 
        - Protocol Type set to ARP_PROTOCOL_UNKNOWN */
        ARP_Packet ARP_P = CreateARP();
        EditARPField(&ARP_P,ARP,opcode,(void*)ARP_config.OPERATION_RESPONSE);
        EditARPField(&ARP_P,ETHERNET,ETH_destinationMAC,(void*)ARP_config.DIFACE_O_MAC_ADDR); /* set the mac address of the DUT */
        EditARPField(&ARP_P,ARP,protocol_address_format,(void*)ARP_config.ARP_PROTOCOL_UNKNOWN);
        SendARP(ARP_P);
        /*wait <ARP-TOLERANCE-TIME> seconds for the arp cache to get refreshed*/
        sleep(ARP_config.ARP_TOLERANCE_TIME); 
        /* Configure DUT to send a ICMP Request Message to TESTER */
        ip4addr ipv4tester;
        ipv4tester.dataLength = 4;
	    stoiIP((char*)ARP_config.HOST_1_IP,ipv4tester.Data);
        char dataicmp[] = "Test_ARP_28";
        vint8 data_to_send;
        data_to_send.Data = (uint8* )dataicmp;
        data_to_send.dataLength = 12;
        char interf[] = "eth0";
        text text_to_send;
        text_to_send.dataLength = 7;
        text_to_send.Data = (uint8 *)interf;
        Packet_filter f;
        f.arp_operation = 0x0001;   // arp reply
        strcpy(f.spa,(char*)ARP_config.DIface_0_IP ); // address ip of DUT
        strcpy(f.tpa,(char*)ARP_config.HOST_1_IP);
        auto future = std::async(ListenARP,f,ARP_config.ParamListenTime);
        TP_IcmpEchoRequest(ECHO_REQUEST_Cb_Client,text_to_send,ipv4tester,data_to_send);
        /* TESTER: <HOST-1> Listens (up to <ParamListenTime>) on <DIface-0>*/
        ARP_Packet ARP_PR = future.get();
        while(Callback_result == 2){usleep(100);}
        if(Callback_result == 0)
        {
            Callback_result = 2;
            if(ARP_PR.length != 0 )
            {
                printf("\n********test ARP_28 passed********\n");
                Arp_28_Test_result = 0;
            }
            else
            {
                printf("\n********test ARP_28 failed: no ARP request recieved********\n");
            }
        }
        else
        {
            printf("\n*********test ARP_28 failed:ARP reply recieved*********\n");
        }
    } 
    else
    {
        printf("\n*******test ARP_28 failed : TP_ClearArpCache*******\n");
    }
    TP_GenaralEndTest(GenaralEndTest_Cb_Client,0,(text){0});
       
    return 2;//return Arp_28_Test_result;
}

int ARP_32()
{   
    Callback_result = 2;
    int Arp_32_Test_result = 1;
      
    TP_GenaralStartTest(GenaralStartTest_Cb_Client);
    /* Configure DUT to clear the dynamic entries in its ARP Cache */
    TP_ClearArpCache(ARP_CLEAR_CACHE_Cb_Client);
    while(Callback_result == 2){usleep(100);}
    if(Callback_result == 0) /* arp cache cleared */
    {   
        Callback_result = 2;
        /* ESTER:  <HOST-1> Sends ARP Request to DUT through <DIface-0> */
        ARP_Packet ARP_P1 = CreateARP();
        SendARP(ARP_P1);
        /*  TESTER:  <HOST-1> Sends ARP Request to DUT through <DIface-0> containing:
         - ARP Sender Hardware Address set to <MAC-ADDR2> 
          - Ethernet Source Address set to <MAC-ADDR2> */
        ARP_Packet ARP_P2 = CreateARP();
        EditARPField(&ARP_P2,ARP,sender_hardware_address,(void*)ARP_config.MAC_ADDR2);
        //EditARPField(&ARP_P2,ETHERNET,ETH_sourceMac,(void*)mac_addr2);
        SendARP(ARP_P2);
        /*wait <ARP-TOLERANCE-TIME> seconds for the arp cache to get refreshed*/
        sleep(ARP_config.ARP_TOLERANCE_TIME); 
        /* Configure DUT to send a ICMP Request Message to TESTER */
        ip4addr ipv4tester;
        ipv4tester.dataLength = 4;
	    stoiIP((char*)ARP_config.HOST_1_IP,ipv4tester.Data);
        char dataicmp[] = "Test_ARP_32";
        vint8 data_to_send;
        data_to_send.Data = (uint8* )dataicmp;
        data_to_send.dataLength = 12;
        char interf[] = "eth0";
        text text_to_send;
        text_to_send.dataLength = 7;
        text_to_send.Data = (uint8 *)interf;
        Packet_filter f;
        strcpy(f.SrcAdd ,(char*)ARP_config.DIface_0_IP );
        strcpy(f.dstAdd ,(char*)ARP_config.HOST_1_IP);
        TP_IcmpEchoRequest(ECHO_REQUEST_Cb_Client,text_to_send,ipv4tester,data_to_send);
        /* TESTER: <HOST-1> Listens (up to <ParamListenTime>) on <DIface-0> */
        ICMP_Packet ICMP_PR = ListenICMP(f,ARP_config.ParamListenTime); /* ParamListenTime = 3s */
        while(Callback_result == 2) {usleep(100);}
        if(Callback_result == 0)
        {   
            Callback_result = 2;
            if(ICMP_PR.length !=0 )
            {   
                char MAC2[18];
                lower_to_upper_mac(ARP_config.MAC_ADDR2,MAC2);
                if(strcmp((char *)GetICMPField(&ICMP_PR,ETHERNET,ETH_destinationMAC),(char*)MAC2)==0)
                {
                    printf("\n********test ARP_32 passed********\n");
                    Arp_32_Test_result = 0;
                }
                else
                {
                    printf("\n*********test ARP_32 failed : Ethernet Destination Address != <MAC-ADDR2>*******\n");
                } 
            }
            else
            {
                printf("\n********test ARP_32 failed********\n");
            }
        }
        else
        {
            printf("\n\n ******** test ARP_32 failed : TP_IcmpEchoRequest ********** \n\n");
        }
    }
    else
    {
        printf("\n*******test ARP_32 failed : TP_ClearArpCache*******\n");
    }
    TP_GenaralEndTest(GenaralEndTest_Cb_Client,0,(text){0});
       
    return Arp_32_Test_result;
}

int ARP_33()
{   
    Callback_result = 2;
    int Arp_33_Test_result = 1;
      
    TP_GenaralStartTest(GenaralStartTest_Cb_Client);
    /* Configure DUT to clear the dynamic entries in its ARP Cache */
    TP_ClearArpCache(ARP_CLEAR_CACHE_Cb_Client);
    while(Callback_result == 2){usleep(100);}
    if(Callback_result == 0) /* arp cache cleared */
    {   
        Callback_result = 2;
        /*TESTER:  <HOST-1> Sends ARP Response to DUT through <DIface-0>:containing:
            -ARP Sender Hardware Address set to <MAC-ADDR1>*/
        ARP_Packet ARP_P1 = CreateARP();
        EditARPField(&ARP_P1,ARP,opcode,(void*)ARP_config.OPERATION_RESPONSE);   /* ARP Response */
        EditARPField(&ARP_P1,ETHERNET,ETH_destinationMAC,(void*)ARP_config.DIFACE_O_MAC_ADDR); /* set the mac address of the DUT */
        SendARP(ARP_P1);
        /*TESTER:  <HOST-1> Sends ARP Response to DUT through <DIface-0>:containing:
            -ARP Sender Hardware Address set to <MAC-ADDR2>*/
        ARP_Packet ARP_P2 = CreateARP();
        EditARPField(&ARP_P2,ARP,opcode,(void*)ARP_config.OPERATION_RESPONSE);   /* ARP Response */
        EditARPField(&ARP_P2,ETHERNET,ETH_destinationMAC,(void*)ARP_config.DIFACE_O_MAC_ADDR); /* set the mac address of the DUT */
        EditARPField(&ARP_P2,ARP,sender_hardware_address,(void*)ARP_config.MAC_ADDR2);
        //EditARPField(&ARP_P2,ETHERNET,ETH_sourceMac,(void*)mac_addr2);
        SendARP(ARP_P2);
        /*wait <ARP-TOLERANCE-TIME> seconds for the arp cache to get refreshed*/
        sleep(ARP_config.ARP_TOLERANCE_TIME); 
        /* Configure DUT to send a ICMP Request Message to TESTER */
        ip4addr ipv4tester;
        ipv4tester.dataLength = 4;
	    stoiIP((char*)ARP_config.HOST_1_IP,ipv4tester.Data);
        char dataicmp[] = "Test_ARP_33";
        vint8 data_to_send;
        data_to_send.Data = (uint8* )dataicmp;
        data_to_send.dataLength = 12;
        char interf[] = "eth0";
        text text_to_send;
        text_to_send.dataLength = 7;
        text_to_send.Data = (uint8 *)interf;
        Packet_filter f;
        strcpy(f.SrcAdd ,(char*)ARP_config.DIface_0_IP );
        strcpy(f.dstAdd ,(char*)ARP_config.HOST_1_IP);
        TP_IcmpEchoRequest(ECHO_REQUEST_Cb_Client,text_to_send,ipv4tester,data_to_send);
        /* TESTER: <HOST-1> Listens (up to <ParamListenTime>) on <DIface-0> */
        ICMP_Packet ICMP_PR = ListenICMP(f,ARP_config.ParamListenTime); /* ParamListenTime = 3s */
        while(Callback_result == 2) {usleep(100);}
        if(Callback_result == 0)
        {   
            Callback_result = 2;
            if(ICMP_PR.length !=0 )
            {   
                char MAC2[18];
                lower_to_upper_mac(ARP_config.MAC_ADDR2,MAC2);
                if(strcmp((char *)GetICMPField(&ICMP_PR,ETHERNET,ETH_destinationMAC),(char*)MAC2)==0)
                {
                    printf("\n********test ARP_33 passed********\n");
                    Arp_33_Test_result = 0;
                }
                else
                {
                    printf("\n*********test ARP_33 failed : Ethernet Destination Address != <MAC-ADDR2>*******\n");
                } 
            }
            else
            {
                printf("\n********test ARP_33 failed********\n");
            }
        }
        else
        {
            printf("\n\n ******** test ARP_33 failed : TP_IcmpEchoRequest ********** \n\n");
        }
    }
    else
    {
        printf("\n*******test ARP_33 failed : TP_ClearArpCache*******\n");
    }
    TP_GenaralEndTest(GenaralEndTest_Cb_Client,0,(text){0});
       
    return Arp_33_Test_result;
}

int ARP_34()
{   
    Callback_result = 2;
    int Arp_34_Test_result = 1;
      
    TP_GenaralStartTest(GenaralStartTest_Cb_Client);
    /* Configure DUT to clear the dynamic entries in its ARP Cache */
    TP_ClearArpCache(ARP_CLEAR_CACHE_Cb_Client);
    while(Callback_result == 2);
    if(Callback_result == 0) /* arp cache cleared */
    {   
        Callback_result = 2;
        /*TESTER:  <HOST-1> Sends ARP request to DUT through <DIface-0>:containing:
            -ARP Sender Hardware Address set to <MAC-ADDR1>*/
        ARP_Packet ARP_P1 = CreateARP();
        SendARP(ARP_P1);
        /*TESTER:  <HOST-1> Sends ARP Response to DUT through <DIface-0>:containing:
            -ARP Sender Hardware Address set to <MAC-ADDR2>*/
        ARP_Packet ARP_P2 = CreateARP();
        EditARPField(&ARP_P2,ARP,opcode,(void*)ARP_config.OPERATION_RESPONSE);   /* ARP Response */
        EditARPField(&ARP_P2,ETHERNET,ETH_destinationMAC,(void*)ARP_config.DIFACE_O_MAC_ADDR); /* set the mac address of the DUT */
        EditARPField(&ARP_P2,ARP,sender_hardware_address,(void*)(char*)ARP_config.MAC_ADDR2);
        //EditARPField(&ARP_P2,ETHERNET,ETH_sourceMac,(void*)mac_addr2);
        SendARP(ARP_P2);
        /*wait <ARP-TOLERANCE-TIME> seconds for the arp cache to get refreshed*/
        sleep(ARP_config.ARP_TOLERANCE_TIME); 
        /* Configure DUT to send a ICMP Request Message to TESTER */
        ip4addr ipv4tester;
        ipv4tester.dataLength = 4;
	    stoiIP((char*)ARP_config.HOST_1_IP,ipv4tester.Data);
        char dataicmp[] = "Test_ARP_34";
        vint8 data_to_send;
        data_to_send.Data = (uint8* )dataicmp;
        data_to_send.dataLength = 12;
        char interf[] = "eth0";
        text text_to_send;
        text_to_send.dataLength = 7;
        text_to_send.Data = (uint8 *)interf;
        Packet_filter f;
        strcpy(f.SrcAdd ,(char*)ARP_config.DIface_0_IP );
        strcpy(f.dstAdd ,(char*)ARP_config.HOST_1_IP);
        TP_IcmpEchoRequest(ECHO_REQUEST_Cb_Client,text_to_send,ipv4tester,data_to_send);
        ICMP_Packet ICMP_PR = ListenICMP(f,ARP_config.ParamListenTime); /* ParamListenTime = 3s */
        while(Callback_result == 2) {usleep(100);}
        if(Callback_result == 0)
        {   
            Callback_result = 2;
            if(ICMP_PR.length !=0 )
            {   
                char MAC2[18];
                lower_to_upper_mac(ARP_config.MAC_ADDR2,MAC2);
                if(strcmp((char *)GetICMPField(&ICMP_PR,ETHERNET,ETH_destinationMAC),(char*)MAC2)==0)
                {
                    printf("\n********test ARP_34 passed********\n");
                    Arp_34_Test_result = 0;
                }
                else
                {
                    printf("\n*********test ARP_34 failed : Ethernet Destination Address != <MAC-ADDR2>*******\n");
                } 
            }
            else
            {
                printf("\n********test ARP_34 failed********\n");
            }
        }
        else
        {
            printf("\n\n ******** test ARP_34 failed : TP_IcmpEchoRequest ********** \n\n");
        }
    }
    else
    {
        printf("\n*******test ARP_34 failed : TP_ClearArpCache*******\n");
    }
    TP_GenaralEndTest(GenaralEndTest_Cb_Client,0,(text){0});
       
    return Arp_34_Test_result;
}

int ARP_35()
{   
    Callback_result = 2;
    int Arp_35_Test_result = 1;
      
    TP_GenaralStartTest(GenaralStartTest_Cb_Client);
    /* Configure DUT to clear the dynamic entries in its ARP Cache */
    TP_ClearArpCache(ARP_CLEAR_CACHE_Cb_Client);
    while(Callback_result == 2);
    if(Callback_result == 0) /* arp cache cleared */
    {   
        Callback_result = 2;
        /*TESTER:  <HOST-1> Sends ARP response to DUT through <DIface-0>:containing:
            -ARP Sender Hardware Address set to <MAC-ADDR1>*/
        ARP_Packet ARP_P1 = CreateARP();
        EditARPField(&ARP_P1,ARP,opcode,(void*)ARP_config.OPERATION_RESPONSE);   /* ARP Response */
        EditARPField(&ARP_P1,ETHERNET,ETH_destinationMAC,(void*)ARP_config.DIFACE_O_MAC_ADDR); /* set the mac address of the DUT */
        SendARP(ARP_P1);
        /*TESTER:  <HOST-1> Sends ARP Request to DUT through <DIface-0>:containing:
            -ARP Sender Hardware Address set to <MAC-ADDR2>*/
        ARP_Packet ARP_P2 = CreateARP();
        EditARPField(&ARP_P2,ARP,sender_hardware_address,(void*)(char*)ARP_config.MAC_ADDR2);
        //EditARPField(&ARP_P2,ETHERNET,ETH_sourceMac,(void*)mac_addr2);
        SendARP(ARP_P2);
        /*wait <ARP-TOLERANCE-TIME> seconds for the arp cache to get refreshed*/
        sleep(ARP_config.ARP_TOLERANCE_TIME); 
        /* Configure DUT to send a ICMP Request Message to TESTER */
        ip4addr ipv4tester;
        ipv4tester.dataLength = 4;
	    stoiIP((char*)ARP_config.HOST_1_IP,ipv4tester.Data);
        char dataicmp[] = "Test_ARP_35";
        vint8 data_to_send;
        data_to_send.Data = (uint8* )dataicmp;
        data_to_send.dataLength = 12;
        char interf[] = "eth0";
        text text_to_send;
        text_to_send.dataLength = 7;
        text_to_send.Data = (uint8 *)interf;
        Packet_filter f;
        strcpy(f.SrcAdd ,(char*)ARP_config.DIface_0_IP );
        strcpy(f.dstAdd ,(char*)ARP_config.HOST_1_IP);
        TP_IcmpEchoRequest(ECHO_REQUEST_Cb_Client,text_to_send,ipv4tester,data_to_send);
        ICMP_Packet ICMP_PR = ListenICMP(f,ARP_config.ParamListenTime); /* ParamListenTime = 3s */
        while(Callback_result == 2) {usleep(100);}
        if(Callback_result == 0)
        {   
            Callback_result = 2;
            if(ICMP_PR.length !=0 )
            {   
                char MAC2[18];
                lower_to_upper_mac(ARP_config.MAC_ADDR2,MAC2);
                if(strcmp((char *)GetICMPField(&ICMP_PR,ETHERNET,ETH_destinationMAC),(char*)MAC2)==0)
                {
                    printf("\n********test ARP_35 passed********\n");
                    Arp_35_Test_result = 0;
                }
                else
                {
                    printf("\n*********test ARP_35 failed : Ethernet Destination Address != <MAC-ADDR2>*******\n");
                } 
            }
            else
            {
                printf("\n********test ARP_35 failed********\n");
            }
        }
        else
        {
            printf("\n\n ******** test ARP_35 failed : TP_IcmpEchoRequest ********** \n\n");
        }
    }
    else
    {
        printf("\n*******test ARP_35 failed : TP_ClearArpCache*******\n");
    }
    TP_GenaralEndTest(GenaralEndTest_Cb_Client,0,(text){0});
       
    return Arp_35_Test_result;
}

int ARP_36()
{   
    Callback_result = 2;
    int Arp_36_Test_result = 1;
      
    TP_GenaralStartTest(GenaralStartTest_Cb_Client);
    /* Configure DUT to clear the dynamic entries in its ARP Cache */
    TP_ClearArpCache(ARP_CLEAR_CACHE_Cb_Client);
    while(Callback_result == 2){usleep(100);}
    if(Callback_result == 0) /* arp cache cleared */
    {   
        Callback_result = 2;
        Packet_filter f;
        f.arp_operation = 0x0002;   // arp reply
        strcpy(f.spa,(char*)ARP_config.DIface_0_IP ); // address ip of DUT
        strcpy(f.tpa,(char*)ARP_config.HOST_1_IP);
        /*TESTER then sends an ARP Request with target protocol address field set to <DIface-0-IP>*/
        ARP_Packet ARP_P = CreateARP();
        SendARP(ARP_P);
        /*  Tester Listens (up to <ParamListenTime>) on <DIface-0> */
        ARP_Packet ARP_PR = ListenARP(f,ARP_config.ParamListenTime);
        if(ARP_PR.length != 0 )
        {
            printf("\n********test ARP_36 passed********\n");
            Arp_36_Test_result = 0;
        }
        else
        {
            printf("\n********test ARP_36 failed: no ARP reply recieved********\n");
        } 
    }
    else
    {
        printf("\n*******test ARP_36 failed : TP_ClearArpCache*******\n");
    }
    TP_GenaralEndTest(GenaralEndTest_Cb_Client,0,(text){0});
        
    return Arp_36_Test_result;
}
    

int ARP_37()
{   
    Callback_result = 2;
    int Arp_37_Test_result = 1;
      
    TP_GenaralStartTest(GenaralStartTest_Cb_Client);
    /* Configure DUT to clear the dynamic entries in its ARP Cache */
    TP_ClearArpCache(ARP_CLEAR_CACHE_Cb_Client);
    while(Callback_result == 2);
    if(Callback_result == 0) /* arp cache cleared */
    {   
        Callback_result = 2;
        Packet_filter f;
        f.arp_operation = 0x0002;   // arp reply
        strcpy(f.spa,(char*)ARP_config.DIface_0_IP ); // address ip of DUT
        strcpy(f.tpa,(char*)ARP_config.HOST_1_IP);
        /*TESTER then sends an ARP Request with target protocol address field set to  <IP-FIRST-UNUSED-ADDR-INTERFACE-1>*/
        ARP_Packet ARP_P = CreateARP();
        EditARPField(&ARP_P,ARP,target_protocol_address,(void*)ARP_config.IP_FIRST_UNUSED_ADDR_INTERFACE_1);
        SendARP(ARP_P);
        /*Tester Listens (up to <ParamListenTime>) on <DIface-0> */
        ARP_Packet ARP_PR = ListenARP(f,ARP_config.ParamListenTime);
        if(ARP_PR.length == 0 )
        {
            printf("\n********test ARP_37 passed********\n");
            Arp_37_Test_result = 0;
        }
        else
        {
            printf("\n********test ARP_37 failed: no ARP reply recieved********\n");
        } 
    }
    else
    {
        printf("\n*******test ARP_37 failed : TP_ClearArpCache*******\n");
    }
    TP_GenaralEndTest(GenaralEndTest_Cb_Client,0,(text){0});
         
    return Arp_37_Test_result;
}

int ARP_38()
{   
    Callback_result = 2;
    int Arp_38_Test_result = 1;
      
    TP_GenaralStartTest(GenaralStartTest_Cb_Client);
    /* Configure DUT to clear the dynamic entries in its ARP Cache */
    TP_ClearArpCache(ARP_CLEAR_CACHE_Cb_Client);
    while(Callback_result == 2);
    if(Callback_result == 0) /* arp cache cleared */
    {   
        Callback_result = 2;
        Packet_filter f;
        f.arp_operation = 0x0001;   // arp request
        strcpy(f.spa,(char*)ARP_config.DIface_0_IP ); // address ip of DUT
        strcpy(f.tpa,(char*)ARP_config.HOST_1_IP);
        /*TESTER then sends an ARP Request with target protocol address field set to <DIface-0-IP>*/
        ARP_Packet ARP_P = CreateARP();
        EditARPField(&ARP_P,ARP,opcode,(void*)ARP_config.OPERATION_RESPONSE);
        EditARPField(&ARP_P,ETHERNET,ETH_destinationMAC,(void*)ARP_config.DIFACE_O_MAC_ADDR); /* set the mac address of the DUT */
        EditARPField(&ARP_P,ARP,target_protocol_address,(void*)ARP_config.IP_FIRST_UNUSED_ADDR_INTERFACE_1);
        SendARP(ARP_P);
        /* TESTER: <HOST-1> Waits up to (1) second(s) for the ARP cache of DUT to get refreshed*/
        sleep(1);
        /* Configure DUT to send a ICMP Request Message to TESTER */
        ip4addr ipv4tester;
        ipv4tester.dataLength = 4;
	    stoiIP((char*)ARP_config.HOST_1_IP,ipv4tester.Data);
        char dataicmp[] = "Test_ARP_38";
        vint8 data_to_send;
        data_to_send.Data = (uint8* )dataicmp;
        data_to_send.dataLength = 12;
        char interf[] = "eth0";
        text text_to_send;
        text_to_send.dataLength = 7;
        text_to_send.Data = (uint8 *)interf;
        auto future = std::async(ListenARP,f,ARP_config.ParamListenTime);
        TP_IcmpEchoRequest(ECHO_REQUEST_Cb_Client,text_to_send,ipv4tester,data_to_send);
        /*Tester Listens (up to <ParamListenTime>) on <DIface-0> */
        ARP_Packet ARP_PR = future.get();
        while(Callback_result == 2){usleep(100);}
        if(Callback_result == 0)
        {

            if(ARP_PR.length != 0 )
            {
                printf("\n********test ARP_38 passed********\n");
                Arp_38_Test_result = 0;
            }
            else
            {
                printf("\n********test ARP_38 failed: no ARP reply recieved********\n");
            }
        }
        else
        {
            printf("\n******** test ARP_38 failed : TP_IcmpEchoRequest **********\n");
        } 
    }
    else
    {
        printf("\n*******test ARP_38 failed : TP_ClearArpCache*******\n");
    }
    TP_GenaralEndTest(GenaralEndTest_Cb_Client,0,(text){0});
         
    return Arp_38_Test_result;
}

int ARP_39()
{   
    Callback_result = 2;
    int Arp_39_Test_result1 = 1;
    int Arp_39_Test_result2 = 1;
      
    TP_GenaralStartTest(GenaralStartTest_Cb_Client);

    /* Configure DUT to clear the dynamic entries in its ARP Cache */
    TP_ClearArpCache(ARP_CLEAR_CACHE_Cb_Client);
    while(Callback_result == 2);
    if(Callback_result == 0) /* arp cache cleared */
    {
        Callback_result = 2;
        /* Configure DUT to send a ICMP Message from <DIface-0> */ 
        ip4addr ipv4tester;
        ipv4tester.dataLength = 4;
	    stoiIP((char*)ARP_config.HOST_1_IP,ipv4tester.Data);
        char dataicmp[] = "Test_ARP_39";
        vint8 data_to_send;
        data_to_send.Data = (uint8* )dataicmp;
        data_to_send.dataLength = 12;
        char interf[] = "eth0";
        text text_to_send;
        text_to_send.dataLength = 7;
        text_to_send.Data = (uint8 *)interf;
        Packet_filter f;
        f.arp_operation = 0x0001;   // arp reply
        strcpy(f.spa,(char*)ARP_config.DIface_0_IP ); // address ip of DUT
        strcpy(f.tpa,(char*)ARP_config.HOST_1_IP);
        auto future = std::async(ListenARP,f,ARP_config.ParamListenTime);
        TP_IcmpEchoRequest(ECHO_REQUEST_Cb_Client,text_to_send,ipv4tester,data_to_send);
        /* Listens (up to <ParamListenTime>) on <DIface-0> */
        ARP_Packet ARP_PR = future.get();
        while(Callback_result ==2){usleep(100);}
        if(Callback_result ==0)
        {
            Callback_result == 2;
            if((ARP_PR.length != 0))
            {
                printf("\n********Test ARP_39 part1 Passed********\n");
                Arp_39_Test_result1 = 0;
            }
            else
            {
                printf("\n*******Test ARP_39 Part1 failed: no ARP request seen*******\n");
            }
            /*  Sends ARP Request to DUT through <DIface-0> containing :  ARP Sender Hardware Address set to <MAC-ADDR2>*/
            ARP_Packet ARP_P = CreateARP();
            EditARPField(&ARP_P,ARP,sender_hardware_address,(void*)ARP_config.MAC_ADDR2);
            SendARP(ARP_P);
            /* Waits up to (<ARP-TOLERANCE-TIME>) second(s) for the arp cache of DUT to get refreshed */
            sleep(ARP_config.ARP_TOLERANCE_TIME);
            /* Configure DUT to send a ICMP Message from <DIface-0> */
            TP_IcmpEchoRequest(ECHO_REQUEST_Cb_Client,text_to_send,ipv4tester,data_to_send);
            /*  Listens (up to <ParamListenTime>) on <DIface-0> */
            ICMP_Packet ICMP_PR = ListenICMP(f,ARP_config.ParamListenTime); /* ParamListenTime = 3s */
            while(Callback_result == 2){usleep(100);}
            if(Callback_result ==0)
            {
                if(ICMP_PR.length !=0)
                {   
                    char MAC2[18];
                    lower_to_upper_mac(ARP_config.MAC_ADDR2,MAC2);
                    if(strcmp((char *)GetICMPField(&ICMP_PR,ETHERNET,ETH_destinationMAC),(char*)MAC2)==0)
                    {
                        printf("\n*********test ARP_39 Part2 passed*******\n");
                        Arp_39_Test_result2 = 0;
                    }
                    else
                    {
                        printf("\n********test ARP_39 Part2 failed : Ethernet Destination Address != <MAC-ADDR2>*******\n");
                    }   
                }
                else
                {
                    printf("\n*********test ARP_39 failed : No ICMP packet received********\n");
                }
            }
            else
            {
                printf("\n******** test ARP_39 Part2 failed : TP_IcmpEchoRequest **********\n");
            }
        }
        else
        {
            printf("\n******** test ARP_39 Part1 failed : TP_IcmpEchoRequest **********\n");
        }
    }
    else
    {
        printf("\n*******test ARP_39 failed : TP_ClearArpCache*******\n");
    }
    TP_GenaralEndTest(GenaralEndTest_Cb_Client,0,(text){0});
       
    return Arp_39_Test_result1||Arp_39_Test_result2;
}   

int ARP_40()
{   
    Callback_result = 2;
    int Arp_40_Test_result1 = 1;
    int Arp_40_Test_result2 = 1;
      
    TP_GenaralStartTest(GenaralStartTest_Cb_Client);
    /* Configure DUT to clear the dynamic entries in its ARP Cache */
    TP_ClearArpCache(ARP_CLEAR_CACHE_Cb_Client);
    while(Callback_result == 2){usleep(100);}
    if(Callback_result == 0) /* arp cache cleared */
    {   
        Callback_result = 2;
        /* Configure DUT to send a ICMP Message from <DIface-0> */ 
        ip4addr ipv4tester;
        ipv4tester.dataLength = 4;
	    stoiIP((char*)ARP_config.HOST_1_IP,ipv4tester.Data);
        char dataicmp[] = "Test_ARP_40";
        vint8 data_to_send;
        data_to_send.Data = (uint8* )dataicmp;
        data_to_send.dataLength = 12;
        char interf[] = "eth0";
        text text_to_send;
        text_to_send.dataLength = 7;
        text_to_send.Data = (uint8 *)interf;
        Packet_filter f;
        f.arp_operation = 0x0001;   // arp reqest
        strcpy(f.spa,(char*)ARP_config.DIface_0_IP ); // address ip of DUT
        strcpy(f.tpa,(char*)ARP_config.HOST_1_IP);
        auto future = std::async(ListenARP,f,ARP_config.ParamListenTime);
        TP_IcmpEchoRequest(ECHO_REQUEST_Cb_Client,text_to_send,ipv4tester,data_to_send);
        /* Listens (up to <ParamListenTime>) on <DIface-0> */
        ARP_Packet ARP_PR = future.get();
        while(Callback_result == 2){usleep(100);}
        if(Callback_result == 0)
        {
            Callback_result = 2;
            if((ARP_PR.length != 0))
            {
                printf("\n********Test ARP_40 Part1 Passed*********\n");
                Arp_40_Test_result1 = 0;
            }
            else
            {
                printf("\n*********Test ARP_40 Part1 failed: no ARP request seen*********\n");
            }
            /*  Sends ARP response to DUT through <DIface-0> containing :  ARP Sender Hardware Address set to <MAC-ADDR2>*/
            ARP_Packet ARP_P = CreateARP();
            EditARPField(&ARP_P,ARP,opcode,(void*)ARP_config.OPERATION_RESPONSE);
            EditARPField(&ARP_P,ETHERNET,ETH_destinationMAC,(void*)ARP_config.DIFACE_O_MAC_ADDR); /* set the mac address of the DUT */
            EditARPField(&ARP_P,ARP,sender_hardware_address,(void*)ARP_config.MAC_ADDR3);
            SendARP(ARP_P);
            /* Waits up to (<ARP-TOLERANCE-TIME>) second(s) for the arp cache of DUT to get refreshed */
            sleep(ARP_config.ARP_TOLERANCE_TIME);
            /* Configure DUT to send a ICMP Message from <DIface-0> */
            TP_IcmpEchoRequest(ECHO_REQUEST_Cb_Client,text_to_send,ipv4tester,data_to_send);
            /*  Listens (up to <ParamListenTime>) on <DIface-0> */
            ICMP_Packet ICMP_PR = ListenICMP(f,ARP_config.ParamListenTime); /* ParamListenTime = 3s */
            while(Callback_result == 2){usleep(100);}
            if(Callback_result == 0)
            {
                Callback_result = 0;
                if(ICMP_PR.length !=0 )
                {   
                    char MAC3[18];
                    lower_to_upper_mac(ARP_config.MAC_ADDR3,MAC3);
                    if(strcmp((char *)GetICMPField(&ICMP_PR,ETHERNET,ETH_destinationMAC),(char*)MAC3)==0)
                    {
                        printf("\n*********test ARP_40 Part 2passed*********\n");
                        Arp_40_Test_result2 = 0;
                    }
                    else
                    {
                        printf("\n***********test ARP_40 Part2 failed : Ethernet Destination Address != <MAC-ADDR2>*************\n");
                    }
                }
                else
                {
                    printf("\n**********test ARP_40 failed : No ICMP packet received***********\n");
                }
            }
            else
            {
                printf("\n******** test ARP_40 failed : TP_IcmpEchoRequest **********\n");
            }
        }
        else
        {
            printf("\n******** test ARP_40 failed : TP_IcmpEchoRequest **********\n");
        }
    }
    else
    {
        printf("\n*******test ARP_40 failed : TP_ClearArpCache*******\n");
    }
    TP_GenaralEndTest(GenaralEndTest_Cb_Client,0,(text){0});
       
    return Arp_40_Test_result1||Arp_40_Test_result2;
}

int ARP_41()
{  
    int Arp_41_Test_result = 1; 
    Packet_filter f;
    f.arp_operation = 0x0002;   // arp reply
    strcpy(f.spa,(char*)ARP_config.DIface_0_IP ); // address ip of DUT
    strcpy(f.tpa,(char*)ARP_config.HOST_1_IP);
    /* ESTER: <HOST-1> Sends ARP Request to DUT through <DIface-0> */
    ARP_Packet ARP_P = CreateARP();
    SendARP(ARP_P);
    /*  TESTER: <HOST-1> Listens (up to <ParamListenTime>) on <DIface-0> */
    ARP_Packet ARP_PR = ListenARP(f,ARP_config.ParamListenTime);
    if((ARP_PR.length != 0))
    {
        if((uint16_t)GetARPField(&ARP_PR,ARP,opcode) == ARP_config.OPERATION_RESPONSE)  /* OPERATION_Response */
        {
            printf("\n********Test ARP_41 Passed**********\n");
            Arp_41_Test_result = 0;
        }
        else
        {
             printf("\n********Test ARP_41 failed: code operation != OPERATION_Response*******\n");
        }
    }
    else
    {
        printf("\n********Test ARP_41 failed: No received packet********\n");
    }
    return Arp_41_Test_result;
}

int ARP_42()
{  
    int Arp_42_Test_result = 1; 
    Packet_filter f;
    f.arp_operation = 0x0002;   // arp reply
    strcpy(f.spa,(char*)ARP_config.DIface_0_IP ); // address ip of DUT
    strcpy(f.tpa,(char*)ARP_config.HOST_1_IP);
    /* TESTER: <HOST-1> Sends ARP Response to DUT through <DIface-0>*/
    ARP_Packet ARP_P = CreateARP();
    EditARPField(&ARP_P,ARP,opcode,(void*)ARP_config.OPERATION_RESPONSE);
    EditARPField(&ARP_P,ETHERNET,ETH_destinationMAC,(void*)ARP_config.DIFACE_O_MAC_ADDR); /* set the mac address of the DUT */
    SendARP(ARP_P);
    /* TESTER: <HOST-1> Listens (up to <ParamListenTime>) on <DIface-0> */
    ARP_Packet ARP_PR = ListenARP(f,ARP_config.ParamListenTime);
    if((ARP_PR.length != 0))
    {
        printf("\n******Test ARP_42 Failed : ARP response packet received******\n");
    }
    else
    {
        printf("\n*******Test ARP_42 passed: No arp response received packet*******\n");
        Arp_42_Test_result = 0;
    }
    return Arp_42_Test_result;
}

int ARP_43()
{   
    int Arp_43_Test_result = 1;
    Packet_filter f;
    f.arp_operation = 0x0002;   // arp reply
    strcpy(f.spa,(char*)ARP_config.DIface_0_IP ); // address ip of DUT
    strcpy(f.tpa,(char*)ARP_config.HOST_1_IP);
    /* TESTER: <HOST-1> Sends ARP Request to DUT through <DIface-0> */
    ARP_Packet ARP_P = CreateARP();
    SendARP(ARP_P);
    /*  TESTER: <HOST-1> Listens (up to <ParamListenTime>) on <DIface-0> */ 
    ARP_Packet ARP_PR = ListenARP(f,ARP_config.ParamListenTime);
    if((ARP_PR.length != 0))
    {   
        char * eth_src;
        eth_src = (char *)GetARPField(&ARP_PR,ETHERNET,ETH_sourceMac);
        char MAC_DUT[18];
        lower_to_upper_mac(ARP_config.DIFACE_O_MAC_ADDR,MAC_DUT);
        if(strcmp(eth_src, (char*)MAC_DUT)==0)  /*  Ethernet Source Hardware Address */
        {
            printf("\n*******Test ARP_43 Passed*******\n");
            Arp_43_Test_result = 0;
        }
        else
        {
            printf("\n*******Test ARP_43 failed: Ethernet Source Hardware Address %s != DIFACE-0-MAC-ADDR %s******\n",eth_src,ARP_config.DIFACE_O_MAC_ADDR);
        }
    }
    else
    {
        printf("\n*******Test ARP_43 failed: No arp response packet received*******\n");
    }
    return Arp_43_Test_result;
}

int ARP_44()
{   
    int Arp_44_Test_result = 1;
    Packet_filter f;
    f.arp_operation = 0x0002;   // arp reply
    strcpy(f.spa,(char*)ARP_config.DIface_0_IP ); // address ip of DUT
    strcpy(f.tpa,(char*)ARP_config.HOST_1_IP);
    /* TESTER: <HOST-1> Sends ARP Request to DUT through <DIface-0> */
    ARP_Packet ARP_P = CreateARP();
    SendARP(ARP_P);
    /* TESTER: <HOST-1> Listens (up to <ParamListenTime>) on <DIface-0> */
    ARP_Packet ARP_PR = ListenARP(f,ARP_config.ParamListenTime);
    if((ARP_PR.length != 0))
    {   
        char * arp_spa;
        arp_spa = (char *)GetARPField(&ARP_PR,ARP,sender_protocol_address);
        if(strcmp(arp_spa, (char*)ARP_config.DIface_0_IP ) == 0)  /*  DIface-0-IP */
        {
            printf("\n******Test ARP_44 Passed*******\n");
            Arp_44_Test_result = 0;
        }
        else
        {
            printf("\n******Test ARP_44 failed:  Sender IP Address != DIface-0-IP:%s******\n",arp_spa);
        }
    }
    else
    {
        printf("\n*******Test 44 failed: No arp response packet received******\n");
    }
    return Arp_44_Test_result;
}

int ARP_45()
{   
    Callback_result = 2;
    int Arp_45_Test_result1 = 1;
    int Arp_45_Test_result2 = 1;
      
    TP_GenaralStartTest(GenaralStartTest_Cb_Client);

    /* Configure DUT to clear the dynamic entries in its ARP Cache */
    TP_ClearArpCache(ARP_CLEAR_CACHE_Cb_Client);
    while(Callback_result == 2);
    if(Callback_result == 0) /* arp cache cleared */
    {   
        Callback_result = 2;
        Packet_filter f;
        f.arp_operation = 0x0002;   // arp reply
        strcpy(f.spa,(char*)ARP_config.DIface_0_IP ); // address ip of DUT
        strcpy(f.tpa,(char*)ARP_config.HOST_1_IP);
        /* Sends ARP Request to DUT through <DIface-0> */
        ARP_Packet ARP_P = CreateARP();
        SendARP(ARP_P);
        /*  Listens (up to <ParamListenTime>) on <DIface-0> */
        ARP_Packet ARP_PR = ListenARP(f,ARP_config.ParamListenTime);
        if((ARP_PR.length != 0))
        {   
            char * arp_tha;
            arp_tha = (char *)GetARPField(&ARP_PR,ARP,target_hardware_address);
            /* transfer the mac addr from lower to upper case*/    
            char MAC1[18];
            lower_to_upper_mac(ARP_config.MAC_ADDR1,MAC1);
            if(strcmp(arp_tha, (char*)MAC1) == 0)  /*  MAC-ADDR1 */
            {
                printf("\n*******Test ARP_45 Part1 Passed*******\n");
                Arp_45_Test_result1 = 0;
            }
            else
            {
                printf("\n********Test ARP_45 Part1 failed: Target Hardware Address != MAC-ADDR1:%s********\n",arp_tha);
            }
        }
        else
        {
            printf("\n********Test ARP_45 Part1 failed: No arp response packet received*******\n");
        }
        /* Sends ARP Request to DUT through <DIface-0> containing  ARP Sender Hardware Address set to <MAC-ADDR2>*/
        ARP_Packet ARP_P2 = CreateARP();
        EditARPField(&ARP_P2,ARP,sender_hardware_address,(void*)ARP_config.MAC_ADDR2);
        SendARP(ARP_P2);
        /* Listens (up to <ParamListenTime>) on <DIface-0> */
        ARP_Packet ARP_PR2 = ListenARP(f,ARP_config.ParamListenTime);
        if(ARP_PR2.length != 0)
        {  
            char * arp_tha;
            arp_tha = (char *)GetARPField(&ARP_PR2,ARP,target_hardware_address);
            char MAC2[18];
            lower_to_upper_mac(ARP_config.MAC_ADDR2,MAC2);
            if(strcmp(arp_tha, (char*)MAC2) == 0)  /*  MAC-ADDR2 */
            {
                printf("\n******Test ARP_45 Part2 Passed******\n");
                Arp_45_Test_result2 = 0;
            }
            else
            {
                printf("\n*******Test ARP_45 part2 failed: Target Hardware Address != MAC-ADDR2:%s*******\n",arp_tha);
            }
        }
        else
        {
            printf("\n*******Test ARP_45 part2 failed: No arp response packet received*******\n");
        }
    }
    else
    {
        printf("\n*******test ARP_45 failed : TP_ClearArpCache*******\n");
    }
    TP_GenaralEndTest(GenaralEndTest_Cb_Client,0,(text){0});
       
    return Arp_45_Test_result1||Arp_45_Test_result2;
}

int ARP_46()
{  
    int Arp_46_Test_result = 1; 
    Packet_filter f;
    f.arp_operation = 0x0002;   // arp reply
    strcpy(f.spa,(char*)ARP_config.DIface_0_IP ); // address ip of DUT
    strcpy(f.tpa,(char*)ARP_config.HOST_1_IP);
    /*TESTER: <HOST-1> Sends ARP Request to DUT through <DIface-0> */
    ARP_Packet ARP_P = CreateARP();
    SendARP(ARP_P);
    /* TESTER: <HOST-1> Listens (up to <ParamListenTime>) on <DIface-0> */
    ARP_Packet ARP_PR = ListenARP(f,ARP_config.ParamListenTime);
    if((ARP_PR.length != 0))
    {   
        uint16_t s = (uint16_t)GetARPField(&ARP_PR,ARP,hardware_address_format);
        if( s == 0x0001)  /*   ARP_HARDWARE_ETHERNET*/
        {
            printf("\n********Test ARP_46 Passed********\n");
            Arp_46_Test_result = 0;
        }
        else
        {
            printf("\n**********Test ARP_46 failed:  Hardware Type !=  ARP_HARDWARE_ETHERNET********\n");
        }
    }
    else
    {
        printf("\n********Test ARP_46 failed: No arp response packet received********\n");
    }
    return Arp_46_Test_result;
}

int ARP_47()
{   
    int Arp_47_Test_result = 1;
    /* TESTER: <HOST-1> Sends ARP Request to DUT through <DIface-0> */
    ARP_Packet ARP_P = CreateARP();
    SendARP(ARP_P);
    /* TESTER: <HOST-1> Listens (up to <ParamListenTime>) on <DIface-0> */
    Packet_filter f;
    f.arp_operation = 0x0002;   // arp reply
    strcpy(f.spa,(char*)ARP_config.DIface_0_IP ); // address ip of DUT
    strcpy(f.tpa,(char*)ARP_config.HOST_1_IP);
    ARP_Packet ARP_PR = ListenARP(f,ARP_config.ParamListenTime);
    /* TESTER: Verify that received ARP Response contains:
        - Hardware Address Length is set to <ETHERNET_ADDR_LEN>*/
    if((ARP_PR.length != 0))
    {   
        uint8_t s = (uint8_t)GetARPField(&ARP_PR,ARP,Length_hardware_address);
        if( s == 6)  /*   ETHERNET_ADDR_LEN */
        {
            printf("\n********Test ARP_47 Passed*********\n");
            Arp_47_Test_result = 0;
        }
        else
        {
            printf("\n**********Test ARP_47 failed:   Hardware Address Length !=  ARP_HARDWARE_ETHERNET***********\n");
        }
    }
    else
    {
        printf("\n********Test ARP_47 failed: No arp response packet received************\n");
    }
    return Arp_47_Test_result;
}

int ARP_48()
{   
    Callback_result = 2;
    int Arp_48_Test_result = 1;
      
    TP_GenaralStartTest(GenaralStartTest_Cb_Client);

    /* Configure DUT to clear the dynamic entries in the ARP Cache */
    TP_ClearArpCache(ARP_CLEAR_CACHE_Cb_Client);
    while(Callback_result == 2);
    if(Callback_result == 0) /* arp cache cleared */
    {   
        Callback_result = 2;
        /* DUT CONFIGURE: Configure DUT to set a timeout of
        <DYNAMIC-ARP-CACHE-TIMEOUT> seconds
        for the dynamic entries in the ARP Cache of <DIface-0> */

        /* TESTER:  <HOST-1> Sends ARP Request to DUT through <DIface-0> */
        ARP_Packet ARP_P = CreateARP();
        SendARP(ARP_P);
        /* TESTER: <HOST-1> Waits up to (<ARP-TOLERANCE-TIME>) second(s) for the
        ARP cache of DUT to get refreshed */
        sleep(ARP_config.ARP_TOLERANCE_TIME);
        /* Configure DUT to send a ICMP Request Message to TESTER */
        ip4addr ipv4tester;
        ipv4tester.dataLength = 4;
        stoiIP((char*)ARP_config.HOST_1_IP,ipv4tester.Data);
        char dataicmp[] = "Test_ARP_48";
        vint8 data_to_send;
        data_to_send.Data = (uint8* )dataicmp;
        data_to_send.dataLength = 12;
        char interf[] = "eth0";
        text text_to_send;
        text_to_send.dataLength = 7;
        text_to_send.Data = (uint8 *)interf;
        Packet_filter f;
        strcpy(f.SrcAdd,(char*)ARP_config.DIface_0_IP );
        strcpy(f.dstAdd,(char*)ARP_config.HOST_1_IP);
        TP_IcmpEchoRequest(ECHO_REQUEST_Cb_Client,text_to_send,ipv4tester,data_to_send);
        /* ESTER: <HOST-1> Listens (up to <ParamListenTime>) on <DIface-0> */
        ICMP_Packet ICMP_PR = ListenICMP(f,ARP_config.ParamListenTime);
        while(Callback_result == 2){usleep(100);}
        if(Callback_result == 0) /* arp cache cleared */
        {   
            Callback_result = 2;
            if(ICMP_PR.length !=0 )
            {       
                printf("\n***********ICMP request received**************\n");
                /* TESTER: <HOST-1> Waits up to (<DYNAMIC-ARP-CACHE-TIMEOUT> + <ARPTOLERANCE-TIME>)
                second(s) for the arp cache of DUT to get refreshed */
                sleep(ARP_config.ARP_TOLERANCE_TIME+ARP_config.DYNAMIC_ARP_CACHETIMEOUT);
                /* Configure DUT to send a ICMP Request Message to TESTER */
                TP_IcmpEchoRequest(ECHO_REQUEST_Cb_Client,text_to_send,ipv4tester,data_to_send);
                /* TESTER: <HOST-1> Listens (up to <ParamListenTime>) on <DIface-0> */
                ARP_Packet ARP_PR = ListenARP(f,ARP_config.ParamListenTime);
                while(Callback_result == 2){usleep(100);}
                if(Callback_result == 0) /* arp cache cleared */
                {   
                    Callback_result = 2;
                    if(ARP_PR.length != 0)
                    {
                        printf("\n**********test ARP_48 passed*********\n");
                        Arp_48_Test_result = 0;
                    }
                    else
                    {
                        printf("\n*********test ARP_48 failed:no ARP request seen***********\n");
                    }
                    /*  CLEANUP: Configure DUT to clear a timeout of
                    <DYNAMIC-ARP-CACHE-TIMEOUT> seconds
                    for the dynamic entries in the ARP Cache of <DIface-0>*/
                }
                else
                {
                    printf("\n******** test ARP_48 failed : TP_IcmpEchoRequest **********\n");
                }
            }
            else
            {
                printf("\n**********test ARP_48 failed:No ICMP request recieved**********\n");
            }
        }
        else
        {
            printf("\n******** test ARP_48 failed : TP_IcmpEchoRequest **********\n");
        }
    }
    else
    {
        printf("\n*******test ARP_48 failed : TP_ClearArpCache*******\n");
    }
    TP_GenaralEndTest(GenaralEndTest_Cb_Client,0,(text){0});
       
    return 2;//Arp_48_Test_result
}

int ARP_49()
{   
    Callback_result = 2;
    int Arp_49_Test_result1 = 1;
    int Arp_49_Test_result2 = 1;
    int Arp_49_Test_result3 = 1;
      
    TP_GenaralStartTest(GenaralStartTest_Cb_Client);

    /* Configure DUT to clear the dynamic entries in the ARP Cache */
    TP_ClearArpCache(ARP_CLEAR_CACHE_Cb_Client);
    while(Callback_result == 2);
    if(Callback_result == 0) /* arp cache cleared */
    {   
        Callback_result = 2;
        /* DUT CONFIGURE: Configure DUT to set a timeout of
        <DYNAMIC-ARP-CACHE-TIMEOUT> seconds
        for the dynamic entries in the ARP Cache of <DIface-0> */

        /* TESTER:  <HOST-1> Sends ARP Request to DUT through <DIface-0> */
        ARP_Packet ARP_P = CreateARP();
        SendARP(ARP_P);
        /* TESTER: <HOST-1> Waits up to (<ARP-TOLERANCE-TIME>) second(s) for the
        ARP cache of DUT to get refreshed */
        sleep(ARP_config.ARP_TOLERANCE_TIME);
        /* Configure DUT to send a ICMP Request Message to TESTER */
        ip4addr ipv4tester;
        ipv4tester.dataLength = 4;
        stoiIP((char*)ARP_config.HOST_1_IP,ipv4tester.Data);
        char dataicmp[] = "Test_ARP_49";
        vint8 data_to_send;
        data_to_send.Data = (uint8* )dataicmp;
        data_to_send.dataLength = 12;
        char interf[] = "eth0";
        text text_to_send;
        text_to_send.dataLength = 7;
        text_to_send.Data = (uint8 *)interf;
        Packet_filter f;
        strcpy(f.SrcAdd,(char*)ARP_config.DIface_0_IP );
        strcpy(f.dstAdd,(char*)ARP_config.HOST_1_IP);
        TP_IcmpEchoRequest(ECHO_REQUEST_Cb_Client,text_to_send,ipv4tester,data_to_send);
        /* ESTER: <HOST-1> Listens (up to <ParamListenTime>) on <DIface-0> */
        ICMP_Packet ICMP_PR = ListenICMP(f,ARP_config.ParamListenTime);
        while(Callback_result == 2){usleep(100);}
        if(Callback_result == 0) /* arp cache cleared */
        {   
            Callback_result = 2;
            if(ICMP_PR.length !=0)
            {   
                printf("\n***********The first ICMP request received**************\n");
                Arp_49_Test_result1 = 0;
                /* TESTER: <HOST-1> Waits up to (<DYNAMIC-ARP-CACHE-TIMEOUT>/2>)
                second(s) for the arp cache of DUT to get refreshed */
                sleep(ARP_config.ARP_TOLERANCE_TIME/2);
                /* Configure DUT to send a ICMP Request Message to TESTER */
                TP_IcmpEchoRequest(ECHO_REQUEST_Cb_Client,text_to_send,ipv4tester,data_to_send);
                /* TESTER: <HOST-1> Listens (up to <ParamListenTime>) on <DIface-0> */
                ICMP_Packet ICMP_PR1 = ListenICMP(f,ARP_config.ParamListenTime);
                while(Callback_result == 2){usleep(100);}
                if(Callback_result == 0) /* arp cache cleared */
                {   
                    Callback_result = 2;
                    if(ICMP_PR1.length != 0)
                    {
                        printf("\n********The second ICMP request received**************\n");
                        Arp_49_Test_result2 = 0;
                        /* TESTER: <HOST-1> Waits up to ((<DYNAMIC-ARP-CACHE-TIMEOUT>/2)+<ARP-TOLERANCE-TIME>)
                        second(s) for the ARP cache of DUT to get refreshed */
                        sleep(ARP_config.DYNAMIC_ARP_CACHETIMEOUT/2 + ARP_config.ARP_TOLERANCE_TIME);
                        /* Configure DUT to send a ICMP Request Message to TESTER */
                        TP_IcmpEchoRequest(ECHO_REQUEST_Cb_Client,text_to_send,ipv4tester,data_to_send);
                        /* TESTER: <HOST-1> Listens (up to <ParamListenTime>) on <DIface-0> */
                        ARP_Packet ARP_PR = ListenARP(f,ARP_config.ParamListenTime);
                        while(Callback_result == 2){usleep(100);}
                        if(Callback_result == 0)
                        {   
                            Callback_result = 2;
                            if(ARP_PR.length != 0)
                            {
                                printf("\n**********test ARP_49 passed*********\n");
                                Arp_49_Test_result3 = 0;
                            }
                            else
                            {
                                printf("\n*********test ARP_49 failed:no ARP request seen***********\n");
                            }
                            /* CLEANUP: Configure DUT to clear a timeout of
                            <DYNAMIC-ARP-CACHE-TIMEOUT> seconds for the dynamic 
                            entries in the ARP Cache of <DIface-0> */
                        }
                        else
                        {
                            printf("\n******** test ARP_49 failed : TP_IcmpEchoRequest3 **********\n");
                        }
                    }
                    else
                    {
                        printf("\n*********test ARP_49 failed:no ICMP2 request seen***********\n");
                    }
                    /*  CLEANUP: Configure DUT to clear a timeout of
                    <DYNAMIC-ARP-CACHE-TIMEOUT> seconds
                    for the dynamic entries in the ARP Cache of <DIface-0>*/
                }
                else
                {
                    printf("\n******** test ARP_49 failed : TP_IcmpEchoRequest2 **********\n");
                }
            }
            else
            {
                printf("\n**********test ARP_49 failed:No ICMP1 request recieved**********\n");
            }
        }
        else
        {
            printf("\n******** test ARP_49 failed : TP_IcmpEchoRequest1 **********\n");
        }
    }
    else
    {
        printf("\n*******test ARP_49 failed : TP_ClearArpCache*******\n");
    }
    TP_GenaralEndTest(GenaralEndTest_Cb_Client,0,(text){0});
       
    return 2;//Arp_49_Test_result1 Arp_49_Test_result2 Arp_49_Test_result3;
}