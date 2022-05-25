#ifndef IP_H
#define IP_H
#include <iostream>
#include <string>
#include "layer.h"
 
typedef struct 
{
    unsigned int IHL:4;
    unsigned int Version:4;
    uint8_t TOS;
    uint16_t TotalLength;
    uint16_t Identification;
    uint16_t Offset;
    uint8_t TTL;
    uint8_t Protocol;
    uint16_t HeaderChecksum;
    uint8_t SrcAddress[4];
    uint8_t DstAddress[4];
} IpHeader;
 
class ip : public layer
 {
   public:
    ip();
    ip( char *);
    ~ip();
    unsigned int Get_Version();
    unsigned int Get_IHL();
    uint8_t Get_TOS();
    uint16_t Get_TotalLength();
    uint16_t Get_Identification();
    uint16_t Get_Offset();
    uint8_t Get_TTL();
    uint8_t Get_Protocol();
    uint16_t Get_HeaderChecksum();
    std::string Get_SrcAddress();
    std::string Get_DstAddress();
    void Set_Version(unsigned int);
    void Set_IHL(unsigned int);
    void Set_TOS(uint8_t);
    void Set_TotalLength(uint16_t);
    void Set_Identification(uint16_t);
    void Set_Offset(uint16_t);
    void Set_TTL(uint8_t);
    void Set_Protocol(uint8_t);
    void Set_HeaderChecksum(uint16_t);
    void Set_CorrectHeaderChecksum(void);
    void Set_SrcAddress(std::string);
    void Set_DstAddress(std::string);
    virtual const char * Get_header_data();
 
   private:
    IpHeader m_header;
};


#endif
