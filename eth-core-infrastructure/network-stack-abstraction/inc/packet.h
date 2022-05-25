#ifndef PACKET_H
#define PACKET_H
#include <iostream>
#include <string>
#include <stdint.h>
#include <vector> 
#include "layer.h"
#include "common.h"
typedef struct{
uint8_t data[MAX_SIZE_IP_PACKET];
int length;
}packetData;

class packet 
{
   public:
      packet();
      packet( char *,uint32_t);
      ~packet();
      void AddLayer(layer * );
      int GetLayer(int ,layer **);
      int sendPacket(const char * );
      void sendArpPacket(const char * );
      packet sendRecvPacket(const char * );
      void printPacket(void);
      int SetCorrectLength(void);
      int Set_CorrectChecksum(void);
      int Verify_Checksum(void);
      int cleanUp(void);


      packetData GetPacketData();

   private:
      std::vector<layer*> LayerStack;
     


};
#endif