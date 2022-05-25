#include "packet_generation.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "AbstractionAPI.h"
#include "TestabilityProtocol_Intern.h"
#include "TestabilityProtocol_api.h"
#include "config.h"
#include"common.h"
#include <unistd.h>
#include <netinet/in.h>
#include<thread>
#include <future>

extern Arp_config_t ARP_config;
uint8_t callback_result  = 2;

void  ARP_CLEAR_CACHE_cb_client(TP_ResultID_t b)
{   
    callback_result = b;
    TP_TRACE_DEBUG("cb TP_ResultID_t : %d  ",b);
}

void ARP_ADD_STATIC_ENTRY_cb_client(TP_ResultID_t b)
{   
    callback_result = b;
    TP_TRACE_DEBUG("cb TP_ResultID_t : %d  ",b);
}

void ARP_DELETE_STATIC_ENTRY_cb_client(TP_ResultID_t b)
{   
    callback_result = b;
    TP_TRACE_DEBUG("cb TP_ResultID_t : %d  ",b);
}

void ECHO_REQUEST_cb_client(TP_ResultID_t b)
{   
    callback_result = b;
    TP_TRACE_DEBUG("cb TP_ECHO_REQUEST_t : %d  ",b);
}

void GenaralStartTest_cb_client(TP_ResultID_t b)
{   
    TP_TRACE_DEBUG("cb TP_ResultID_t %d  ",b);
}

void GenaralEndTest_cb_client(TP_ResultID_t b)
{
    TP_TRACE_DEBUG("cb TP_ResultID_t %d socketID ",b);
}



int ARP_01()
{   
    callback_result = 2;
    int Arp_01_Test_result = 1;
    TP_GenaralStartTest(GenaralStartTest_cb_client);

    /* Configure DUT to clear the dynamic entries in its ARP Cache */
    TP_ClearArpCache(ARP_CLEAR_CACHE_cb_client);
    while(callback_result == 2){usleep(100);}
    if(callback_result == 0) /* arp cache cleared */
    {   
        callback_result = 2;
        /*  Configure DUT to add a static entry in its ARP Cache */
        vint8 addip;
        addip.dataLength = 15;
        char IP[16];
        strcpy(IP,(char*)ARP_config.HOST_1_IP); //Tester IP
        addip.Data = (uint8 *) IP;
        vint8 addmac;
        addmac.dataLength = 17;
        char MAC[18] ; // Tester MAC
        strcpy(MAC,(char *)ARP_config.MAC_ADDR1);
        addmac.Data = (uint8 *) MAC;
        TP_AddStaticEntry(ARP_ADD_STATIC_ENTRY_cb_client,addip,addmac);
        while (callback_result == 2){usleep(100);}
        if(callback_result == 0)
        {
            callback_result = 2;
            /* Configure DUT to send a ICMP Request Message to TESTER */
            ip4addr ipv4tester;
            ipv4tester.dataLength = 4;
	        stoiIP((char*)ARP_config.HOST_1_IP,ipv4tester.Data);
            char dataicmp[] = "Test_ARP_01";
            vint8 data_to_send;
            data_to_send.Data = (uint8* )dataicmp;
            data_to_send.dataLength = 12;

            char interf[] = "eth0";
            text text_to_send;
            text_to_send.dataLength = 7;
            text_to_send.Data = (uint8 *)interf;

            TP_IcmpEchoRequest(ECHO_REQUEST_cb_client,text_to_send,ipv4tester,data_to_send);
            while(callback_result == 2) {usleep(100);}
            if(callback_result == 0)
            {
                callback_result = 2;
                /* TESTER: <HOST-1> Listens (up to <ParamListenTime>) on <DIface-0> */
                Packet_filter f;
                f.arp_operation = 0x0001;       /* arp request */
                strcpy(f.spa,(char*)ARP_config.DIface_0_IP );   /* DUT ip address */
                strcpy(f.tpa,(char*)ARP_config.HOST_1_IP);    /* TESTER ip address */
                ARP_Packet ARP_PR = ListenARP(f,ARP_config.ParamListenTime); /* ParamListenTime = 3s */
                if(ARP_PR.length == 0 )
                {
                    printf("\n ********** test ARP_01 passed *********\n");
                    Arp_01_Test_result = 0;
                }
                else
                {
                    printf("\n********* test ARP_01 failed : DUT send ARP request **********\n");
                }
                /* Configure DUT to delete static entry in its ARP Cache */
                TP_DeleteStaticEntry(ARP_DELETE_STATIC_ENTRY_cb_client,addip);
                while(callback_result == 2 ) {usleep(100);}
                if(callback_result == 0)
                {   
                    callback_result = 2;
                }
                else
                {
                    printf("\n ******** Error in test ARP_01 : TP_DeleteStaticEntry ********** \n");
                } 
            }
            else
            {
                printf("\n ******** test ARP_01 failed : TP_IcmpEchoRequest **********\n");
            }
        }
        else
        {
            printf("\n******** test ARP_01 failed : TP_AddStaticEntry **********\n");
        }
    }
    else
    {
        printf("\n********test ARP_01 failed : TP_ClearArpCache********\n");
    }
    TP_GenaralEndTest(GenaralEndTest_cb_client,0,(text){0});
 
    return Arp_01_Test_result;
}

