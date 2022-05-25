#include <iostream>
#include "ethernet.h"
#include<stdio.h>
#include <netinet/in.h>
using namespace std;
int main()
{   
     
    // test ethernet layer 
    ethernet eth;
    
    cout << eth.Get_DestinationMAC()<<endl;
    cout << eth.Get_SourceMac()<<endl;
    printf("%.4X\n",eth.Get_Type());
    eth.Set_DestinationMAC("aa:bb:cc:dd:2f:78");
    eth.Set_SourceMac("b8:c9:28:ea:2f:aa");
    eth.Set_Type(0x0808);
    cout << eth.Get_DestinationMAC()<<endl;
    cout << eth.Get_SourceMac()<<endl;
    printf("%.4X\n",eth.Get_Type());
    
    
    /*
    // test ip lyaer
    ip *IP= new ip();
    IP->Set_Fragment_Offset( (unsigned int) 0x01 );
    IpHeader* h= (IpHeader*) (IP->Get_header_data());
    cout<<h->FragmentOffset<<endl;
    cout<<IP->Get_SrcAddress()<<endl;
    cout<<IP->Get_DstAddress()<<endl;
    IP->Set_DstAddress("192.168.20.14");
    IP->Set_SrcAddress("192.54.15.13");
    cout<<IP->Get_SrcAddress()<<endl;
    cout<<IP->Get_DstAddress()<<endl;
    //cout<<<<endl;
    //std::string str(IP->Get_header_data());
    //cout<<str<<sizeof(str)<<endl;
    //cout<<sizeof(IP->Get_header_data())<<endl;
    cout<<IP->Get_Version()<<endl;
    cout<<IP->Get_IHL()<<endl;
    cout<<IP->Get_TOS()<<endl;
    cout<<IP->Get_TotalLength()<<endl;
    cout<<IP->Get_Identification()<<endl;
    cout<<IP->Get_Flags()<<endl;
    cout<<IP->Get_Fragment_Offset()<<endl;
    cout<<IP->Get_TTL()<<endl;
    cout<<IP->Get_Protocol()<<endl;
    cout<<IP->Get_HeaderChecksum()<<endl;
    cout<<IP->Get_SrcAddress()<<endl;
    cout<<IP->Get_DstAddress()<<endl<<endl<<endl;
    IP->Set_Version(5);
    IP->Set_IHL(12);
    IP->Set_TOS(2);
    IP->Set_TotalLength((uint16_t)16);
    IP->Set_Identification(10);
    IP->Set_Flags(2);
    IP->Set_Fragment_Offset(2);
    IP->Set_TTL(5);
    IP->Set_Protocol(9);
    IP->Set_HeaderChecksum(0x57);
    IP->Set_DstAddress("2.0.0.0");
    IP->Set_SrcAddress("15.54.15.13");
    cout<<IP->Get_Version()<<endl;
    cout<<IP->Get_IHL()<<endl;
    cout<<"tos "<<(int ) IP->Get_TOS()<<endl;
    cout<<"totL "<<IP->Get_TotalLength()<<endl;
    cout<<"id "<<IP->Get_Identification()<<endl;
    cout<<"flag "<<IP->Get_Flags()<<endl;
    cout<<"f offset "<<IP->Get_Fragment_Offset()<<endl;
    cout<<"ttl "<< (int) IP->Get_TTL()<<endl;
    cout<<"proto "<< (int) IP->Get_Protocol()<<endl;
    cout<<"chacksum "<<IP->Get_HeaderChecksum()<<endl;
    cout<<"src"<<IP->Get_SrcAddress()<<endl;
    cout<<"dst"<<IP->Get_DstAddress()<<endl;

    //test icmp 
    icmp *icmph = new icmp();
    cout << "ID:" << icmph->Get_identifier() << endl;
    cout << "Type: " << +icmph->Get_type() << endl; // we use '+' before car cout can't print an unsigned char
    cout << "code: " << +icmph->Get_code() << endl;
    cout << "Checksum : " << icmph->Get_checksum() << endl;

    icmph->Set_identifer(150);
    icmph->Set_type(ICMP_ECHO_REQUEST);
    icmph->Set_checksum(0xffff);

    cout << "update..." << endl; 

    cout << "ID:" << icmph->Get_identifier() << endl;
    cout << "Type: " << +icmph->Get_type() << endl; 
    cout << "code: " << +icmph->Get_code() << endl;
    cout << "Checksum : " << icmph->Get_checksum() << endl;*/


    //test send packet
    /*
    ethernet *eth= new ethernet();
    eth->Set_DestinationMAC("08:00:27:bc:4c:d5");
    eth->Set_SourceMac("08:00:27:cd:64:f1");
    ip *IP= new ip();
    IP->Set_DstAddress("192.168.20.155");
    IP->Set_SrcAddress("192.168.20.83");
    IP->Set_Protocol(1);
    //IP->Set_TotalLength(htons(28));
    icmp* ICMP = new icmp();
    packet *packet1 = new packet();
    packet1->addLayer(eth);
    packet1->addLayer(IP);
    packet1->addLayer(ICMP);
    packet1->sendPacket();
    delete(IP);*/
}