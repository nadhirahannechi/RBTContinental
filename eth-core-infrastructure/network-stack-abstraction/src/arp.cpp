#include"arp.h"


using namespace std;

arp::arp() : layer(ARP_LAYER_Code, sizeof(arp_header))
{   
    m_length = sizeof(arp_header);
    m_arp_header.ar_hln = 6;
    m_arp_header.ar_hrd = htons(1); // Ethernet
    m_arp_header.ar_op = htons(ARP_REQUEST);
    m_arp_header.ar_pln = 4;
    m_arp_header.ar_pro = htons(ETH_IP);
    memset(m_arp_header.arp_sha,0,6);
    memset(m_arp_header.arp_spa,0,4);
    memset(m_arp_header.arp_tha,0xff,6);
    memset(m_arp_header.arp_tpa,0,4);
}

arp::arp( char * header) : layer(  ARP_LAYER_Code ,sizeof(arp_header))
{
    memcpy(&m_arp_header, header,sizeof(arp_header));   
}

arp::~arp() 
{

}

uint16_t arp::Get_ar_hdr()
{
    return htons(m_arp_header.ar_hrd);
}

uint16_t arp::Get_ar_pro()
{
    return htons(m_arp_header.ar_pro);
}

uint8_t arp::Get_ar_hln()
{
    return m_arp_header.ar_hln;
}

uint8_t arp::Get_ar_pln()
{
    return m_arp_header.ar_pln;
}

uint16_t arp::Get_ar_op()
{
    return htons(m_arp_header.ar_op);
}


string arp::Get_sha()
{
    return mac_to_string(m_arp_header.arp_sha);
}

string arp::Get_spa()
{
    return ipadress_to_string(m_arp_header.arp_spa);
}

string arp::Get_tha()
{
    return mac_to_string(m_arp_header.arp_tha);
}

string arp::Get_tpa()
{
    return ipadress_to_string(m_arp_header.arp_tpa);
}

void arp::Set_ar_hdr(uint16_t ar_hdr)
{
    m_arp_header.ar_hrd = htons(ar_hdr);
}

void arp::Set_ar_pro(uint16_t ar_pro)
{
    m_arp_header.ar_pro = htons(ar_pro);
}

void arp::Set_ar_hln(uint8_t ar_hln)
{
    m_arp_header.ar_hln = ar_hln;
}

void arp::Set_ar_pln(uint8_t ar_pln)
{
    m_arp_header.ar_pln = ar_pln;
}

void arp::Set_ar_op(uint16_t ar_op)
{
    m_arp_header.ar_op  = htons(ar_op);
}


void arp::Set_sha(string sha)
{
    string_to_mac(sha,m_arp_header.arp_sha);
}

void arp::Set_spa(string spa)
{
    string_to_ipadress(spa, m_arp_header.arp_spa);
}

void arp::Set_tha(string tha)
{
    string_to_mac(tha,m_arp_header.arp_tha);
}

void arp::Set_tpa(string tpa)
{
    string_to_ipadress(tpa, m_arp_header.arp_tpa);
}

const char * arp::Get_header_data()
{
    return (char*)&m_arp_header;
}

