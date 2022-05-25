#include "ethernet.h"


ethernet :: ethernet() : layer( ETH_LAYER_Code,sizeof(ethernet_header))
{   
    memset(m_Eth_header.destinationMAC,0,6);
    memset(m_Eth_header.sourceMac,0,6);
    m_Eth_header.type = htons(ETH_IP); // ipv4
}

ethernet :: ethernet( char * header ) : layer( ETH_LAYER_Code,sizeof(ethernet_header))
{   
    memcpy( &m_Eth_header, header, sizeof(ethernet_header));
   
}

ethernet::~ethernet()
{
}

std::string ethernet::Get_DestinationMAC()
{
    return mac_to_string(m_Eth_header.destinationMAC);
}

std::string ethernet::Get_SourceMac()
{
    return mac_to_string(m_Eth_header.sourceMac);
}

uint16_t ethernet::Get_Type()
{
    return m_Eth_header.type;
}

void ethernet::Set_DestinationMAC(std::string DestinationMAC)
{
    string_to_mac(DestinationMAC,m_Eth_header.destinationMAC);
}
void ethernet::Set_SourceMac(std::string SourceMac)
{
    string_to_mac(SourceMac,m_Eth_header.sourceMac);
}

void ethernet::Set_Type(uint16_t Type)
{   
    m_Eth_header.type = Type;
}

const char * ethernet::Get_header_data()
{ 
  return (char *)&m_Eth_header;
}
