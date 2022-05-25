#include"config.h"
 
Arp_config_t ARP_config; 

void init_ARP_config()
{   
    ARP_config.DYNAMIC_ARP_CACHETIMEOUT = 1;
    strcpy((char*)ARP_config.HOST_1_IP,"192.168.20.83");
    strcpy((char*)ARP_config.DIface_0_IP,"192.168.20.248");
    ARP_config.ParamListenTime = 10; 
    strcpy((char*)ARP_config.MAC_ADDR1, "08:00:27:cd:64:f1");
    strcpy((char*)ARP_config.MAC_ADDR2, "08:00:27:cd:64:f2");
    strcpy((char*)ARP_config.MAC_ADDR3, "08:00:27:cd:64:f3");
    strcpy((char*)ARP_config.DIFACE_O_MAC_ADDR, "08:00:27:68:41:d5");
    strcpy((char*)ARP_config.ARBIT_MAC_ADDR, "12:34:56:78:90:00");
    ARP_config.ARP_HARDWARE_TYPE_UNKNOWN = 0x0002;
    ARP_config.UNKNOWN_HW_ADDR_LEN = 7;
    ARP_config.ARP_PROTOCOL_UNKNOWN = 0x1234;
    ARP_config.UNKNOWN_PROTCOL_ADDR_LEN = 5;
    ARP_config.ARP_TOLERANCE_TIME = 1;
    ARP_config.ETHERNET_ADDR_LEN = 6;
    ARP_config.ARP_HARDWARE_ETHERNET = 0x0001;
    ARP_config.IP_ADDR_LEN = 4;
    ARP_config.ARP_PROTOCOL_IP = 0x0800;
    ARP_config.OPERATION_REQUEST = 1;
    strcpy((char*)ARP_config.all_zeroes, "00:00:00:00:00:00");
    strcpy((char*)ARP_config.ETHERNET_BROADCAST_ADDR, "ff:ff:ff:ff:ff:ff");
    strcpy((char*)ARP_config.IP_FIRST_UNUSED_ADDR_INTERFACE_1, "192.168.100.200");
    ARP_config.OPERATION_RESPONSE = 2;
}

int set_ARP_config(Arp_config_t new_ARP_config)
{   
    ARP_config.DYNAMIC_ARP_CACHETIMEOUT = new_ARP_config.DYNAMIC_ARP_CACHETIMEOUT;
    strcpy((char*)ARP_config.HOST_1_IP,(char*)new_ARP_config.HOST_1_IP);
    strcpy((char*)ARP_config.DIface_0_IP,(char*)new_ARP_config.DIface_0_IP);
    ARP_config.ParamListenTime = new_ARP_config.ParamListenTime;
    strcpy((char*)ARP_config.MAC_ADDR1,(char*)new_ARP_config.MAC_ADDR1);
    strcpy((char*)ARP_config.MAC_ADDR2,(char*)new_ARP_config.MAC_ADDR2);
    strcpy((char*)ARP_config.MAC_ADDR3,(char*)new_ARP_config.MAC_ADDR3);
    strcpy((char*)ARP_config.DIFACE_O_MAC_ADDR, (char*)new_ARP_config.DIFACE_O_MAC_ADDR);
    strcpy((char*)ARP_config.ARBIT_MAC_ADDR, (char*)new_ARP_config.ARBIT_MAC_ADDR);
    ARP_config.ARP_HARDWARE_TYPE_UNKNOWN = new_ARP_config.ARP_HARDWARE_TYPE_UNKNOWN;
    ARP_config.UNKNOWN_HW_ADDR_LEN = new_ARP_config.UNKNOWN_HW_ADDR_LEN;
    ARP_config.ARP_PROTOCOL_UNKNOWN = new_ARP_config.ARP_PROTOCOL_UNKNOWN;
    ARP_config.UNKNOWN_PROTCOL_ADDR_LEN = new_ARP_config.UNKNOWN_PROTCOL_ADDR_LEN;
    ARP_config.ARP_TOLERANCE_TIME = new_ARP_config.ARP_TOLERANCE_TIME;
    ARP_config.ETHERNET_ADDR_LEN = new_ARP_config.ETHERNET_ADDR_LEN;
    ARP_config.ARP_HARDWARE_ETHERNET = new_ARP_config.ARP_HARDWARE_ETHERNET;
    ARP_config.ARP_PROTOCOL_IP = new_ARP_config.ARP_PROTOCOL_IP;
    ARP_config.OPERATION_REQUEST = new_ARP_config.OPERATION_REQUEST;
    strcpy((char*)ARP_config.all_zeroes, (char*)new_ARP_config.all_zeroes);
    strcpy((char*)ARP_config.ETHERNET_BROADCAST_ADDR, (char*)new_ARP_config.ETHERNET_BROADCAST_ADDR);
    strcpy((char*)ARP_config.IP_FIRST_UNUSED_ADDR_INTERFACE_1, (char*)new_ARP_config.IP_FIRST_UNUSED_ADDR_INTERFACE_1);
    ARP_config.OPERATION_RESPONSE = new_ARP_config.OPERATION_RESPONSE;
}

Arp_config_t get_ARP_config()
{
    return ARP_config;
}