int ARP_02()
{   
    callback_result = 2;
    int Arp_02_Test_result = 1;
    TP_GenaralStartTest(GenaralStartTest_cb_client);
    /* Configure DUT to clear the dynamic entries in its ARP Cache */
    TP_ClearArpCache(ARP_CLEAR_CACHE_cb_client);
    while(callback_result == 2){usleep(100);}
    if(callback_result == 0) /* arp cache cleared */
    {   
        callback_result = 2;
        /* Configure DUT to add a static entry in its ARP Cache */
        vint8 addip;
        addip.dataLength = 15;
        char IP[16];
        strcpy(IP,(char*)ARP_config.HOST_1_IP); //Tester IP
        addip.Data = (uint8 *) IP;
        vint8 addmac;
        addmac.dataLength = 17;
        char MAC[18] ; // Tester MAC
        strcpy(MAC,(char *)ARP_config.MAC_ADDR1);
        addmac.Data = (uint8 *) MAC;
        TP_AddStaticEntry(ARP_ADD_STATIC_ENTRY_cb_client,addip,addmac);
        while (callback_result == 2){usleep(100);}
        if(callback_result == 0)
        {
            callback_result = 2;
            /* Configure DUT to send a ICMP Request Message to TESTER */
            ip4addr ipv4tester;
            ipv4tester.dataLength = 4;
            stoiIP((char*)ARP_config.HOST_1_IP,ipv4tester.Data);
            char dataicmp[] = "Test_ARP_02";
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
            TP_IcmpEchoRequest(ECHO_REQUEST_cb_client,text_to_send,ipv4tester,data_to_send);
            ICMP_Packet ICMP_PR = ListenICMP(f,ARP_config.ParamListenTime);
            while(callback_result == 2) {usleep(100);}
            if(callback_result == 0)
            {
                callback_result = 2;
                if(ICMP_PR.length != 0 )
                {   
                    char MAC1[18];
                    lower_to_upper_mac(ARP_config.MAC_ADDR1,MAC1);
                    if(strcmp((char *)GetICMPField(&ICMP_PR,ETHERNET,ETH_destinationMAC),(char*)MAC1)==0)
                    {       
                        //strcmp((char *)GetICMPField(&ICMP_PR,IP,IP_DstAddress),(char*)ARP_config.HOST_1_IP)
                        printf("\n ********** test ARP_02 passed *********\n");
                        Arp_02_Test_result = 0;
                    }
                    else
                    {
                        printf("\n********** test ARP_02 failed : recieved mac != MAC_ADDR1 *********\n");
                    }
                }
                else
                {
                    printf("\n********* test ARP_02 failed : DUT does not send icmp request ********\n");
                }
                /* Configure DUT to delete static entry in its ARP Cache */
                TP_DeleteStaticEntry(ARP_DELETE_STATIC_ENTRY_cb_client,addip);
                while(callback_result == 2 ) {usleep(100);}
                if(callback_result == 0)
                {   
                    callback_result = 2;
                }
                else
                {
                    printf("\n******** Error in test ARP_02 : TP_DeleteStaticEntry **********\n");
                } 
            }
            else
            {
                printf("\n******** test ARP_02 failed : TP_IcmpEchoRequest **********\n");
            }
        }
        else
        {
            printf("\n******** test ARP_02 failed : TP_AddStaticEntry **********\n");
        }
    }
    else
    {
        printf("\n*******test ARP_02 failed : TP_ClearArpCache*******\n");
    }
    TP_GenaralEndTest(GenaralEndTest_cb_client,0,(text){0});
 
    return Arp_02_Test_result; 
}

int ARP_03()
{   
    callback_result = 2;
    int Arp_03_Test_result = 1;
    TP_GenaralStartTest(GenaralStartTest_cb_client);

    /* Configure DUT to clear the dynamic entries in the ARP Cache */
    TP_ClearArpCache(ARP_CLEAR_CACHE_cb_client);
    while(callback_result == 2){usleep(100);}
    if(callback_result == 0)
    {   
        callback_result = 2;
        /*TESTER:  <HOST-1> Sends ARP Request to DUT through <DIface-0>*/
        ARP_Packet ARP_P = CreateARP();
        SendARP(ARP_P); 
        /* TESTER: <HOST-1> Waits up to (<ARP-TOLERANCE-TIME>) second(s) for the arp cache of DUT to get refreshed */
        sleep(ARP_config.ARP_TOLERANCE_TIME);
        /* Configure DUT to send a ICMP Request Message to TESTER */
        ip4addr ipv4tester;
        ipv4tester.dataLength = 4;
        stoiIP((char*)ARP_config.HOST_1_IP,ipv4tester.Data);
        char dataicmp[] = "Test_ARP_03";
        vint8 data_to_send;
        data_to_send.Data = (uint8* )dataicmp;
        data_to_send.dataLength = 12;
        char interf[] = "eth0";
        text text_to_send;
        text_to_send.dataLength = 7;
        text_to_send.Data = (uint8 *)interf;
        Packet_filter f;
        f.arp_operation = 0x0001;       /* arp request */
        strcpy(f.spa,(char*)ARP_config.DIface_0_IP );   /* DUT ip address */
        strcpy(f.tpa,(char*)ARP_config.HOST_1_IP);    /* TESTER ip address */
        TP_IcmpEchoRequest(ECHO_REQUEST_cb_client,text_to_send,ipv4tester,data_to_send);
        ARP_Packet ARP_PR = ListenARP(f,ARP_config.ParamListenTime);
        while(callback_result == 2){usleep(100);}
        if(callback_result == 0) /* arp cache cleared */
        {   
            callback_result = 2;
            /* TESTER: <HOST-1> Listens (up to <ParamListenTime>) on <DIface-0> */
            if(ARP_PR.length == 0 )
            {
                printf("\n*******Test ARP_03 passed*******\n");
                Arp_03_Test_result = 0;
            }
            else
            {
                printf("\n*******Test ARP_03 failed : ARP request recieved\n");
            }
        }
        else
        {
            printf("\n******** test ARP_03 failed : TP_IcmpEchoRequest **********\n");
        }
    }
    else
    {
        printf("\n*******test ARP_03 failed : TP_ClearArpCache*******\n");
    }
    TP_GenaralEndTest(GenaralEndTest_cb_client,0,(text){0});
 
    return Arp_03_Test_result;
}

