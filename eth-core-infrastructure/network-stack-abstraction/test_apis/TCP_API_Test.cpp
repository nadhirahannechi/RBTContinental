#include "AbstractionAPI.h"
#include<cstdio>
#include<string>
#include<cstring>
#include <arpa/inet.h>

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
   TCP_Packet TCP_P = CreateTCP();
   //EditTCPField(&TCP_P, TCP, TCP_Checksum, (void*)0x9aba);
   EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void*)1);
   TCP_MSS_OPTION mss;
   mss.length=4;
   mss.value=htons(521);

   uint8_t options[4];
    options[0]=TCP_NO_OPERATION_OPTION ;
    options[1]=TCP_NO_OPERATION_OPTION ;
    options[2]=TCP_NO_OPERATION_OPTION ;
    options[3]=TCP_END_OF_OPTION ;


   TCP_Add_Option(&TCP_P,(uint8_t*)options,4);
      TCP_Add_Option(&TCP_P,(uint8_t*)&mss,4);




   TCP_Compute_checksum(&TCP_P);

   SendTCP(TCP_P);
   uint8_t value ;
   uint16_t v;
   Get_TCP_Option_Value(TCP_P, 2,(uint8_t*)&v);
   printf(" value =============== %d \n", htons(v));
    Get_TCP_Option_Value(TCP_P, 1,&value);
    printf(" value =============== %d \n", value);
    Get_TCP_Option_Value(TCP_P, 0,&value);
    printf(" value =============== %d \n", value);


   /*EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void*)8080);
   EditTCPField(&TCP_P, TCP, TCP_DstPort, (void*)8080);
   EditTCPField(&TCP_P, TCP, TCP_Reserved, (void*)0b0101);
   cout<< "verify checksum"<<TCP_Verify_Correct_checksum(TCP_P)<<endl;
   TCP_Compute_checksum(&TCP_P);
      cout<< "verify checksum"<<TCP_Verify_Correct_checksum(TCP_P)<<endl;

   packet pp((char*)TCP_P.data);
   pp.printPacket();
   EditTCPField(&TCP_P, TCP, TCP_Reserved, (void*)0b1101);
   TCP_Compute_checksum(&TCP_P);
   packet pp2((char*)TCP_P.data);
   pp2.printPacket();
   SendTCP(TCP_P);
   Packet_filter f;

   f.Srcport = htons(8080);
   f.Dstport = htons(8080);
   TCP_Packet TCP_RP = ListenTCP(f);
   packet p((char*)TCP_RP.data);
   p.printPacket();
   if (TCP_RP.length != 0) {
       int rRST = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagRST);
       if (rRST == 1) {
           printf("DUT sent RST packet --> test passed\n");
       } else printf("No RST packet detected --> test failed\n");
   } else printf("Test failed!! -> no packet recieved");

   cout<<"reserver == "<< GetTCPField(&TCP_P, TCP, TCP_Reserved)<<endl;*/
}
