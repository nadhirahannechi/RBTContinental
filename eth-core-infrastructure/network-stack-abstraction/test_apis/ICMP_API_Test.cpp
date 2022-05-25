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
    strcpy(configuration.S_DUT_HW_Address, "08:00:27:68:41:d5");
    strcpy(configuration.S_DUT_IP_Address, "192.168.20.248");
    strcpy(configuration.S_Tester_HW_Address, "08:00:27:cd:64:f1" );
    strcpy(configuration.S_Tester_IP_Address, "192.168.20.83");
    Set_Network_Abstration_API_Config(configuration);
    Net_API_config_t currentconfig=Get_Network_Abstration_API_Config();;
    cout<<"interface "<<currentconfig.S_interface<<endl;
    cout<<"dut mac "<<currentconfig.S_DUT_HW_Address<<endl;
    cout<<"dut ip "<<currentconfig.S_DUT_IP_Address<<endl;
    cout<<"tester mac "<<currentconfig.S_Tester_HW_Address<<endl;
    cout<<"tester ip "<<currentconfig.S_Tester_IP_Address<<endl;
    //EditICMPField(&ICMP_P,ICMP,ICMP_identifier,(void *)0xffee);




   /* uint16_t a= 0b1111111111111111;

    EditICMPField(&ICMP_P,IP,IP_Offset,(void*)a);
    cout<<"ip offset = "<<a <<endl;*/


    //EditICMPField(&ICMP_P, PAYLAOD, PAYLOAD_data, (void*)"mlzelmzlmflmezlmzelmzelmezlmezflmlemzflmezlmezflmezlmzeflmlmelmzelmzelmzelmezzelmflmzezem");

    //ICMP_Compute_checksum(&ICMP_P);


  
    
    
      char payload[] = "ECU NETWORK VALIDATION TEST";
    ICMP_Packet ICMP_P = CreateICMP();
    EditICMPField(&ICMP_P, IP, IP_TotalLength, (void *)55);
    EditICMPField(&ICMP_P, PAYLAOD, PAYLOAD_data, (void *)payload);
    ICMP_Compute_checksum(&ICMP_P);
    SendICMP(ICMP_P);
    //packet presc((char*)ICMP_PR.data);
    //presc.printPacket();
}