int ARP_04()
{   
    callback_result = 2;  
    int Arp_04_Test_result = 1;
    TP_GenaralStartTest(GenaralStartTest_cb_client);
    /* Configure DUT to clear the dynamic entries in the ARP Cache */
    TP_ClearArpCache(ARP_CLEAR_CACHE_cb_client);
    while(callback_result == 2){usleep(100);}
    if(callback_result == 0) /* arp cache cleared */
    {   
        callback_result = 2;

        /* TESTER: <HOST-1> Sends ARP Response(gratuitous) to DUT through <DIface-0> */
        ARP_Packet ARP_P = CreateARP();
        EditARPField(&ARP_P,ARP,opcode,(void*)2);
        SendARP(ARP_P);
        /* TESTER: <HOST-1> Waits up to (<ARP-TOLERANCE-TIME>) second(s) for the
        arp cache of DUT to get refreshed */
        sleep(ARP_config.ARP_TOLERANCE_TIME); /* ARP-TOLERANCE-TIME */
        /* Configure DUT to send a ICMP Request Message to TESTER */
        ip4addr ipv4tester;
        ipv4tester.dataLength = 4;
        stoiIP((char*)ARP_config.HOST_1_IP,ipv4tester.Data);
        char dataicmp[] = "Test_ARP_04";
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
        TP_IcmpEchoRequest(ECHO_REQUEST_cb_client,text_to_send,ipv4tester,data_to_send);
        /* TESTER: <HOST-1> Listens (up to <ParamListenTime>) on <DIface-0> */
        ICMP_Packet ICMP_PR = ListenICMP(f,ARP_config.ParamListenTime); 
        while(callback_result == 2){usleep(100);}
        if(callback_result == 0) /* arp cache cleared */
        {   
            callback_result = 2;
            if(ICMP_PR.length !=0 )
            {       
                /* transfer the mac addr from lower to upper case*/    
                char MAC1[18];
                lower_to_upper_mac(ARP_config.MAC_ADDR1,MAC1);
                if(strcmp((char *)GetICMPField(&ICMP_PR,ETHERNET,ETH_destinationMAC),(char*)MAC1)==0)
                {
                    printf("\n*********test ARP_04 passed**********\n");
                    Arp_04_Test_result = 0;
                }
                else
                {
                    printf("\n*********test ARP_04 failed: eth_dest_mac != MAC_ARRD1*********\n");
                }
                
            }
            else
            {
                printf("\n**********test ARP_04 failed:No ICMP request recieved**********\n");
            }
        }
        else
        {
            printf("\n******** test ARP_04 failed : TP_IcmpEchoRequest **********\n");
        }
    }
    else
    {
        printf("\n*******test ARP_04 failed : TP_ClearArpCache*******\n");
    }
    TP_GenaralEndTest(GenaralEndTest_cb_client,0,(text){0});
 
    return Arp_04_Test_result;
}

int ARP_05()
{   
    callback_result = 2;
    int Arp_05_Test_result = 1;
    TP_GenaralStartTest(GenaralStartTest_cb_client);
    /* Configure DUT to clear the dynamic entries in the ARP Cache */
    TP_ClearArpCache(ARP_CLEAR_CACHE_cb_client);
    while(callback_result == 2){usleep(100);}
    if(callback_result == 0) /* arp cache cleared */
    {   
        callback_result = 2;
        /* TER:  <HOST-1> Sends ARP Response to DUT through <DIface-0> */
        ARP_Packet ARP_P = CreateARP();
        EditARPField(&ARP_P,ARP,opcode,(void*)2);   /* ARP Response */
        SendARP(ARP_P);
        /* TESTER: <HOST-1> Waits up to (<ARP-TOLERANCE-TIME>) second(s) for the
        ARP cache of DUT to get refreshed */
        sleep(ARP_config.ARP_TOLERANCE_TIME); /* ARP-TOLERANCE-TIME */
        /* Configure DUT to send a ICMP Request Message to TESTER */
        ip4addr ipv4tester;
        ipv4tester.dataLength = 4;
        stoiIP((char*)ARP_config.HOST_1_IP,ipv4tester.Data);
        char dataicmp[] = "Test_ARP_05";
        vint8 data_to_send;
        data_to_send.Data = (uint8* )dataicmp;
        data_to_send.dataLength = 12;
        char interf[] = "eth0";
        text text_to_send;
        text_to_send.dataLength = 7;
        text_to_send.Data = (uint8 *)interf;
        Packet_filter f;
        f.arp_operation = 0x0001;       /* arp request */
        strcpy(f.spa,(char*)ARP_config.DIface_0_IP );   /* DUT ip address */
        strcpy(f.tpa,(char*)ARP_config.HOST_1_IP);    /* TESTER ip address */
        TP_IcmpEchoRequest(ECHO_REQUEST_cb_client,text_to_send,ipv4tester,data_to_send);
        /* TESTER: <HOST-1> Listens (up to <ParamListenTime>) on <DIface-0> */
        ARP_Packet ARP_PR = ListenARP(f,ARP_config.ParamListenTime);
        while(callback_result == 2){usleep(100);}
        if(callback_result == 0) /* arp cache cleared */
        {   
            callback_result = 2;
            if(ARP_PR.length == 0)
            {
                printf("\n**********test ARP_05 passed*********\n");
                Arp_05_Test_result = 0;
            }
            else
            {
                printf("\n*********test ARP_05 failed***********\n");
            }
        }
        else
        {
            printf("\n******** test ARP_05 failed : TP_IcmpEchoRequest **********\n");
        }
    }
    else
    {
        printf("\n*******test ARP_05 failed : TP_ClearArpCache*******\n");
    }
    TP_GenaralEndTest(GenaralEndTest_cb_client,0,(text){0});
 
    return Arp_05_Test_result;
}

