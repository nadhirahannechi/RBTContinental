#include <iostream>
#include "ethernet.h"
#include "ip.h"
#include "icmp.h"
#include "packet.h"
#include "payload.h"
#include "common.h"
#include "ip_option.h"
#include "sniffer.h"
#include <cstring>
#include <netinet/in.h>

using namespace std;

int main()
{
    /* test Send iCMPH Packet */
    string interface("enp0s3");
    ethernet *eth= new ethernet();
    eth->Set_DestinationMAC("08:00:27:68:41:d5");
    eth->Set_SourceMac("08:00:27:cd:64:f1");
    eth->Set_Type(htons(0x0800));

    ip *IP= new ip();
    IP->Set_IHL(15);
    IP->Set_TotalLength(80);
    IP->Set_Identification(0x0100);
    IP->Set_Protocol(1);
    IP->Set_DstAddress("192.168.20.248");
    IP->Set_SrcAddress("192.168.20.83");
    IP->Set_HeaderChecksum(0x82fb);

    ipOption* IP_OPTION = new ipOption();

    IP_OPTION->Set_ipt_code(68);
    IP_OPTION->Set_ipt_len(10);
    IP_OPTION->Set_ipt_ptr(9);
    IP_OPTION->Set_ipt_flg(0);
    IP_OPTION->Set_ipt_oflw(0);
    uint32_t data[9];
    memset(data,0,36);
    data[0] = 2000000000;
    IP_OPTION->Set_ipt_data(data);

    icmp* iCMPH = new icmp();
    iCMPH->Set_identifer(0x0100);
    iCMPH->Set_sequence(0x0100);
    iCMPH->Set_checksum(0x70ad);

    payload* p=new payload("testIPpPtion");
    
    packet *packet1 = new packet();
    packet1->AddLayer(eth);
    packet1->AddLayer(IP);
    packet1->AddLayer(IP_OPTION);
    packet1->AddLayer(iCMPH);
    packet1->AddLayer(p);

    packet1->sendPacket(interface.c_str());

    sniffer sniff;
    filter filter;
    filter.ETHType =htons(ETH_IP);
    filter.IPproto=ICMP_LAYER_Code;
    strcpy(filter.SrcIPAddress,"192.168.20.248");
    strcpy(filter.DstIPAddress,"192.168.20.83");

    sniff.Set_Filter(filter);
    sniff.Set_Timeout(2);
    sniff.Set_Interface((char *)"enp0s3");
    sniff.Start_Sniff();
    packet pa=sniff.Get_Packet();
    icmp * icmph = new icmp();
    pa.GetLayer(ICMP_LAYER_Code,(layer**)&icmph);
    if(icmph->Get_pointer() == 22)
    {
        cout << "\ntest passed : " << +icmph->Get_pointer() << endl;
    }
    else
    {
        cout << "test failed : " <<  +icmph->Get_pointer() << endl;
    }

    delete(eth);
    delete(IP);
    delete(IP_OPTION);
    delete(iCMPH);
    delete(p);
    delete(packet1);
}