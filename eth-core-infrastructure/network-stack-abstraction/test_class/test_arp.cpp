#include <iostream>
#include "ethernet.h"
#include "ip.h"
#include "arp.h"
#include "packet.h"
#include "payload.h"
#include "common.h"
#include <cstring>
#include <netinet/in.h>

using namespace std;
int main()
{  
    /* test Send ARP Packet */
    string interface("enp0s3");
    ethernet *eth= new ethernet();
    eth->Set_DestinationMAC("ff:ff:ff:ff:ff:ff");
    eth->Set_SourceMac("08:00:27:cd:64:f1");
    eth->Set_Type(htons(ETH_ARP));
    
    arp *arph = new arp();
    arph->Set_tha("00:00:00:00:00:00");
    arph->Set_tpa("192.168.20.101");
    arph->Set_spa("192.168.20.83");
  
    packet *packet1 = new packet();
    packet1->AddLayer(eth);
    packet1->AddLayer(arph);
    packet1->sendArpPacket(interface.c_str());

    delete(eth);
    delete(arph);
    delete(packet1); 
}