int ARP_06()
{   
    callback_result = 2;
    int Arp_06_Test_result = 1;
    TP_GenaralStartTest(GenaralStartTest_cb_client);
    /* Configure DUT to clear the dynamic entries in the ARP Cache */
    TP_ClearArpCache(ARP_CLEAR_CACHE_cb_client);
    while(callback_result == 2);
    if(callback_result == 0) /* arp cache cleared */
    {   
        callback_result = 2;
        /* TESTER:  <HOST-1> Sends ARP Response to DUT through <DIface-0> */
        ARP_Packet ARP_P = CreateARP();
        EditARPField(&ARP_P,ARP,opcode,(void*)2);   /* ARP Response */
        SendARP(ARP_P); /* send AARP request */
        /* TESTER: <HOST-1> Waits up to (<ARP-TOLERANCE-TIME>) second(s) for the
        arp cache of DUT to get refreshed */
        sleep(ARP_config.ARP_TOLERANCE_TIME); /* ARP-TOLERANCE-TIME */
        /* Configure DUT to send a ICMP Request Message to TESTER */
        ip4addr ipv4tester;
        ipv4tester.dataLength = 4;
        stoiIP((char*)ARP_config.HOST_1_IP,ipv4tester.Data);
        char dataicmp[] = "Test_ARP_06";
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
        TP_IcmpEchoRequest(ECHO_REQUEST_cb_client,text_to_send,ipv4tester,data_to_send);
        /* TESTER: <HOST-1> Listens (up to <ParamListenTime>) on <DIface-0> */
        ICMP_Packet ICMP_PR = ListenICMP(f,ARP_config.ParamListenTime);
        while(callback_result == 2){usleep(100);}
        if(callback_result == 0) /* arp cache cleared */
        {   
            callback_result = 2;
            if(ICMP_PR.length !=0 )
            {
                printf("\n**********test ARP_06 passed***********\n");
                Arp_06_Test_result = 0;
            }
            else
            {
                printf("\n**********test ARP_06 failed***********\n");
            }
        }
        else
        {
            printf("\n******** test ARP_06 failed : TP_IcmpEchoRequest **********\n");
        }  
    }
    else
    {
        printf("\n*******test ARP_06 failed : TP_ClearArpCache*******\n");
    }
    TP_GenaralEndTest(GenaralEndTest_cb_client,0,(text){0});
 
    return Arp_06_Test_result;
}

int ARP_07()
{   
    callback_result = 2;
    int Arp_07_Test_result = 1;
    TP_GenaralStartTest(GenaralStartTest_cb_client);
    /* Configure DUT to clear the dynamic entries in the ARP Cache */
    TP_ClearArpCache(ARP_CLEAR_CACHE_cb_client);
    while(callback_result == 2){usleep(100);}
    if(callback_result == 0) /* arp cache cleared */
    {   
        callback_result = 2;
        /* Configure DUT to send a ICMP Request Message to TESTER */
        ip4addr ipv4tester;
        ipv4tester.dataLength = 4;
        stoiIP((char*)ARP_config.HOST_1_IP,ipv4tester.Data);
        char dataicmp[] = "Test_ARP_07";
        vint8 data_to_send;
        data_to_send.Data = (uint8* )dataicmp;
        data_to_send.dataLength = 12;
        char interf[] = "eth0";
        text text_to_send;
        text_to_send.dataLength = 7;
        text_to_send.Data = (uint8 *)interf;
        Packet_filter f;
        f.arp_operation = 0x0001;       /* arp request */
        strcpy(f.spa,(char*)ARP_config.DIface_0_IP );   /* DUT ip address */
        strcpy(f.tpa,(char*)ARP_config.HOST_1_IP);    /* TESTER ip address */
        auto future = std::async(ListenARP,f,ARP_config.ParamListenTime);
        TP_IcmpEchoRequest(ECHO_REQUEST_cb_client,text_to_send,ipv4tester,data_to_send);
        ARP_Packet ARP_PR = future.get();
        while(callback_result == 2){usleep(100);}
        if(callback_result == 0)
        {   
            callback_result = 2;
            if(ARP_PR.length != 0 )
            {   
                if((uint16_t)GetARPField(&ARP_PR,ETHERNET,ETH_type) == htons(0x0806))
                {
                    printf("\n*********test ARP_07 passed***********\n");
                    Arp_07_Test_result = 0;
                }
                else
                {
                    printf("\n**********test ARP_07 faield : ethernet tyep != 0x0806*********\n");
                }
            }
            else
            {
                printf("\n*********test ARP_07 failed : no arp request received:%d*********\n",ARP_PR.length);
            }
        }
        else
        {
            printf("\n******** test ARP_07 failed : TP_IcmpEchoRequest **********\n");
        }
    }
    else
    {
        printf("\n*******test ARP_07 failed : TP_ClearArpCache*******\n");
    }
    TP_GenaralEndTest(GenaralEndTest_cb_client,0,(text){0});
 
    return 2;//Arp_07_Test_result;
}

