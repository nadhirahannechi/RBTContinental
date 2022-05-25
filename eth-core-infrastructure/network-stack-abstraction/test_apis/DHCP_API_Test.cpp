#include <AbstractionAPI.h>
#include <stdlib.h>
#include <cstring>
#include <cstdio>
#include <iostream>

using namespace std;

int main()
{   
    Net_API_config_t configuration; 
    strcpy(configuration.S_interface, "enp0s3");
    strcpy(configuration.S_DUT_HW_Address, "00:24:9b:6a:4b:1f" );
    strcpy(configuration.S_DUT_IP_Address, "169.254.56.44");
    strcpy(configuration.S_Tester_HW_Address, "08:00:27:bc:4c:d5" );
    strcpy(configuration.S_Tester_IP_Address, "169.254.56.45");
    Set_Network_Abstration_API_Config(configuration);
    Packet_filter f;
    strcpy(f.SrcAdd, "0.0.0.0");
    strcpy(f.dstAdd, "255.255.255.255");
    DHCP_Packet DHCP_RP = ListenDHCP(f,20);
    uint32_t xid = 0;
    if(DHCP_RP.length > 0)
    {   
        xid = (uint32_t)GetDHCPField(&DHCP_RP,DHCP,random_transaction_id_number);
        cout << xid << endl;
        /* create dhcp ofer */
        DHCP_Packet DHCP_P =  CreateDHCP();
        EditDHCPField(&DHCP_P,DHCP,random_transaction_id_number,(void*)xid);
        EditDHCPField(&DHCP_P, DHCP, yiaddr, (void*)"192.168.1.10");
        dhcp_value *val;
        val = (dhcp_value*)malloc(sizeof(dhcp_value));
        val->data[0] = '\x63';
	    val->data[1] = '\x82';
	    val->data[2] = '\x53';
	    val->data[3] = '\x63';

	    val->data[4] = '\x35';
	    val->data[5] = '\x01';
        val->data[6] = '\x02';
        val->data[7] = '\xff';
        val->len = 8;
        EditDHCPField(&DHCP_P,DHCP,options,(void *)val);
        SendDHCP(DHCP_P);
        free(val);
    }
    else
    {
        printf("nooooooooooooooooo dhcp received\n");
    }
    
    /*char* Option = (char*)GetDHCPField(&DHCP_P,DHCP,options);
    
    for(int i=0; i<8; ++i)
    {
        printf("%.2X ",Option[i]);
    }*/
}