#include "AbstractionAPI.h"
#include <stdlib.h>
#include <cstring>
#include <cstdio>
#include <string>
//#include "NetworkLib.h"
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
    Packet_filter f;
    /*f.arp_operation = 0x0002;
    printf("hello");
    memcpy(f.spa,"192.168.20.117",16);
    memcpy(f.tpa,"192.168.20.83",16);*/

    printf("hello22");
    ARP_Packet ARP_P = CreateARP();
    SendARP(ARP_P);  
    /*string *s = new string("00:00:00:00:00:00");
    EditARPField(&ARP_P,ARP,target_hardware_address,(void*)s);*/
    /*SendARP(ARP_P);
    ARP_Packet ARP_PR = ListenARP(f,10);*/
    //packet p((char*)ARP_P.data);
    //p.printPacket();
   
    
    /*if(ARP_PR.length != 0)
    {
        printf("packet received %d",ARP_PR.length);
    }
    else
    {
        printf("no packet detected");
    }*/
    //p.printPacket();

}