int ARP_08()
{   
    callback_result = 2;
    int Arp_08_Test_result = 1;
    TP_GenaralStartTest(GenaralStartTest_cb_client);
    /* Configure DUT to clear the dynamic entries in the ARP Cache */
    TP_ClearArpCache(ARP_CLEAR_CACHE_cb_client);
    while(callback_result == 2);
    if(callback_result == 0) /* arp cache cleared */
    {   
        callback_result = 2;
        /* Configure DUT to send a ICMP Request Message to TESTER */
        ip4addr ipv4tester;
        ipv4tester.dataLength = 4;
        stoiIP((char*)ARP_config.HOST_1_IP,ipv4tester.Data);
        char dataicmp[] = "Test_ARP_06";
        vint8 data_to_send;
        data_to_send.Data = (uint8* )dataicmp;
        data_to_send.dataLength = 12;
        char interf[] = "eth0";
        text text_to_send;
        text_to_send.dataLength = 7;
        text_to_send.Data = (uint8 *)interf;
        Packet_filter f;
        f.arp_operation = 0x0001;       /* arp request */
        strcpy(f.spa,(char*)ARP_config.DIface_0_IP );   /* DUT ip address */
        strcpy(f.tpa,(char*)ARP_config.HOST_1_IP);    /* TESTER ip address */
        auto future = std::async(ListenARP,f,ARP_config.ParamListenTime);
        TP_IcmpEchoRequest(ECHO_REQUEST_cb_client,text_to_send,ipv4tester,data_to_send);
        ARP_Packet ARP_PR = future.get();
        while(callback_result == 2){usleep(100);}
        if(callback_result == 0)
        {   
            callback_result = 2;
            if(ARP_PR.length != 0)
            {   
                if((uint16_t)GetARPField(&ARP_PR,ARP,hardware_address_format) == 1)
                {
                    printf("\n********test ARP_08 passed*******\n");
                    Arp_08_Test_result = 0;
                }
                else
                {
                    printf("test ARP_08 faield :  Hardware Type != ARP_HARDWARE_ETHERNET\n");
                }
            }
            else
            {
                printf("test ARP_08 failed : no arp request received\n");
            }
        }
        else
        {
            printf("\n******** test ARP_08 failed : TP_IcmpEchoRequest **********\n");
        }
    }
    else
    {
        printf("\n*******test ARP_08 failed : TP_ClearArpCache*******\n");
    }
    TP_GenaralEndTest(GenaralEndTest_cb_client,0,(text){0});
 
    return 2;
}

int ARP_09()
{   
    callback_result = 2;
    int Arp_09_Test_result = 1;
    TP_GenaralStartTest(GenaralStartTest_cb_client);

    /* Configure DUT to clear the dynamic entries in the ARP Cache */
    TP_ClearArpCache(ARP_CLEAR_CACHE_cb_client);
    while(callback_result == 2);
    if(callback_result == 0) /* arp cache cleared */
    {   
        callback_result = 2;
        /* Configure DUT to send a ICMP Request Message to TESTER */
        ip4addr ipv4tester;
        ipv4tester.dataLength = 4;
        stoiIP((char*)ARP_config.HOST_1_IP,ipv4tester.Data);
        char dataicmp[] = "Test_ARP_09";
        vint8 data_to_send;
        data_to_send.Data = (uint8* )dataicmp;
        data_to_send.dataLength = 12;
        char interf[] = "eth0";
        text text_to_send;
        text_to_send.dataLength = 7;
        text_to_send.Data = (uint8 *)interf;
        Packet_filter f;
        f.arp_operation = 0x0001;       /* arp request */
        strcpy(f.spa,(char*)ARP_config.DIface_0_IP );   /* DUT ip address */
        strcpy(f.tpa,(char*)ARP_config.HOST_1_IP);    /* TESTER ip address */
        auto future = std::async(ListenARP,f,ARP_config.ParamListenTime);
        TP_IcmpEchoRequest(ECHO_REQUEST_cb_client,text_to_send,ipv4tester,data_to_send);
        ARP_Packet ARP_PR = future.get();
        while(callback_result == 2){usleep(100);}
        if(callback_result == 0)
        {   
            callback_result = 2;
            if(ARP_PR.length != 0 )
            {   
                if((uint16_t)GetARPField(&ARP_PR,ARP,protocol_address_format) == 0x0800)
                {
                    printf("\n********test ARP_09 passed**********\n");
                    Arp_09_Test_result = 0;
                }
                else
                {
                    printf("\n**********test ARP_09 faield :  Protocol Type != ARP_PROTOCOL_IP*******\n");
                }
            }
            else
            {
                printf("\n********test ARP_09 failed : no arp request received*********\n");
            }
        }
        else
        {
            printf("\n******** test ARP_09 failed : TP_IcmpEchoRequest **********\n");
        }
    }
    else
    {
        printf("\n*******test ARP_09 failed : TP_ClearArpCache*******\n");
    }
    TP_GenaralEndTest(GenaralEndTest_cb_client,0,(text){0});
 
    return 2;
}

