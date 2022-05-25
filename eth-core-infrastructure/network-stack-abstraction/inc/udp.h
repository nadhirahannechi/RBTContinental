#ifndef UDP_H
#define UDP_H

#include "layer.h"
#include "common.h"


typedef struct 
{
    uint16_t SrcPort;
    uint16_t DstPort;
    uint16_t Length;
    uint16_t Checksum;

} UDPHeader;
 
class udp : public layer
 {
   public:
    udp();
    udp(char*);
    ~udp();
    uint16_t Get_SrcPort();
    uint16_t Get_DstPort();
    uint16_t Get_Length();
    uint16_t Get_Checksum();
    void Set_SrcPort(uint16_t);
    void Set_DstPort(uint16_t);
    void Set_Length(uint16_t);
    void Set_Checksum(uint16_t);

    virtual const char * Get_header_data();
 
   private:
    UDPHeader m_UDPHeader;
};


#endif
