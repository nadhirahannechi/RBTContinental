#include "icmp.h"



icmp :: icmp() : layer(ICMP_LAYER_Code,sizeof(icmp_header))
{
    m_icmp_header.type=ICMP_ECHO_REQUEST;
    m_icmp_header.code=0;
    m_icmp_header.checksum = 0;
    m_icmp_header.un.echo1.identifier = 1;//rand();
    m_icmp_header.un.echo1.sequence = 1;//rand();
}

icmp :: icmp(char * header) : layer(ICMP_LAYER_Code,sizeof(icmp_header))
{
   memcpy(&m_icmp_header, header, sizeof(icmp_header));
}

icmp :: ~icmp()
{

}

uint8_t icmp::Get_type()
{
    return m_icmp_header.type;
}

uint8_t icmp::Get_code()
{
    return  m_icmp_header.code;
}

uint16_t icmp::Get_checksum()
{
    return htons(m_icmp_header.checksum);
}


uint16_t icmp::Get_identifier()
{
    return htons(m_icmp_header.un.echo1.identifier);
}


uint16_t icmp::Get_seuqence()
{
    return htons(m_icmp_header.un.echo1.sequence);
}

uint8_t icmp::Get_pointer()
{
    return m_icmp_header.un.echo2.ptr;
}

void icmp::Set_type(uint8_t type)
{
    m_icmp_header.type=type;
}

void icmp::Set_code(uint8_t code)
{
    m_icmp_header.code=code;
}

void icmp::Set_checksum(uint16_t checksum)
{
    m_icmp_header.checksum = htons(checksum);
}

void icmp::Set_identifer(uint16_t id)
{
    m_icmp_header.un.echo1.identifier = htons(id);
}

void icmp::Set_sequence(uint16_t seq)
{
    m_icmp_header.un.echo1.sequence = htons(seq);
}

const char * icmp::Get_header_data()
{
    return ( char *)&m_icmp_header;
}


void icmp::Set_CorrectHeaderChecksum(char* payloaddata)
{
    char *data = (char*) malloc (sizeof(icmp_header)+strlen(payloaddata));
    memcpy(data, this->Get_header_data(), sizeof(icmp_header));
    memcpy(data+sizeof(icmp_header), payloaddata, strlen(payloaddata));
    this->Set_checksum(in_cksum((unsigned short*)data,sizeof(icmp_header)+strlen(payloaddata)));
    delete(data);
}