int ARP_10()
{   
    callback_result = 2;
    int Arp_10_Test_result = 1;
    TP_GenaralStartTest(GenaralStartTest_cb_client);

    /* Configure DUT to clear the dynamic entries in the ARP Cache */
    TP_ClearArpCache(ARP_CLEAR_CACHE_cb_client);
    while(callback_result == 2);
    if(callback_result == 0) /* arp cache cleared */
    {   
        callback_result = 2;
        /* Configure DUT to send a ICMP Request Message to TESTER */
        ip4addr ipv4tester;
        ipv4tester.dataLength = 4;
        stoiIP((char*)ARP_config.HOST_1_IP,ipv4tester.Data);
        char dataicmp[] = "Test_ARP_10";
        vint8 data_to_send;
        data_to_send.Data = (uint8* )dataicmp;
        data_to_send.dataLength = 12;
        char interf[] = "eth0";
        text text_to_send;
        text_to_send.dataLength = 7;
        text_to_send.Data = (uint8 *)interf;
        Packet_filter f;
        f.arp_operation = 0x0001;       /* arp request */
        strcpy(f.spa,(char*)ARP_config.DIface_0_IP );   /* DUT ip address */
        strcpy(f.tpa,(char*)ARP_config.HOST_1_IP);    /* TESTER ip address */
        auto future = std::async(ListenARP,f,ARP_config.ParamListenTime);
        TP_IcmpEchoRequest(ECHO_REQUEST_cb_client,text_to_send,ipv4tester,data_to_send);
        ARP_Packet ARP_PR = future.get();
        while(callback_result == 2){usleep(100);}
        if(callback_result == 0)
        {   
            callback_result = 2;
            if(ARP_PR.length != 0 )
            {   
                if((uint16_t)GetARPField(&ARP_PR,ARP,Length_hardware_address) == 6)
                {
                    printf("test ARRP_10 passed \n");
                    Arp_10_Test_result = 0;
                }
                else
                {
                    printf("test ARP_10 faield :  Hardware Address Length != ETHERNET_ADDR_LEN \n");
                }
            }
            else
            {
                printf("test ARP_10 failed : no arp request received\n");
            }
        }
        else
        {
            printf("\n******** test ARP_10 failed : TP_IcmpEchoRequest **********\n");
        }
    }
    else
    {
        printf("\n*******test ARP_10 failed : TP_ClearArpCache*******\n");
    }
    TP_GenaralEndTest(GenaralEndTest_cb_client,0,(text){0});
 
    return 2;
}

