#ifndef ICMP_H
#define ICMP_H

#include "layer.h"
#include "common.h"
#include "payload.h"


typedef struct 
{
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
    union
    {
        struct
        {
            u_int16_t	identifier;
            u_int16_t	sequence;
        } echo1;	
        struct 
        {
            uint8_t ptr;
            uint8_t data[3];
        }echo2;
    }un;
}icmp_header;

class icmp : public layer
{
    public:
    icmp();
    icmp( char *);
    ~icmp();
    uint8_t Get_type();
    uint8_t Get_code();
    uint16_t Get_checksum();
    uint16_t Get_identifier();
    uint16_t Get_seuqence();
    uint8_t Get_pointer();
    void Set_type(uint8_t);
    void Set_code(uint8_t);
    void Set_checksum(uint16_t);
    void Set_identifer(uint16_t);
    void Set_sequence(uint16_t);
    void Set_CorrectHeaderChecksum(char*);
    virtual const char * Get_header_data();
    private:
    icmp_header m_icmp_header;
};


#endif