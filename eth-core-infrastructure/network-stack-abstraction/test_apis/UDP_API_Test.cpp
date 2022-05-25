#include <AbstractionAPI.h>
#include "../inc/NetworkLib.h"
#include<cstdio>
#include<string>
#include<cstring>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;



int main()
{   
    Net_API_config_t configuration; 
    strcpy(configuration.S_interface, "enp0s3");
    strcpy(configuration.S_DUT_HW_Address, "50:9a:4c:34:2e:a6" );
    strcpy(configuration.S_DUT_IP_Address, "192.168.20.178");
    strcpy(configuration.S_Tester_HW_Address, "08:00:27:bc:4c:d5" );
    strcpy(configuration.S_Tester_IP_Address, "192.168.20.155");
    Set_Network_Abstration_API_Config(configuration);
    UDP_Packet UDP_P=CreateUDP();
    EditUDPField(&UDP_P,UDP,UDP_SrcPort,(void*)20000);
    EditUDPField(&UDP_P,UDP,UDP_DstPort,(void*)htons(20001));
    EditUDPField(&UDP_P,UDP,UDP_Length,(void*)16);
    //EditUDPField(&UDP_P,ETHERNET,ETH_sourceMac,(void*)"54:e1:ad:39:8b:16" );
    //EditUDPField(&UDP_P,ETHERNET,ETH_destinationMAC,(void*)"08:00:27:e7:2d:e8");

    //EditUDPField(&UDP_P,IP,IP_SrcAddress,(void*)"127.0.0.2");
    //EditUDPField(&UDP_P,IP,IP_DstAddress,(void*)"192.1.2.1");
    cout<<" src port "<< (uint16_t) htons(GetUDPField(&UDP_P,UDP,UDP_SrcPort))<<endl;
    cout<<" dst port "<< (uint16_t) htons(GetUDPField(&UDP_P,UDP,UDP_DstPort))<<endl;
    cout<<" length "<< (uint16_t) htons(GetUDPField(&UDP_P,UDP,UDP_Length))<<endl;
    cout<<" checksum "<< (uint16_t) htons(GetUDPField(&UDP_P,UDP,UDP_Checksum))<<endl;
    cout<< "verify checksum "<<UDP_Verify_Correct_checksum(UDP_P)<<endl;
    UDP_Compute_checksum(&UDP_P);
    cout<< "verify checksum "<<UDP_Verify_Correct_checksum(UDP_P)<<endl;
    //packet pp((char*)UDP_P.data);
    //pp.printPacket();
    SendUDP(UDP_P);
    //packet p1((char*)UDP_P.data);
    //p1.printPacket();
    UDP_Compute_checksum(&UDP_P);
    //packet p2((char*)UDP_P.data);
    //p2.printPacket();
/*
    Packet_filter f;
 
    f.Srcport=htons(30490);
    f.Dstport=htons(30490);
    UDP_Packet UDP_PR= ListenUDP(f,1);
    packet p((char*)UDP_PR.data);*/
    //p.printPacket();
    
}