int ARP_11()
{   
    callback_result = 2;
    int Arp_11_Test_result = 1;
    TP_GenaralStartTest(GenaralStartTest_cb_client);
    /* Configure DUT to clear the dynamic entries in the ARP Cache */
    TP_ClearArpCache(ARP_CLEAR_CACHE_cb_client);
    while(callback_result == 2);
    if(callback_result == 0) /* arp cache cleared */
    {   
        callback_result = 2;
        /* Configure DUT to send a ICMP Request Message to TESTER */
        ip4addr ipv4tester;
        ipv4tester.dataLength = 4;
        stoiIP((char*)ARP_config.HOST_1_IP,ipv4tester.Data);
        char dataicmp[] = "Test_ARP_11";
        vint8 data_to_send;
        data_to_send.Data = (uint8* )dataicmp;
        data_to_send.dataLength = 12;
        char interf[] = "eth0";
        text text_to_send;
        text_to_send.dataLength = 7;
        text_to_send.Data = (uint8 *)interf;
        Packet_filter f;
        f.arp_operation = 0x0001;       /* arp request */
        strcpy(f.spa,(char*)ARP_config.DIface_0_IP );   /* DUT ip address */
        strcpy(f.tpa,(char*)ARP_config.HOST_1_IP);    /* TESTER ip address */
        auto future = std::async(ListenARP,f,ARP_config.ParamListenTime);
        TP_IcmpEchoRequest(ECHO_REQUEST_cb_client,text_to_send,ipv4tester,data_to_send);
        ARP_Packet ARP_PR = future.get();
        while(callback_result == 2){usleep(100);}
        if(callback_result == 0)
        {   
            callback_result = 2;
            if(ARP_PR.length != 0 )
            {   
                if((uint16_t)GetARPField(&ARP_PR,ARP,Length_protocol_address) == 4)
                {
                    printf("\n*******test ARP_11 passed********\n");
                    Arp_11_Test_result = 0;
                }
                else
                {
                    printf("\n*******test ARP_11 faield :  Protocol Address Length != IP_ADDR_LEN\n*******\n");
                }

            }
            else
            {
                printf("\n*********test ARP_11 failed : no arp request received*********\n");
            }
        }
        else
        {
            printf("\n******** test ARP_11 failed : TP_IcmpEchoRequest **********\n");
        }
    }
    else
    {
        printf("\n*******test ARP_11 failed : TP_ClearArpCache*******\n");
    }
    TP_GenaralEndTest(GenaralEndTest_cb_client,0,(text){0});
 
    return 2;
}
int ARP_12()
{   
    callback_result = 2;
    int Arp_12_Test_result = 1;
    TP_GenaralStartTest(GenaralStartTest_cb_client);
    /* Configure DUT to clear the dynamic entries in the ARP Cache */
    TP_ClearArpCache(ARP_CLEAR_CACHE_cb_client);
    while(callback_result == 2);
    if(callback_result == 0) /* arp cache cleared */
    {   
        callback_result = 2;
        /* Configure DUT to send a ICMP Request Message to TESTER */
        ip4addr ipv4tester;
        ipv4tester.dataLength = 4;
        stoiIP((char*)ARP_config.HOST_1_IP,ipv4tester.Data);
        char dataicmp[] = "Test_ARP_12";
        vint8 data_to_send;
        data_to_send.Data = (uint8* )dataicmp;
        data_to_send.dataLength = 12;
        char interf[] = "eth0";
        text text_to_send;
        text_to_send.dataLength = 7;
        text_to_send.Data = (uint8 *)interf;
        Packet_filter f;
        f.arp_operation = 0x0001;       /* arp request */
        strcpy(f.spa,(char*)ARP_config.DIface_0_IP );   /* DUT ip address */
        strcpy(f.tpa,(char*)ARP_config.HOST_1_IP);    /* TESTER ip address */
        auto future = std::async(ListenARP,f,ARP_config.ParamListenTime);
        TP_IcmpEchoRequest(ECHO_REQUEST_cb_client,text_to_send,ipv4tester,data_to_send);
        ARP_Packet ARP_PR = future.get();
        while(callback_result == 2){usleep(100);}
        if(callback_result == 0)
        {   
            callback_result = 2;
            if(ARP_PR.length != 0 )
            {   
                if((uint16_t)GetARPField(&ARP_PR,ARP,opcode) == 1)
                {
                    printf("\n*******test ARP_12 passed*******\n");
                    Arp_12_Test_result = 0;
                }
                else
                {
                    printf("\n*******test ARP_12 faield :  Operation code != OPERATION_REQUEST******\n");
                }
            }
            else
            {
                printf("\n*******test ARP_12 failed : no arp request received*******\n");
            }
        }
        else
        {
            printf("\n******** test ARP_12 failed : TP_IcmpEchoRequest **********\n");
        }
    }
    else
    {
        printf("\n*******test ARP_12 failed : TP_ClearArpCache*******\n");
    }
    TP_GenaralEndTest(GenaralEndTest_cb_client,0,(text){0});
 
    return 2;
}

int ARP_13()
{   
    callback_result = 2;
    int Arp_13_Test_result = 1;
    TP_GenaralStartTest(GenaralStartTest_cb_client);

    /* Configure DUT to clear the dynamic entries in the ARP Cache */
    TP_ClearArpCache(ARP_CLEAR_CACHE_cb_client);
    while(callback_result == 2);
    if(callback_result == 0) /* arp cache cleared */
    {   
        callback_result = 2;
        /* Configure DUT to send a ICMP Request Message to TESTER */
        ip4addr ipv4tester;
        ipv4tester.dataLength = 4;
        stoiIP((char*)ARP_config.HOST_1_IP,ipv4tester.Data);
        char dataicmp[] = "Test_ARP_13";
        vint8 data_to_send;
        data_to_send.Data = (uint8* )dataicmp;
        data_to_send.dataLength = 12;
        char interf[] = "eth0";
        text text_to_send;
        text_to_send.dataLength = 7;
        text_to_send.Data = (uint8 *)interf;
        Packet_filter f;
        f.arp_operation = 0x0001;       /* arp request */
        strcpy(f.spa,(char*)ARP_config.DIface_0_IP );   /* DUT ip address */
        strcpy(f.tpa,(char*)ARP_config.HOST_1_IP);    /* TESTER ip address */
        auto future = std::async(ListenARP,f,ARP_config.ParamListenTime);
        TP_IcmpEchoRequest(ECHO_REQUEST_cb_client,text_to_send,ipv4tester,data_to_send);
        ARP_Packet ARP_PR = future.get();
        while(callback_result == 2){usleep(100);}
        if(callback_result == 0)
        {   
            callback_result = 2;
            if(ARP_PR.length != 0 )
            {   
                if(strcmp((char*)GetARPField(&ARP_PR,ARP,sender_hardware_address),(char*)ARP_config.DIFACE_O_MAC_ADDR) == 0)
                {
                    printf("\n*******test ARP_13 passed*******\n");
                    Arp_13_Test_result = 0;
                }
                else
                {
                    printf("\n********test ARP_13 faield :  ARP Sender Hardware Address!= DIFACE-0-MAC-ADDR*******\n");
                }

            }
            else
            {
                printf("\n*******test ARP_13 failed : no arp request received*******\n");
            }
        }
        else
        {
            printf("\n******** test ARP_13 failed : TP_IcmpEchoRequest **********\n");
        }
    }
    else
    {
        printf("\n*******test ARP_13 failed : TP_ClearArpCache*******\n");
    }
    TP_GenaralEndTest(GenaralEndTest_cb_client,0,(text){0});
 
    return 2;
}

