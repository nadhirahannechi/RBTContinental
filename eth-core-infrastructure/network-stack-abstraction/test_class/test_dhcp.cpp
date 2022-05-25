#include <iostream>
#include "ethernet.h"
#include "ip.h"
#include "udp.h"
#include "dhcp.h"
#include "packet.h"
#include "payload.h"
#include "common.h"
#include <cstring>
#include <netinet/in.h>

using namespace std;
int main()
{  
       
    
       /* test Send ICMP Packet */
    string interface("enp0s3");
    ethernet *eth= new ethernet();
    eth->Set_DestinationMAC("ff:ff:ff:ff:ff:ff");
    eth->Set_SourceMac("08:00:27:cd:64:f1");

    ip *IP= new ip();
    
    IP->Set_TotalLength(576);
    IP->Set_Protocol(17);
    IP->Set_DstAddress("255.255.255.255");
    IP->Set_SrcAddress("0.0.0.0");
    IP->Set_CorrectHeaderChecksum();
    
    udp *udph = new udp();
    udph->Set_SrcPort(DHCP_SERVER_PORT);
    udph->Set_DstPort(DHCP_CLIENT_PORT);
    udph->Set_Length(556);
    dhcp *dhcph = new dhcp();
    dhcph->Set_yiaddr("192.168.20.200");
    dhcph->Set_siaddr("192.168.20.83");
    //dhcph->Set_chaddr("08:00:27:cd:64:f1");
    unsigned char  option[100];
    option[0] = '\x63';
	option[1] = '\x82';
	option[2] = '\x53';
	option[3] = '\x63';
    
	option[4] = '\x35';
	option[5] = '\x01';
    option[6] = '\x02';
   
    option[7] =  '\x33';
    option[8] =  '\x04';
    option[9] =  '\x0e';
    option[10] = '\x10';
    option[11] = '\x10';
    option[12] = '\x10';
    
    option[13] = '\xff';
    dhcph->Set_option((char*)option,14);

    //dhcph->Set_option((char*)option,12);

    packet *packet1 = new packet();
    packet1->AddLayer(eth);
    packet1->AddLayer(IP);
     packet1->AddLayer(udph);
    packet1->AddLayer(dhcph);
    packet1->sendPacket(interface.c_str());

    delete(eth);
    delete(IP);
    delete(udph);
    delete(dhcph);
    delete(packet1);


    
}
