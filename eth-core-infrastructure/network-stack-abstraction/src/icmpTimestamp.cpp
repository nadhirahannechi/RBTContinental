#include "icmpTimestamp.h"



icmpTimestamp::icmpTimestamp() : layer(ICMP_TIME_STAMP_LAYER_CODE,sizeof(icmp_time_stamp_header))
{
    m_icmp_timestamp_header.type =  ICMP_TIMESTAMP;
    m_icmp_timestamp_header.code = 0;
    m_icmp_timestamp_header.checksum = 0;
    m_icmp_timestamp_header.identifier = 1;
    m_icmp_timestamp_header.sequence = 1;
    m_icmp_timestamp_header.originate_time_stamp = rand();
    m_icmp_timestamp_header.receive_time_stamp = 0;
    m_icmp_timestamp_header.transmit_time_stamp = 0;
}

icmpTimestamp :: icmpTimestamp(char * header) : layer(ICMP_TIME_STAMP_LAYER_CODE,sizeof(icmp_time_stamp_header))
{
   memcpy(&m_icmp_timestamp_header, header, sizeof(icmp_time_stamp_header));
}

icmpTimestamp :: ~icmpTimestamp()
{

}

uint8_t icmpTimestamp::Get_type()
{
    return m_icmp_timestamp_header.type;
}

uint8_t icmpTimestamp::Get_code()
{
    return  m_icmp_timestamp_header.code;
}

uint16_t icmpTimestamp::Get_checksum()
{
    return htons(m_icmp_timestamp_header.checksum);
}


uint16_t icmpTimestamp::Get_identifier()
{
    return htons(m_icmp_timestamp_header.identifier);
}


uint16_t icmpTimestamp::Get_seuqence()
{
    return htons(m_icmp_timestamp_header.sequence);
}

uint32_t icmpTimestamp::Get_originate_time_stamp()
{
    return htonl(m_icmp_timestamp_header.originate_time_stamp);
}

uint32_t icmpTimestamp::Get_receive_time_stamp()
{
    return htonl(m_icmp_timestamp_header.receive_time_stamp);
}

uint32_t icmpTimestamp::Get_transmit_time_stamp()
{
    return htonl(m_icmp_timestamp_header.transmit_time_stamp);
}

void icmpTimestamp::Set_type(uint8_t type)
{
    m_icmp_timestamp_header.type=type;
}

void icmpTimestamp::Set_code(uint8_t code)
{
    m_icmp_timestamp_header.code=code;
}

void icmpTimestamp::Set_checksum(uint16_t checksum)
{
    m_icmp_timestamp_header.checksum = htons(checksum);
}

void icmpTimestamp::Set_identifer(uint16_t id)
{
    m_icmp_timestamp_header.identifier = htons(id);
}

void icmpTimestamp::Set_sequence(uint16_t seq)
{
    m_icmp_timestamp_header.sequence = htons(seq);
}

void icmpTimestamp::Set_originate_time_stamp(uint32_t originate_time_stamp)
{
    m_icmp_timestamp_header.originate_time_stamp = htonl(originate_time_stamp);
}

const char * icmpTimestamp::Get_header_data()
{
    return ( char *)&m_icmp_timestamp_header;
}


void icmpTimestamp::Set_CorrectHeaderChecksum()
{
    char *data = (char*) malloc (sizeof(icmp_time_stamp_header));
    memcpy(data, this->Get_header_data(), sizeof(icmp_time_stamp_header));
    this->Set_checksum(htons(in_cksum((unsigned short*)data,sizeof(icmp_time_stamp_header))));
    free(data);
}