int ARP_14()
{   
    callback_result = 2;
    int Arp_14_Test_result = 1;
    TP_GenaralStartTest(GenaralStartTest_cb_client);

    /* Configure DUT to clear the dynamic entries in the ARP Cache */
    TP_ClearArpCache(ARP_CLEAR_CACHE_cb_client);
    while(callback_result == 2);
    if(callback_result == 0) /* arp cache cleared */
    {   
        callback_result = 2;
        /* Configure DUT to send a ICMP Request Message to TESTER */
        ip4addr ipv4tester;
        ipv4tester.dataLength = 4;
        stoiIP((char*)ARP_config.HOST_1_IP,ipv4tester.Data);
        char dataicmp[] = "Test_ARP_14";
        vint8 data_to_send;
        data_to_send.Data = (uint8* )dataicmp;
        data_to_send.dataLength = 12;
        char interf[] = "eth0";
        text text_to_send;
        text_to_send.dataLength = 7;
        text_to_send.Data = (uint8 *)interf;
        Packet_filter f;
        f.arp_operation = 0x0001;       /* arp request */
        strcpy(f.spa,(char*)ARP_config.DIface_0_IP );   /* DUT ip address */
        strcpy(f.tpa,(char*)ARP_config.HOST_1_IP);    /* TESTER ip address */
        auto future = std::async(ListenARP,f,ARP_config.ParamListenTime);
        TP_IcmpEchoRequest(ECHO_REQUEST_cb_client,text_to_send,ipv4tester,data_to_send);
        ARP_Packet ARP_PR = future.get();
        while(callback_result == 2){usleep(100);}
        if(callback_result == 0)
        {   
            callback_result = 2;
            if(ARP_PR.length != 0 )
            {   
                if(strcmp((char*)GetARPField(&ARP_PR,ARP,sender_protocol_address),(char*)ARP_config.DIface_0_IP) == 0)
                {
                    printf("\n*******test ARP_14 passed*******\n");
                    Arp_14_Test_result = 0;
                }
                else
                {
                    printf("\n********test ARP_14 faield :  ARP Sender Hardware Address!= DIFACE-0-MAC-ADDR*******\n");
                }
            }
            else
            {
                printf("\n*******test ARP_14 failed : no arp request received*******\n");
            }
        }
        else
        {
            printf("\n******** test ARP_14 failed : TP_IcmpEchoRequest **********\n");
        }
    }   
    else
    {
        printf("\n*******test ARP_14 failed : TP_ClearArpCache*******\n");
    }
    TP_GenaralEndTest(GenaralEndTest_cb_client,0,(text){0});
 
    return 2;
}

int ARP_15()
{   
    callback_result = 2;
    int Arp_15_Test_result = 1;
    TP_GenaralStartTest(GenaralStartTest_cb_client);

    /* Configure DUT to clear the dynamic entries in the ARP Cache */
    TP_ClearArpCache(ARP_CLEAR_CACHE_cb_client);
    while(callback_result == 2);
    if(callback_result == 0) /* arp cache cleared */
    {   
        callback_result = 2;
        /* Configure DUT to send a ICMP Request Message to TESTER */
        ip4addr ipv4tester;
        ipv4tester.dataLength = 4;
        stoiIP((char*)ARP_config.HOST_1_IP,ipv4tester.Data);
        char dataicmp[] = "Test_ARP_15";
        vint8 data_to_send;
        data_to_send.Data = (uint8* )dataicmp;
        data_to_send.dataLength = 12;
        char interf[] = "eth0";
        text text_to_send;
        text_to_send.dataLength = 7;
        text_to_send.Data = (uint8 *)interf;
        Packet_filter f;
        f.arp_operation = 0x0001;       /* arp request */
        strcpy(f.spa,(char*)ARP_config.DIface_0_IP );   /* DUT ip address */
        strcpy(f.tpa,(char*)ARP_config.HOST_1_IP);    /* TESTER ip address */
        auto future = std::async(ListenARP,f,ARP_config.ParamListenTime);
        TP_IcmpEchoRequest(ECHO_REQUEST_cb_client,text_to_send,ipv4tester,data_to_send);
        ARP_Packet ARP_PR = future.get();
        while(callback_result == 2){usleep(100);}
        if(callback_result == 0)
        {   
            callback_result = 2;
            if(ARP_PR.length != 0 )
            {   
                if(strcmp((char*)GetARPField(&ARP_PR,ARP,target_protocol_address),(char*)ARP_config.HOST_1_IP) == 0)
                {
                    printf("\n*******test ARP_15 passed*******\n");
                    Arp_15_Test_result = 0;
                }
                else
                {
                    printf("\n********test ARP_15 faield :  ARP Sender Hardware Address!= DIFACE-0-MAC-ADDR*******\n");
                }
            }
            else
            {
                printf("\n*******test ARP_15 failed : no arp request received*******\n");
            }
        }
        else
        {
            printf("\n******** test ARP_15 failed : TP_IcmpEchoRequest **********\n");
        }
    }
    else
    {
        printf("\n*******test ARP_15 failed : TP_ClearArpCache*******\n");
    }
    TP_GenaralEndTest(GenaralEndTest_cb_client,0,(text){0});
 
    return 2;
}
