#include <iostream>
#include "ethernet.h"
#include "ip.h"
#include "icmp.h"
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
    eth->Set_DestinationMAC("50:9a:4c:2c:87:38");
    eth->Set_SourceMac("08:00:27:cd:64:f1");
    ip *IP= new ip();
    IP->Set_IHL(5);
    IP->Set_TotalLength(38);
    IP->Set_Protocol(1);
    IP->Set_DstAddress("192.168.20.76");
    IP->Set_SrcAddress("192.168.20.83");
    IP->Set_CorrectHeaderChecksum();

    icmp* ICMP = new icmp();
    ICMP->Set_checksum(htons(0x2e99));

    payload* p=new payload("dffffffffd");
    packet *packet1 = new packet();
    packet1->AddLayer(eth);
    packet1->AddLayer(IP);
    packet1->AddLayer(ICMP);
    packet1->AddLayer(p);
    packet1->printPacket();

    packet1->sendPacket(interface.c_str());

    delete(eth);
    delete(IP);
    delete(ICMP);
    delete(p);
    delete(packet1); 
}
