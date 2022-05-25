#include "ip.h"
#include "common.h"
#include <cstring>
#include <netinet/in.h>

using namespace std;
ip :: ip() : layer(  IP_LAYER_Code ,sizeof(IpHeader))
{
    m_header.IHL =  5;
    m_header.Version= 4;
    m_header.TOS = 0;
    m_header.TotalLength = 100;
    m_header.Identification = 1;//rand(); 
    m_header.Offset = 0;
    m_header.TTL = 255;
    m_header.Protocol = 0;
    m_header.HeaderChecksum = 0; 
    this->Set_SrcAddress("0.0.0.0");
    this->Set_DstAddress("0.0.0.0");
}

ip :: ip( char * header) : layer(  IP_LAYER_Code ,sizeof(IpHeader))
{
    memcpy(&m_header, header,sizeof(IpHeader));
    
}

ip :: ~ip() 
{

}

unsigned int ip ::  Get_Version()
{
    return m_header.Version;
}

unsigned int ip ::Get_IHL()
{
    return m_header.IHL;
}
   
uint8_t ip ::Get_TOS()
{
    return m_header.TOS;
}

uint16_t ip ::Get_TotalLength()
{
    return htons(m_header.TotalLength);
}

uint16_t ip ::Get_Identification()
{
    return htons(m_header.Identification);
}

uint16_t ip ::Get_Offset()
{
    return htons(m_header.Offset);
}


uint8_t ip ::Get_TTL()
{
    return m_header.TTL;
}

uint8_t ip ::Get_Protocol()
{
    return m_header.Protocol;
}

uint16_t ip ::Get_HeaderChecksum()
{
    return htons(m_header.HeaderChecksum);
}

string ip ::Get_SrcAddress()
{
    return ipadress_to_string(m_header.SrcAddress);
}

string ip ::Get_DstAddress()
{
    return ipadress_to_string(m_header.DstAddress);

}

void ip ::Set_Version(unsigned int Version)
{
    m_header.Version=Version;
}

void ip ::Set_IHL(unsigned int IHL)
{
    m_header.IHL=IHL;
}

void ip ::Set_TOS(uint8_t TOS)
{
    m_header.TOS = TOS;
}

void ip ::Set_TotalLength(uint16_t TotalLength)
{
    m_header.TotalLength = htons(TotalLength);
}

void ip ::Set_Identification(uint16_t ID)
{
    m_header.Identification= htons(ID);
}

void ip ::Set_Offset(uint16_t Offset)
{
    m_header.Offset=htons(Offset);
}



void ip ::Set_TTL(uint8_t TTL)
{
    m_header.TTL= TTL;
}

void ip ::Set_Protocol(uint8_t Protocol)
{
    m_header.Protocol = Protocol;
}

void ip ::Set_HeaderChecksum(uint16_t HeaderChecksum)
{
    m_header.HeaderChecksum = htons(HeaderChecksum);
}

void ip ::Set_CorrectHeaderChecksum()
{   
    m_header.HeaderChecksum = in_cksum((unsigned short*)&m_header, sizeof(IpHeader));
}

void ip ::Set_SrcAddress(string srcAdress )
{
    string_to_ipadress(srcAdress, m_header.SrcAddress);
}

void ip ::Set_DstAddress(string dstAdress)
{
    string_to_ipadress(dstAdress, m_header.DstAddress);

}

const char * ip::Get_header_data()
{   
   return (char*)&m_header;
}
