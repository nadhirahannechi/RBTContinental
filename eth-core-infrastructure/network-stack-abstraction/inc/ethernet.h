#ifndef ETH_H
#define ETH_H

#include "layer.h"
#include "common.h"


typedef struct 
{
    unsigned char  destinationMAC[6];
    unsigned char  sourceMac[6];
    uint16_t type;
}ethernet_header;


class ethernet : public layer
 {
   public:
    ethernet();
    ethernet( char * );
    ~ethernet();
    std::string Get_DestinationMAC();
    std::string Get_SourceMac();
    uint16_t Get_Type();
    void Set_DestinationMAC(std::string);
    void Set_SourceMac(std::string);
    void Set_Type(uint16_t);
    virtual const char * Get_header_data();
  
   private:
   ethernet_header m_Eth_header;
};

#endif
