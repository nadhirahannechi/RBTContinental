#ifndef ICMP_TIME_STAMP_H
#define ICMP_TIME_STAMP_H

#include "layer.h"
#include "common.h"
#include "payload.h"


typedef struct 
{
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
    u_int16_t	identifier;
    u_int16_t	sequence;
    uint32_t  originate_time_stamp;
    uint32_t  receive_time_stamp;
    uint32_t  transmit_time_stamp;
}icmp_time_stamp_header;

class icmpTimestamp : public layer
{
    public:
    icmpTimestamp();
    icmpTimestamp( char *);
    ~icmpTimestamp();
    uint8_t Get_type();
    uint8_t Get_code();
    uint16_t Get_checksum();
    uint16_t Get_identifier();
    uint16_t Get_seuqence();
    uint32_t Get_originate_time_stamp();
    uint32_t Get_receive_time_stamp();
    uint32_t Get_transmit_time_stamp();
    void Set_type(uint8_t);
    void Set_code(uint8_t);
    void Set_checksum(uint16_t);
    void Set_identifer(uint16_t);
    void Set_sequence(uint16_t);
    void Set_originate_time_stamp(uint32_t);
    void Set_CorrectHeaderChecksum();
    virtual const char * Get_header_data();
    private:
    icmp_time_stamp_header m_icmp_timestamp_header;
};


#endif