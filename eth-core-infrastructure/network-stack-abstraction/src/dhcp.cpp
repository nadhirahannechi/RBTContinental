#include"dhcp.h"

using namespace std;


dhcp::dhcp() : layer(DHCP_LAYER_Code,sizeof(dhcp_header))
{
    m_length = sizeof(dhcp_header);
    m_dhcp_header.op = BOOTREPLY;  
    m_dhcp_header.htype = ETHERNET_HARDWARE_ADDRESS; 
    m_dhcp_header.hlen = ETHERNET_HARDWARE_ADDRESS_LENGTH;  
    m_dhcp_header.hops = 0;  
    m_dhcp_header.xid = 0;   
    m_dhcp_header.secs = 0;  
    m_dhcp_header.flags = 0; 
    memset(m_dhcp_header.ciaddr,0,4);
    memset(m_dhcp_header.yiaddr,0,4);
    memset(m_dhcp_header.siaddr,0,4);
    memset(m_dhcp_header.giaddr,0,4);
    memset(m_dhcp_header.chaddr,0,MAX_DHCP_CHADDR_LENGTH);
    memset(m_dhcp_header.sname,0,MAX_DHCP_SNAME_LENGTH);
    memset(m_dhcp_header.file,0,MAX_DHCP_FILE_LENGTH);
    memset(m_dhcp_header.options,0,MAX_DHCP_OPTIONS_LENGTH);
    sname_ptr = 0;
    file_ptr = 0;
    option_ptr = 0;
}

dhcp::dhcp( char * header) : layer(  DHCP_LAYER_Code ,sizeof(dhcp_header))
{
    memcpy(&m_dhcp_header, header,sizeof(dhcp_header));   
}

dhcp::~dhcp() 
{

}

uint8_t dhcp::Get_op()
{
    return m_dhcp_header.op;
}

uint8_t dhcp::Get_htype()
{
    return m_dhcp_header.htype;
}

uint8_t dhcp::Get_hlen()
{
    return m_dhcp_header.hlen;
}

uint8_t dhcp::Get_hops()
{
    return m_dhcp_header.hops;
}

uint32_t dhcp::Get_xid()
{
    return htonl(m_dhcp_header.xid);
}

uint16_t dhcp::Get_secs()
{
    return htons(m_dhcp_header.secs);
}

uint16_t dhcp::Get_flags()
{
    return htons(m_dhcp_header.flags);
}

string dhcp::Get_ciaddr()
{
    return ipadress_to_string(m_dhcp_header.ciaddr);
}

string dhcp::Get_yiaddr()
{
    return ipadress_to_string(m_dhcp_header.yiaddr);
}

string dhcp::Get_siaddr()
{
    return ipadress_to_string(m_dhcp_header.siaddr);
}

string dhcp::Get_giaddr()
{
    return ipadress_to_string(m_dhcp_header.giaddr);
}

string dhcp::Get_chaddr()
{
    mac_to_string(m_dhcp_header.chaddr);
}

uint8_t* dhcp::Get_file()
{
    return (uint8_t*)m_dhcp_header.file;
}

uint8_t* dhcp::Get_sname()
{
    return (uint8_t*)m_dhcp_header.sname;
}

uint8_t* dhcp::Get_options()
{
    return (uint8_t*)m_dhcp_header.options;
}

void dhcp::Set_op(uint8_t op)
{
    m_dhcp_header.op  = op;
}

void dhcp::Set_htype(uint8_t htype)
{
    m_dhcp_header.htype = htype;
}

void dhcp::Set_hlen(uint8_t hlen)
{
    m_dhcp_header.hlen = hlen;
}

void dhcp::Set_hops(uint8_t hops)
{   
    m_dhcp_header.hops = hops;
}

void dhcp::Set_xid(uint32_t xid)
{
    m_dhcp_header.xid = htonl(xid);
}

void dhcp::Set_secs(uint16_t secs)
{
    m_dhcp_header.secs = htons(secs);
}

void dhcp::Set_flags(uint16_t flags)
{
    m_dhcp_header.flags = htons(flags);
}

void dhcp::Set_ciaddr(string ciaddr)
{
    string_to_ipadress(ciaddr,m_dhcp_header.ciaddr);
}

void dhcp::Set_yiaddr(string yiaddr)
{
    string_to_ipadress(yiaddr,m_dhcp_header.yiaddr);
}

void dhcp::Set_siaddr(string siaddr)
{
    string_to_ipadress(siaddr,m_dhcp_header.siaddr);
}

void dhcp::Set_giaddr(string giaddr)
{
    string_to_ipadress(giaddr,m_dhcp_header.giaddr);
}

void dhcp::Set_chaddr(string chaddr)
{
    string_to_mac(chaddr,m_dhcp_header.chaddr);
}

void dhcp::Set_sname(char * ptr_sname, int len_option)
{
    memcpy(&m_dhcp_header.options[option_ptr], ptr_sname, len_option);
    sname_ptr += len_option;
}
void dhcp::Set_file(char * ptr_file, int len_option)
{
    memcpy(&m_dhcp_header.options[option_ptr], ptr_file, len_option);
    file_ptr += len_option;
}
void dhcp::Set_option(char * ptr_option, int len_option)
{  
    memcpy(m_dhcp_header.options,ptr_option, len_option);
}

const char * dhcp::Get_header_data()
{
    return (char*)&m_dhcp_header;
}

