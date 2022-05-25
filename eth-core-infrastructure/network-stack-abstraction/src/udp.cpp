#include "udp.h"
#include "common.h"
#include <cstring>
#include <netinet/in.h>

using namespace std;

udp :: udp() : layer(  UDP_LAYER_Code ,sizeof(UDPHeader))
{
    m_UDPHeader.SrcPort=0;
    m_UDPHeader.DstPort=0;
    m_UDPHeader.Checksum=0;
    m_UDPHeader.Length=0;
}


udp :: udp ( char * header) : layer(  UDP_LAYER_Code ,sizeof(UDPHeader))
{
    memcpy(&m_UDPHeader, header,sizeof(UDPHeader));

}

udp :: ~udp() 
{

}

uint16_t udp ::Get_SrcPort()
{
    return htons(m_UDPHeader.SrcPort);
}

uint16_t udp ::Get_DstPort()
{
    return htons(m_UDPHeader.DstPort);
}

uint16_t udp ::Get_Length()
{
    return htons(m_UDPHeader.Length);
}

uint16_t udp ::Get_Checksum()
{
    return htons(m_UDPHeader.Checksum);
}

void udp ::Set_SrcPort(uint16_t SrcPort)
{
     m_UDPHeader.SrcPort = htons(SrcPort);
}

void udp ::Set_DstPort(uint16_t DstPort)
{
    m_UDPHeader.DstPort = htons(DstPort);
}

void udp ::Set_Length(uint16_t Length)
{
    m_UDPHeader.Length = htons(Length);
}

void udp ::Set_Checksum(uint16_t checksum)
{
    m_UDPHeader.Checksum = htons(checksum);
}

const char * udp ::Get_header_data()
{
    return (char*)&m_UDPHeader;

}