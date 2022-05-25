#ifndef TCP_H
#define TCP_H

#include "layer.h"
#include "common.h"


typedef struct 
{
    uint16_t SrcPort;
    uint16_t DstPort;
    uint32_t SeqNumber;
    uint32_t AckNumber;
    uint16_t Reserved: 4,
             HeaderLength: 4,
             FlagFIN:1,
             FlagSYN:1,
             FlagRST:1,
             FlagPSH:1,
             FlagACK:1,
             FlagURG:1,
             Flagece:1,
		         Flagcwr:1;
    uint16_t WindowsSize;
    uint16_t Checksum;
    uint16_t UrgPointer;


}   TCPHeader;
 
class tcp : public layer
 {
   public:
    tcp();
    tcp(char*);
    ~tcp();
    uint16_t Get_SrcPort();
    uint16_t Get_DstPort();
    uint32_t Get_SeqNumber();
    uint32_t Get_AckNumber();
    uint16_t Get_Reserved();
    int Get_Header_Length();
    int Get_FlagURG();
    int Get_FlagACK();
    int Get_FlagPSH();
    int Get_FlagRST();
    int Get_FlagSYN();
    int Get_FlagFIN();
    uint16_t Get_WindowsSize();
    uint16_t Get_Checksum();
    uint16_t Get_UrgPointer();
    void Set_SrcPort(uint16_t);
    void Set_DstPort(uint16_t);
    void Set_SeqNumber(uint32_t);
    void Set_AckNumber(uint32_t);
    void Set_Reserved(uint16_t);
    void Set_Header_Length(int);
    void Set_FlagURG(int);
    void Set_FlagACK(int);
    void Set_FlagPSH(int);
    void Set_FlagRST(int);
    void Set_FlagSYN(int);
    void Set_FlagFIN(int);
    void Set_WindowsSize(uint16_t);
    void Set_Checksum(uint16_t);
    void Set_UrgPointer(uint16_t);
    virtual const char * Get_header_data();
 
   private:
   TCPHeader m_tcpHeader; 

};


#endif
