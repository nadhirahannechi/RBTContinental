#include <AbstractionAPI.h>
#include <stdlib.h>
#include <cstring>
#include <cstdio>
#include <iostream>
#include "../inc/NetworkLib.h"
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
    /*ICMP_Packet ICMP_P = CreateICMP();
    EditICMPField(&ICMP_P, IP, IP_TotalLength, (void *)52);
    EditICMPField(&ICMP_P, IP, IP_Offset, (void *)0b0010000000000000);
        EditICMPField(&ICMP_P, PAYLAOD, PAYLOAD_data, (void *)"THEFIRST" );

    ICMP_Compute_checksum(&ICMP_P);
    SendICMP(ICMP_P);*/
    char secondHalfPayload[] = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";

   IP_Packet IP_P1 = CreateIP();
    EditIPField(&IP_P1, PAYLAOD, PAYLOAD_data, (void *)secondHalfPayload);
    EditIPField(&IP_P1, IP, IP_Identification, (void *)0x0024);

    EditIPField(&IP_P1, IP, IP_Offset, (void *)0b0000000000000101);

    EditIPField(&IP_P1, IP, IP_Protocol, (void *)(intptr_t)0x06);


    IP_Compute_checksum(&IP_P1);

    SendIP(IP_P1);

/*
    IP_Packet IP_P3 = CreateIP();
    EditIPField(&IP_P3, PAYLAOD, PAYLOAD_data, (void *) "TheSecondPacket..TheSecondPacket");
    EditIPField(&IP_P3, IP, IP_TotalLength, (void *)52);
    EditIPField(&IP_P3, IP, IP_Offset, (void *)0b0010000000000100);
    EditIPField(&IP_P3, IP, IP_Protocol, (void *)0x01);
    IP_Compute_checksum(&IP_P3);
    SendIP(IP_P3);

    IP_Packet IP_P4 = CreateIP();
    EditIPField(&IP_P4, PAYLAOD, PAYLOAD_data, (void *) "TheFinalPaktet....TheFinalPaktet");
    EditIPField(&IP_P4, IP, IP_TotalLength, (void *)52);
    EditIPField(&IP_P4, IP, IP_Offset, (void *)0b0000000000001100);
    EditIPField(&IP_P4, IP, IP_Protocol, (void *)0x01);
    IP_Compute_checksum(&IP_P4);
    SendIP(IP_P4);

    Packet_filter f;
    f.dstAdd=(char*)GetICMPField(&ICMP_P, IP, IP_SrcAddress);
    f.SrcAdd=(char*)GetICMPField(&ICMP_P, IP, IP_DstAddress);
    ICMP_Packet ICMP_PR = ListenICMP(f,3);

    if(ICMP_PR.length == 0)
    {
        printf("IPv4_REASSEMBLY_04: No Packets recieved\n");
    }
    else if((int)GetICMPField(&ICMP_PR ,ICMP, ICMP_type) == 0 && (int)GetICMPField(&ICMP_PR ,ICMP, ICMP_code) == 0)
    {
        printf("IPv4_REASSEMBLY_04: Test passed\n");
    }
    else
    {
        printf("IPv4_REASSEMBLY_04: Test failed\n");
    }*/
}