#include "AbstractionAPI.h"
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
    strcpy(configuration.S_DUT_HW_Address, "08:00:27:05:11:fe");
    strcpy(configuration.S_DUT_IP_Address, "192.168.20.248");
    strcpy(configuration.S_Tester_HW_Address, "08:00:27:cd:64:f1");
    strcpy(configuration.S_Tester_IP_Address, "192.168.20.83");
    Set_Network_Abstration_API_Config(configuration);
    Net_API_config_t currentconfig=Get_Network_Abstration_API_Config();;
    cout<<"interface "<<currentconfig.S_interface<<endl;
    cout<<"dut mac "<<currentconfig.S_DUT_HW_Address<<endl;
    cout<<"dut ip "<<currentconfig.S_DUT_IP_Address<<endl;
    cout<<"tester mac "<<currentconfig.S_Tester_HW_Address<<endl;
    cout<<"tester ip "<<currentconfig.S_Tester_IP_Address<<endl;
    

    // 1. TESTER: Construct an ICMP Timestamp Echo Request
    ICMP_Timestamp_Packet ICMP_Timestamp_P = CreateICMPTimestamp();
    EditICMPTimestampField(&ICMP_Timestamp_P, ICMP_Timestamp, ICMP_Timestamp_originate_timestamp, (void *)5500000);
    ICMP_Timestamp_Compute_checksum(&ICMP_Timestamp_P);
    SendICMPTimestamp(ICMP_Timestamp_P);

    // 2. TESTER: Listen for a response
    Packet_filter filter;
    strcpy(filter.dstAdd, "192.168.20.83");
    strcpy(filter.SrcAdd, "192.168.20.248");
    ICMP_Timestamp_Packet ICMP_PR = ListenICMPTimestamp(filter, 2);

    // 3. DUT: Send ICMP Timestamp Reply containing the same id and sequence as the ICMP request
    if (ICMP_PR.length == 0)
    {
        printf("------------- ICMPv4_TYPE_11: Test failed -> DUT not responding -------------\n");
        return 1;
    }
    else if ((uint32_t)GetICMPTimestampField(&ICMP_PR, ICMP_Timestamp, ICMP_Timestamp_originate_timestamp) == 5500000 && (uint32_t)GetICMPTimestampField(&ICMP_PR, ICMP_Timestamp, ICMP_Timestamp_receive_timestamp) != 0 && (uint32_t)GetICMPTimestampField(&ICMP_PR, ICMP_Timestamp, ICMP_Timestamp_transmit_timestamp) != 0)
    {
        printf("------------- ICMPv4_TYPE_11: Test passed -------------\n");
        return 0;
    }
    else
    {
        printf("------------- ICMPv4_TYPE_12: Test failed -------------\n");
        return 1;
    }
}